#include "../inc/centercutplus/ui/configdialog.h"

#include <CommCtrl.h>
#include <centercutplus/common/types.h>
#include <centercutplus/core/engineconfig.h>
#include <centercutplus/configuration/configcontroller.h>
#include <centercutplus/configuration/iconfigviewmodel.h>

#include "resource.h"

namespace
{
using ccp::uint;

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

namespace ccp
{
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
            {
                InitSlider<int>(HDlg(), IDC_SLIDER1, core::EngineConfig::kMaxPercent - core::EngineConfig::kMinPercent);
                TryInitControls();
            }
            break;
        case WM_VSCROLL:
            {
                _controller->SetLeftToLeftPercent(GetSliderPos<int>(HDlg(), IDC_SLIDER1) + core::EngineConfig::kMinPercent, this);
            }
            break;
        case WM_DESTROY:
            TryUnsubscribe();
            break;
    }

    return baseResult;
}

void ConfigDialog::Update(const void* origin)
{
    if(origin != this)
    {
        SetSliderPos<int>(HDlg(), IDC_SLIDER1, _model->GetCurrentEngineConfig().leftToLeftPercent() - core::EngineConfig::kMinPercent);
    }
}

void ConfigDialog::SetController(configuration::ConfigController& controller)
{
    _controller = &controller;
    _model = _controller->Model();
    _model->Subscribe(*this);
    TryInitControls();
}

void ConfigDialog::TryInitControls()
{
    if(!_model || !::IsWindow(HDlg()))
    {
        return;
    }

    SetSliderPos<int>(HDlg(), IDC_SLIDER1, _model->GetCurrentEngineConfig().leftToLeftPercent() - core::EngineConfig::kMinPercent);
}

void ConfigDialog::TryUnsubscribe()
{
    if(_model)
    {
        _model->Unsubscribe(*this);
    }
}

}
}
