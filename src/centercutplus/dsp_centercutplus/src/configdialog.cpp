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
    T max = GetSliderMax<T>(hDlg, sliderId);
    T min = GetSliderMin<T>(hDlg, sliderId);
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPOS, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(min + max - value));
}

template<typename T>
T GetVerticalSliderValue(HWND hDlg, UINT sliderId)
{
    T max = GetSliderMax<T>(hDlg, sliderId);
    T min = GetSliderMin<T>(hDlg, sliderId);
    T pos = GetSliderPos<T>(hDlg, sliderId);
    return min + max - pos;
}

template<typename T>
void SetSliderRange(HWND hDlg, UINT sliderId, T min, T max)
{
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETRANGE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(MAKELONG(min, max)));
}

template<typename T>
T GetSliderMax(HWND hDlg, UINT sliderId)
{
    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

template<typename T>
T GetSliderMin(HWND hDlg, UINT sliderId)
{
    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMIN, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

template<typename T>
T GetSliderPos(HWND hDlg, UINT sliderId)
{
    return static_cast<T>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETPOS, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

template<typename T>
void SetSliderPos(HWND hDlg, UINT sliderId, T pos)
{
    ::SendDlgItemMessage(hDlg, sliderId, TBM_SETPOS, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(pos));
}

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

INT_PTR ConfigDialog::DlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR baseResult = Dialog::DlgProc(message, wParam, lParam);

    switch(message)
    {
        case WM_INITDIALOG: return static_cast<INT_PTR>(OnInitDialog());
        case WM_VSCROLL:    return static_cast<INT_PTR>(OnVScroll(lParam));
        case WM_COMMAND:    return static_cast<INT_PTR>(OnCommand(HIWORD(wParam), LOWORD(wParam), lParam));
        case WM_DESTROY:    return static_cast<INT_PTR>(OnDestroy());
    }

    return baseResult;
}

BOOL ConfigDialog::OnInitDialog()
{
    InitOutputSliders();
    TryInitControlsWithValues();
    return TRUE; // return FALSE if setting focus ourselves
}

BOOL ConfigDialog::OnVScroll(LPARAM lParam)
{
    UINT id = static_cast<UINT>(::GetDlgCtrlID(reinterpret_cast<HWND>(lParam)));
    if(IsOutputSliderId(id))
    {
        SaveOutputSliderPercentValue(id);
        UpdateLabels();
        return TRUE;
    }

    return FALSE;
}

BOOL ConfigDialog::OnCommand(WORD wParamHi, WORD wParamLo, LPARAM lParam)
{
    switch(wParamHi)
    {
        case BN_CLICKED:
            OnButtonClicked(static_cast<UINT>(wParamLo), reinterpret_cast<HWND>(lParam));
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

BOOL ConfigDialog::OnButtonClicked(UINT buttonId, HWND buttonHandle)
{
    switch(buttonId)
    {
        case IDC_BYPASS:
            SaveBypass();
            break;
        case IDC_INPHASE:
        case IDC_INVLEFT:
        case IDC_INVRIGHT:
        case IDC_BOTHLEFT:
        case IDC_BOTHRIGHT:
            SaveCenterDetectionMode();
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

BOOL ConfigDialog::OnDestroy()
{
    TryUnsubscribe();
    return TRUE;
}

void ConfigDialog::SetController(configuration::ConfigController& controller)
{
    _controller = &controller;
    _model = _controller->Model();
    _model->Subscribe(*this);
    TryInitControlsWithValues();
}

void ConfigDialog::Update(const void* origin)
{
    if(origin != this)
    {
        UpdateBypass();
        UpdateCenterDetectionMode();
        UpdateOutputSliders();
        UpdateLabels();
    }
}

bool ConfigDialog::IsBackendReady()
{
    return _controller != NULL && _model != NULL;
}

void ConfigDialog::TryInitControlsWithValues()
{
    if(IsWindow() && IsBackendReady())
    {
        UpdateBypass();
        UpdateOutputSliders();
        UpdateLabels();
        UpdateCenterDetectionMode();
        UpdateFreqSection();
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
    if(IsBackendReady())
    {
        CALL_MEMBER_FN(*_controller, GetOutputSliderMetadata()[sliderId].setter)(GetOutputSliderPercentValue(sliderId), this);
    }
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
    if(IsBackendReady())
    {
        _controller->SetBypassed(GetCheck(HDlg(), IDC_BYPASS), this);
    }
}

void ConfigDialog::UpdateCenterDetectionMode()
{
    core::CenterDetectionMode newValue = _model->GetCurrentEngineConfig().centerDetectionMode();
    core::CenterDetectionMode oldValue = GetCenterDetectionMode();

    if(newValue != oldValue)
    {
        UINT checkId = IDC_INPHASE;
        switch(newValue)
        {
            case core::kCenterDetectionMode_InPhase:
                checkId = IDC_INPHASE;
                break;
            case core::kCenterDetectionMode_InversePhaseLeft:
                checkId = IDC_INVLEFT;
                break;
            case core::kCenterDetectionMode_InversePhaseRight:
                checkId = IDC_INVRIGHT;
                break;
            case core::kCenterDetectionMode_BothPhaseLeft:
                checkId = IDC_BOTHLEFT;
                break;
            case core::kCenterDetectionMode_BothPhaseRight:
                checkId = IDC_BOTHRIGHT;
                break;
        }

        ::CheckRadioButton(HDlg(), IDC_INPHASE, IDC_BOTHRIGHT, checkId);
    }
}

core::CenterDetectionMode ConfigDialog::GetCenterDetectionMode()
{
    if(GetCheck(HDlg(), IDC_INPHASE))
    {
        return core::kCenterDetectionMode_InPhase;
    }
    if(GetCheck(HDlg(), IDC_INVLEFT))
    {
        return core::kCenterDetectionMode_InversePhaseLeft;
    }
    if(GetCheck(HDlg(), IDC_INVRIGHT))
    {
        return core::kCenterDetectionMode_InversePhaseRight;
    }
    if(GetCheck(HDlg(), IDC_BOTHLEFT))
    {
        return core::kCenterDetectionMode_BothPhaseLeft;
    }
    if(GetCheck(HDlg(), IDC_BOTHRIGHT))
    {
        return core::kCenterDetectionMode_BothPhaseRight;
    }

    return static_cast<core::CenterDetectionMode>(core::kCenterDetectionMode_Max + 1);
}

void ConfigDialog::SaveCenterDetectionMode()
{
    if(IsBackendReady())
    {
        _controller->SetCenterDetectionMode(GetCenterDetectionMode(), this);
    }
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

boost::ptr_map<UINT, ConfigDialog::FreqSliderMetadata>& ConfigDialog::GetFreqSliderMetadata()
{
    static auto& metadata = InitFreqSliderMetadata();
    return metadata;
}

boost::ptr_map<UINT, ConfigDialog::FreqSliderMetadata>& ConfigDialog::InitFreqSliderMetadata()
{
    static boost::ptr_map<UINT, ConfigDialog::FreqSliderMetadata> metadata;

    UINT key = IDC_FROM;
    metadata.insert(key,
                    new ConfigDialog::FreqSliderMetadata(&core::FrequencyInterval::minimum,
                                                         &configuration::ConfigController::SetFreqMin,
                                                         IDC_FROML,
                                                         core::FrequencyInterval::kMinFrequency,
                                                         &core::FrequencyInterval::maximum));

    key = IDC_TO;
    metadata.insert(key,
                    new ConfigDialog::FreqSliderMetadata(&core::FrequencyInterval::maximum,
                                                         &configuration::ConfigController::SetFreqMax,
                                                         IDC_TOL,
                                                         &core::FrequencyInterval::minimum,
                                                         core::FrequencyInterval::kMaxFrequency));

    return metadata;
}

bool ConfigDialog::IsFreqSliderId(UINT id)
{
    static auto& metadata = GetFreqSliderMetadata();
    return metadata.find(id) != metadata.end();
}

void ConfigDialog::UpdateFreqSection()
{
    static auto& metadata = GetFreqSliderMetadata();

    LRESULT selectedFreqIndex = GetSelectedFreqIntervalIndex();

    if(selectedFreqIndex == LB_ERR)
    {
        EnableControl(IDC_DELFREQ, false);

        BOOST_FOREACH(auto md, metadata)
        {
            EnableControl(md.first, false);
            UpdateLabel(md.second->labelId, _T(""));
        }
    }
    else
    {
        EnableControl(IDC_DELFREQ, true);
        size_t index = static_cast<size_t>(selectedFreqIndex);
        const core::FrequencyInterval& interval =
            _model->GetCurrentEngineConfig().centerToSidesFrequencyIntervals()[index];

        BOOST_FOREACH(auto md, metadata)
        {
            EnableControl(md.first, true);

            SetSliderRange<uint>(HDlg(), md.first, md.second->Minimum(interval), md.second->Maximum(interval));

            uint value = CALL_MEMBER_FN(interval, md.second->getter)();
            SetSliderPos<uint>(HDlg(), md.first, value);
            UpdateFreqLabel(md.second->labelId, value);
        }
    }
}

LRESULT ConfigDialog::GetSelectedFreqIntervalIndex()
{
    return ::SendDlgItemMessage(HDlg(), IDC_FREQS, LB_GETCURSEL, static_cast<WPARAM>(0), static_cast<LPARAM>(0));
}

void ConfigDialog::UpdateFreqLabel(UINT id, uint value)
{
    tstringstream buffer;
    buffer << value << _T("Hz");
    tstring newValue = buffer.str();

    UpdateLabel(id, newValue);
}

ConfigDialog::FreqSliderMetadata::FreqSliderMetadata()
    : getter(NULL),
      setter(NULL),
      labelId(0u),
      _getMin(NULL),
      _max(0u),
      _min(0u),
      _getMax(NULL)
{
}

ConfigDialog::FreqSliderMetadata::FreqSliderMetadata(
    ConfigDialog::GetFreqMemFn getter,
    ConfigDialog::SetFreqMemFn setter,
    UINT labelId,
    ConfigDialog::GetFreqMemFn getMin,
    uint max)
    : getter(getter), setter(setter), labelId(labelId), _getMin(getMin), _max(max), _min(0u), _getMax(NULL)
{
}

ConfigDialog::FreqSliderMetadata::FreqSliderMetadata(
    ConfigDialog::GetFreqMemFn getter,
    ConfigDialog::SetFreqMemFn setter,
    UINT labelId,
    uint min,
    ConfigDialog::GetFreqMemFn getMax)
    : getter(getter), setter(setter), labelId(labelId), _min(min), _getMax(getMax), _getMin(NULL), _max(0u)
{
}

uint ConfigDialog::FreqSliderMetadata::Minimum(const core::FrequencyInterval& interval)
{
    return _getMin ? CALL_MEMBER_FN(interval, _getMin)() : _min;
}

uint ConfigDialog::FreqSliderMetadata::Maximum(const core::FrequencyInterval& interval)
{
    return _getMax ? CALL_MEMBER_FN(interval, _getMax)() : _max;
}

}
}
