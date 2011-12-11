#pragma once

void SetSliderPos(HWND hDlg, UINT sliderId, uint8 position);
uint8 GetSliderPos(HWND hDlg, UINT sliderId);
void SetSliderRange(HWND hDlg, UINT sliderId, uint8 max);
uint8 GetSliderMax(HWND hDlg, UINT sliderId);
void InitSlider(HWND hDlg, UINT sliderId, uint8 max, uint8 position);
void ResetSlider(HWND hDlg, UINT sliderId);

void SetCheck(HWND hDlg, UINT checkId, bool check);
bool GetCheck(HWND hDlg, UINT checkId);

void SetLabel(HWND hDlg, UINT labelId, UINT value);

void AddPresetToCombo(HWND hDlg, UINT comboId, DWORD presetKey, const Preset& preset);
