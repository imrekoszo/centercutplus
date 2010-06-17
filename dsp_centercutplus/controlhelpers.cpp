#include "general.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "Preset.h"
#include "controlhelpers.h"


void SetSliderPos(HWND hDlg, UINT sliderId, uint8 position)
{
	::SendDlgItemMessage(hDlg, sliderId, TBM_SETPOS, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(position));
}

uint8 GetSliderPos(HWND hDlg, UINT sliderId)
{
	return static_cast<uint8>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETPOS, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

void SetSliderRange(HWND hDlg, UINT sliderId, uint8 max)
{
	::SendDlgItemMessage(hDlg, sliderId, TBM_SETRANGE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(MAKELONG(0, max)));
}

uint8 GetSliderMax(HWND hDlg, UINT sliderId)
{
	return static_cast<uint8>(::SendDlgItemMessage(hDlg, sliderId, TBM_GETRANGEMAX, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

void InitSlider(HWND hDlg, UINT sliderId, uint8 max, uint8 position)
{
	SetSliderRange(hDlg, sliderId, max);
	SetSliderPos(hDlg, sliderId, position);
	::SendDlgItemMessage(hDlg, sliderId, TBM_SETLINESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(1));
	::SendDlgItemMessage(hDlg, sliderId, TBM_SETPAGESIZE, static_cast<WPARAM>(TRUE), static_cast<LPARAM>(max/10));
}

void ResetSlider(HWND hDlg, UINT sliderId)
{
	SetSliderPos(hDlg, sliderId, GetSliderMax(hDlg, sliderId)/2);
}

void SetCheck(HWND hDlg, UINT checkId, bool check)
{
	::SendDlgItemMessage(hDlg, checkId, BM_SETCHECK, static_cast<WPARAM>(check ? BST_CHECKED : BST_UNCHECKED), static_cast<LPARAM>(0));
}

bool GetCheck(HWND hDlg, UINT checkId)
{
	return ::SendDlgItemMessage(hDlg, checkId, BM_GETCHECK, static_cast<WPARAM>(0), static_cast<LPARAM>(0)) == BST_CHECKED;
}

void SetLabel(HWND hDlg, UINT labelId, UINT value)
{
	::SetDlgItemInt(hDlg, labelId, value, FALSE);
}

void AddPresetToCombo(HWND hDlg, UINT comboId, DWORD presetKey, const Preset& preset)
{
	DWORD index =
		::SendDlgItemMessage(hDlg, comboId, CB_ADDSTRING, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(preset.Name.c_str()));
	::SendDlgItemMessage(hDlg, comboId, CB_SETITEMDATA, static_cast<WPARAM>(index), static_cast<LPARAM>(presetKey));
}
