#pragma once
#include "ConfigurationStore.h"

class ConfigWindow
{
	static HWND& _hwnd()
	{
		static HWND configWnd;
		return configWnd;
	}
	static HWND& _parentWnd()
	{
		static HWND parentWnd;
		return parentWnd;
	}
	static HWND& _presetWnd()
	{
		static HWND presetWnd;
		return presetWnd;
	}
	static PresetContainer& _presets()
	{
		static PresetContainer presets;
		return presets;
	}
	static std::auto_ptr<ConfigurationStore> &_configStore()
	{
		static std::auto_ptr<ConfigurationStore> configStore;
		return configStore;
	}
	static DWORD& _activePresetKey()
	{
		static DWORD activePresetKey = 0x00000000;
		return activePresetKey;
	}

public:
	static void CreateConfigWindow(HINSTANCE hInst, HWND hParentWnd);
	static void DestroyConfigWindow();

private:
	ConfigWindow();
	static void InitConfig(HMODULE hmodule);
	static INT_PTR CALLBACK ConfigWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void ApplyActivePreset();
	static void RefreshSliderValueDisplays();
	static void InitPresets();
	static void PresetSelectionChanged();
	static void DeleteSelectedPreset();
	static void AddPreset();
	static void SliderValueChanged();
};
