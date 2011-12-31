#include "../inc/centercutplus/ui/configdialog.h"

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
void SetSliderPos(HWND hDlg, UINT sliderId, T position)
{
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPOS, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(position));
}

template<typename T>
T GetSliderPos(HWND hDlg, UINT sliderId)
{
    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETPOS, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

template<typename T>
void SetSliderMax(HWND hDlg, UINT sliderId, T max)
{
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETRANGEMAX, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(max));
}

//template<typename T>
//T GetSlidermaximum(HWND hDlg, UINT sliderId)
//{
//    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
//}

template<typename T>
void InitSlider(HWND hDlg, UINT sliderId, T max)
{
    SetSliderMax<T>(hDlg, sliderId, max);
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETLINESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(1));
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPAGESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(max/10));
}

//void ResetSlider(HWND hDlg, UINT sliderId)
//{
//    SetSliderPos(hDlg, sliderId, GetSlidermaximum(hDlg, sliderId)/2);
//}

//void SetCheck(HWND hDlg, UINT checkId, bool check)
//{
//    ::SendDlgItemMessage(hDlg, checkId, BM_SETCHECK, static_cast<WPARAM>(check ? BST_CHECKED : BST_UNCHECKED), static_cast<LPARAM>(0));
//}
//
//bool GetCheck(HWND hDlg, UINT checkId)
//{
//    return ::SendDlgItemMessage(hDlg, checkId, BM_GETCHECK, static_cast<WPARAM>(0), static_cast<LPARAM>(0)) == BST_CHECKED;
//}
//
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
    InitOutputSlider(IDC_LL);
    InitOutputSlider(IDC_CL);
    InitOutputSlider(IDC_RL);
    TryInitControls();
}

void ConfigDialog::OnVScroll(LPARAM lParam)
{
    if(IsOutputSliderId(::GetDlgCtrlID(reinterpret_cast<HWND>(lParam))))
    {
        ReadOutputSlider(IDC_LL);
        ReadOutputSlider(IDC_CL);
        ReadOutputSlider(IDC_RL);
    }
}

void ConfigDialog::Update(const void* origin)
{
    if(origin != this)
    {
        UpdateOutputSlider(IDC_LL);
        UpdateOutputSlider(IDC_CL);
        UpdateOutputSlider(IDC_RL);
    }
}

void ConfigDialog::TryInitControls()
{
    if(!_model || !::IsWindow(HDlg()))
    {
        return;
    }

    UpdateOutputSlider(IDC_LL);
    UpdateOutputSlider(IDC_CL);
    UpdateOutputSlider(IDC_RL);
}

void ConfigDialog::TryUnsubscribe()
{
    if(_model)
    {
        _model->Unsubscribe(*this);
    }
}

void ConfigDialog::InitOutputSlider(UINT sliderId)
{
    InitSlider<int>(HDlg(), sliderId, core::EngineConfig::kMaxPercent - core::EngineConfig::kMinPercent);
}

void ConfigDialog::UpdateOutputSlider(UINT sliderId)
{
    int percentValue = CALL_MEMBER_FN(_model->GetCurrentEngineConfig(), GetOutputSliderMetadata(sliderId).getter)();
    SetSliderPos<int>(HDlg(), sliderId, percentValue - core::EngineConfig::kMinPercent);
}

void ConfigDialog::ReadOutputSlider(UINT sliderId)
{
    int sliderValue = GetSliderPos<int>(HDlg(), sliderId);
    CALL_MEMBER_FN(*_controller, GetOutputSliderMetadata(sliderId).setter)(sliderValue + core::EngineConfig::kMinPercent, this);
}

ConfigDialog::OutputSliderMetadata& ConfigDialog::GetOutputSliderMetadata(UINT sliderId)
{
    static boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& metadata = InitOutputSliderMetadata();
    return metadata[sliderId];
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

    return metadata;
}

bool ConfigDialog::IsOutputSliderId(UINT id)
{
    static boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& metadata = InitOutputSliderMetadata();
    return metadata.find(id) != metadata.end();
}

}
}
