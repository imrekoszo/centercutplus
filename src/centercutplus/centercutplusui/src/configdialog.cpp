#include "../inc/centercutplus/ui/configdialog.h"

#include <boost/foreach.hpp>

#include <CommCtrl.h>
#include <centercutplus/common/types.h>
#include <centercutplus/core/engineconfig.h>
#include <centercutplus/configuration/configcontroller.h>
#include <centercutplus/configuration/iconfigviewmodel.h>

#include "resource.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

namespace ccp
{
namespace
{
template<typename T>
void SetVerticalSliderValue(HWND hDlg, UINT sliderId, T value)
{
    T max = static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
    T min = static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMIN, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPOS, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(min + max - value));
}

template<typename T>
T GetVerticalSliderValue(HWND hDlg, UINT sliderId)
{
    T max = static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
    T min = static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMIN, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
    T pos = static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETPOS, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
    return min + max - pos;
}

template<typename T>
void SetSliderRange(HWND hDlg, UINT sliderId, T min, T max)
{
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETRANGE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(MAKELONG(min, max)));
}

//template<typename T>
//T GetSlidermaximum(HWND hDlg, UINT sliderId)
//{
//    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
//}

template<typename T>
void InitVerticalSlider(HWND hDlg, UINT sliderId, T min, T max)
{
    SetSliderRange<T>(hDlg, sliderId, min, max);
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETLINESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(1));
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPAGESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>((max - min) / 10));
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETTICFREQ, static_cast<WPARAM>((max - min) / 10), static_cast<LPARAM>(0));
}

//void ResetSlider(HWND hDlg, UINT sliderId)
//{
//    SetSliderPos(hDlg, sliderId, GetSlidermaximum(hDlg, sliderId)/2);
//}

void SetCheck(HWND hDlg, UINT checkId, bool check)
{
    ::SendDlgItemMessage(hDlg, checkId, BM_SETCHECK, static_cast<WPARAM>(check ? BST_CHECKED : BST_UNCHECKED), static_cast<LPARAM>(0));
}

bool GetCheck(HWND hDlg, UINT checkId)
{
    return ::SendDlgItemMessage(hDlg, checkId, BM_GETCHECK, static_cast<WPARAM>(0), static_cast<LPARAM>(0)) == BST_CHECKED;
}

//void SetLabel(HWND hDlg, UINT labelId, UINT value)
//{
//    ::SetDlgItemInt(hDlg, labelId, value, FALSE);
//}

//void AddPresetToCombo(HWND hDlg, UINT comboId, DWORD presetKey, const Preset& preset)
//{
//    DWORD index =
//        ::SendDlgItemMessage(hDlg, comboId, CB_ADDSTRING, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(preset.Name.c_str()));
//    ::SendDlgItemMessage(hDlg, comboId, CB_SETITEMDATA, static_cast<WPARAM>(index), static_cast<LPARAM>(presetKey));
//}
}

namespace ui
{

ConfigDialog::ConfigDialog()
    : Dialog(IDD_CONFIG),
      _controller(NULL),
      _model(NULL)
{
}

ConfigDialog::~ConfigDialog()
{
    TryUnsubscribe();
}

INT_PTR ConfigDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR baseResult = Dialog::WindowProc(message, wParam, lParam);

    switch(message)
    {
        case WM_INITDIALOG:
            OnInitDialog();
            break;
        case WM_VSCROLL:
            OnVScroll(lParam);
            break;
        case WM_COMMAND:
            OnCommand(HIWORD(wParam), LOWORD(wParam), lParam);
            break;
        case WM_DESTROY:
            TryUnsubscribe();
            break;
    }

    return baseResult;
}

void ConfigDialog::SetController(configuration::ConfigController& controller)
{
    _controller = &controller;
    _model = _controller->Model();
    _model->Subscribe(*this);
    TryInitControls();
}

void ConfigDialog::OnInitDialog()
{
    InitOutputSliders();
    TryInitControls();
}

void ConfigDialog::OnVScroll(LPARAM lParam)
{
    UINT id = static_cast<UINT>(::GetDlgCtrlID(reinterpret_cast<HWND>(lParam)));
    if(IsOutputSliderId(id))
    {
        SaveOutputSliderPercentValue(id);
        UpdateLabels();
    }
}

void ConfigDialog::OnCommand(WORD wParamHi, WORD wParamLo, LPARAM lParam)
{
    switch(wParamHi)
    {
        case BN_CLICKED:
            if(wParamLo == IDC_BYPASS)
            {
                SaveBypass();
            }
            break;
    }
}

void ConfigDialog::Update(const void* origin)
{
    if(origin != this)
    {
        UpdateBypass();
        UpdateOutputSliders();
        UpdateLabels();
    }
}

bool ConfigDialog::IsBackendReady()
{
    return _controller != NULL && _model != NULL;
}

void ConfigDialog::TryInitControls()
{
    if(IsWindow() && IsBackendReady())
    {
        UpdateBypass();
        UpdateOutputSliders();
        UpdateLabels();
    }
}

void ConfigDialog::TryUnsubscribe()
{
    if(IsBackendReady())
    {
        _model->Unsubscribe(*this);
    }
}

void ConfigDialog::InitOutputSliders()
{
    static auto& metadata = GetOutputSliderMetadata();
    BOOST_FOREACH(auto md, metadata)
    {
        InitVerticalSlider<int>(HDlg(), md.first, core::EngineConfig::kMinPercent, core::EngineConfig::kMaxPercent);
    }
}

void ConfigDialog::UpdateOutputSliders()
{
    static auto& metadata = GetOutputSliderMetadata();
    BOOST_FOREACH(auto md, metadata)
    {
        int oldValue = GetVerticalSliderValue<int>(HDlg(), md.first);
        int newValue = CALL_MEMBER_FN(_model->GetCurrentEngineConfig(), md.second->getter)();
        if(oldValue != newValue)
        {
            SetVerticalSliderValue<int>(HDlg(), md.first, newValue);
        }
    }
}

void ConfigDialog::SaveOutputSliderPercentValue(UINT sliderId)
{
    CALL_MEMBER_FN(*_controller, GetOutputSliderMetadata()[sliderId].setter)(GetOutputSliderPercentValue(sliderId), this);
}

int ConfigDialog::GetOutputSliderPercentValue(UINT sliderId)
{
    return GetVerticalSliderValue<int>(HDlg(), sliderId);
}

void ConfigDialog::UpdateLabels()
{
    static auto& metadata = GetOutputSliderMetadata();
    BOOST_FOREACH(auto md, metadata)
    {
        int percentValue = GetOutputSliderPercentValue(md.first);
        tstringstream buffer;
        buffer << percentValue << _T("%");
        tstring newValue = buffer.str();

        UpdateLabel(md.second->labelId, newValue);
    }
}

void ConfigDialog::UpdateLabel(UINT id, const tstring& newValue)
{
    TCHAR oldValue[100];
    ::GetDlgItemText(HDlg(), id, oldValue, sizeof(oldValue)/sizeof(TCHAR));

    if(newValue.compare(oldValue))
    {
        ::SetDlgItemText(HDlg(), id, newValue.c_str());
    }
}

void ConfigDialog::UpdateBypass()
{
    bool newValue = _model->GetBypassed();
    bool oldValue = GetCheck(HDlg(), IDC_BYPASS);
    if(newValue != oldValue)
    {
        SetCheck(HDlg(), IDC_BYPASS, newValue);
    }
}

void ConfigDialog::SaveBypass()
{
    _controller->SetBypassed(GetCheck(HDlg(), IDC_BYPASS), this);
}

boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& ConfigDialog::GetOutputSliderMetadata()
{
    static auto& metadata = InitOutputSliderMetadata();
    return metadata;
}

boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& ConfigDialog::InitOutputSliderMetadata()
{
    static boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata> metadata;

    UINT key = IDC_LL;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::leftToLeftPercent,
                                                           &configuration::ConfigController::SetLeftToLeftPercent,
                                                           IDC_LLL));

    key = IDC_RL;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::rightToLeftPercent,
                                                           &configuration::ConfigController::SetRightToLeftPercent,
                                                           IDC_RLL));

    key = IDC_CL;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::centerToLeftPercent,
                                                           &configuration::ConfigController::SetCenterToLeftPercent,
                                                           IDC_CLL));

    key = IDC_LR;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::leftToRightPercent,
                                                           &configuration::ConfigController::SetLeftToRightPercent,
                                                           IDC_LRL));

    key = IDC_RR;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::rightToRightPercent,
                                                           &configuration::ConfigController::SetRightToRightPercent,
                                                           IDC_RRL));

    key = IDC_CR;
    metadata.insert(key,
                    new ConfigDialog::OutputSliderMetadata(&core::EngineConfig::centerToRightPercent,
                                                           &configuration::ConfigController::SetCenterToRightPercent,
                                                           IDC_CRL));

    return metadata;
}

bool ConfigDialog::IsOutputSliderId(UINT id)
{
    static auto& metadata = GetOutputSliderMetadata();
    return metadata.find(id) != metadata.end();
}

}
}
