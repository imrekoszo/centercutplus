#include "general.h"
#include "controlhelpers.h"
#include "resource.h"
#include "ConfigurationStore.h"
#include <Shlwapi.h>
#include "ConfigWindow.h"
#include <WindowsX.h>
#include "PositionHelper.h"

// NOTE: disable stupid enum warning
#pragma warning(disable : 4482)

void ConfigWindow::CreateConfigWindow(HINSTANCE hInst, HWND hParentWnd)
{
	InitConfig(static_cast<HMODULE>(hInst));
	_parentWnd() = hParentWnd;
	::CreateDialog(hInst, MAKEINTRESOURCE(IDD_CONFIG), NULL, ConfigWindowProc);
}

void ConfigWindow::DestroyConfigWindow()
{
	if(::IsWindow(_hwnd()))
	{
		::DestroyWindow(_hwnd());
	}
	_configStore()->Store(_presets(), _activePresetKey(), Config::IsBypassed());
}

void ConfigWindow::InitConfig(HMODULE hmodule)
{
	char buffer[1000];
	::GetModuleFileName(hmodule, buffer, 1000);
	sprintf_s(::PathFindExtension(buffer), 6, ".ini");
	_configStore().reset(new ConfigurationStore(buffer));
	_configStore()->Read(_presets(), _activePresetKey(), Config::IsBypassed());
}

INT_PTR CALLBACK ConfigWindow::ConfigWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND sender = reinterpret_cast<HWND>(lParam);
	WORD hiwp = HIWORD(wParam);
	WORD lowp = LOWORD(wParam);

	switch (message)
	{
		case WM_INITDIALOG:
			{
				_hwnd() = hDlg;
				_presetWnd() = ::GetDlgItem(_hwnd(), IDC_PRESET);

				HMENU menu = ::GetSystemMenu(_hwnd(), FALSE);
				::EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND|MF_DISABLED);
				::InsertMenu(menu, 0u, MF_SEPARATOR, NULL, NULL);

				InitSlider(_hwnd(), IDC_BALANCE, MAXBALANCESLIDER, 0);
				InitSlider(_hwnd(), IDC_CENTER, MAXCENTERSLIDER, 0);
				InitSlider(_hwnd(), IDC_FREQ, MAXFREQSLIDER, 0);

				InitPresets();
				SetCheck(_hwnd(), IDC_BYPASS, Config::IsBypassed());

				//HICON icon = reinterpret_cast<HICON>(
				//	::SendMessage(_parentWnd(), WM_GETICON, static_cast<WPARAM>(ICON_BIG), static_cast<LPARAM>(0)));
				//::SendMessage(_hwnd(), WM_SETICON, static_cast<WPARAM>(ICON_BIG), reinterpret_cast<LPARAM>(icon));
				//icon = reinterpret_cast<HICON>(
				//	::SendMessage(_parentWnd(), WM_GETICON, static_cast<WPARAM>(ICON_SMALL), static_cast<LPARAM>(0)));
				//::SendMessage(_hwnd(), WM_SETICON, static_cast<WPARAM>(ICON_SMALL), reinterpret_cast<LPARAM>(icon));
			}
			return static_cast<INT_PTR>(TRUE);

		case WM_HSCROLL:
			{
				SliderValueChanged();
			}
			break;
		case WM_COMMAND:
			if(hiwp == BN_CLICKED)
			{
				if(lowp == IDC_NONE || lowp == IDC_LOW || lowp == IDC_HIGH)
				{
					Config::GetPreset().CenterModeSetting = GetCheck(_hwnd(), IDC_NONE)
															? CenterMode::None
															: GetCheck(_hwnd(), IDC_LOW)
																? CenterMode::LowToSides
																: CenterMode::HighToSides;
				}
				else if(lowp == IDC_CUTTER || lowp == IDC_FILLER || lowp == IDC_MONO)
				{
					Config::GetPreset().BalanceModeSetting = GetCheck(_hwnd(), IDC_CUTTER)
															? BalanceMode::Cutter
															: GetCheck(_hwnd(), IDC_FILLER)
																? BalanceMode::Filler
																: BalanceMode::Mono;
				}
				else if(lowp == IDC_BYPASS)
					Config::IsBypassed() = GetCheck(_hwnd(), IDC_BYPASS);
				else if(lowp == IDC_DELETEPRESET)
					DeleteSelectedPreset();
				else if(lowp == IDC_ADDPRESET)
					AddPreset();
			}
			else if(hiwp == STN_DBLCLK)
			{
				if(lowp == IDC_RESETCENTER || lowp == IDC_RESETBALANCE)
				{
					ResetSlider(_hwnd(), lowp == IDC_RESETCENTER ? IDC_CENTER : IDC_BALANCE);
					SliderValueChanged();
				}
				else if(lowp == IDC_RESETFREQ)
				{
					SetSliderPos(_hwnd(), IDC_FREQ, MAXFREQSLIDER/3);
					SliderValueChanged();
				}
			}
			else if(hiwp == CBN_SELENDOK && lowp == IDC_PRESET)
			{
				PresetSelectionChanged();
			}
			break;
	}
	return static_cast<INT_PTR>(FALSE);
}

void ConfigWindow::ApplyActivePreset()
{
	Config::GetPreset() = _presets()[_activePresetKey()];
	SetSliderPos(_hwnd(), IDC_CENTER, Config::GetPreset().CenterSlider);
	SetSliderPos(_hwnd(), IDC_BALANCE, Config::GetPreset().BalanceSlider);
	SetSliderPos(_hwnd(), IDC_FREQ, Config::GetPreset().FreqSlider);

	CenterMode centerMode = Config::GetPreset().CenterModeSetting;
	CheckRadioButton(_hwnd(), IDC_NONE, IDC_HIGH,
						centerMode == CenterMode::None
							? IDC_NONE
							: centerMode == CenterMode::LowToSides ? IDC_LOW : IDC_HIGH);

	BalanceMode balanceMode = Config::GetPreset().BalanceModeSetting;
	CheckRadioButton(_hwnd(), IDC_CUTTER, IDC_MONO,
						balanceMode == BalanceMode::Cutter
							? IDC_CUTTER
							: balanceMode == BalanceMode::Filler ? IDC_FILLER : IDC_MONO);

	RefreshSliderValueDisplays();
}

void ConfigWindow::RefreshSliderValueDisplays()
{
	SetLabel(_hwnd(), IDC_BALANCEV, Config::GetPreset().BalanceSlider);
	SetLabel(_hwnd(), IDC_CENTERV, Config::GetPreset().CenterSlider);
	SetLabel(_hwnd(), IDC_FREQV,
		static_cast<UINT>(PositionHelper::GetDividerFreq(Config::GetPreset().FreqSlider)));
}

void ConfigWindow::InitPresets()
{
	for(PresetContainer::const_iterator iter = _presets().begin(); iter != _presets().end(); iter++)
	{
		AddPresetToCombo(_hwnd(), IDC_PRESET, iter->first, iter->second);
	}

	int count = ComboBox_GetCount(_presetWnd());
	for(int i = 0; i < count; i++)
	{
		DWORD itemData = static_cast<DWORD>(ComboBox_GetItemData(_presetWnd(), i));
		if(itemData == _activePresetKey())
		{
			ComboBox_SetCurSel(_presetWnd(), i);
			break;
		}
	}
	ApplyActivePreset();
}

void ConfigWindow::PresetSelectionChanged()
{
	int index = ComboBox_GetCurSel(_presetWnd());
	if(index != CB_ERR)
	{
		_activePresetKey() = static_cast<DWORD>(ComboBox_GetItemData(_presetWnd(), index));
		ApplyActivePreset();
	}
}

void ConfigWindow::DeleteSelectedPreset()
{
	int index = ComboBox_GetCurSel(_presetWnd());
	if(index != CB_ERR)
	{
		int mbResult = ::MessageBox(
							_hwnd(),
							"Would you really like to delete the selected preset?",
							"Confirm delete",
							MB_ICONQUESTION | MB_YESNO);
		if(mbResult == IDYES)
		{
			_presets().erase(static_cast<DWORD>(ComboBox_GetItemData(_presetWnd(), index)));
			ComboBox_DeleteString(_presetWnd(), index);
			ComboBox_SetText(_presetWnd(), "");
		}
	}
	else
	{
		ComboBox_SetText(_presetWnd(), "");
	}
}

void ConfigWindow::AddPreset()
{
	int nameLength = ComboBox_GetTextLength(_presetWnd()) + 1;
	std::vector<char> nameBuffer(nameLength);
	char* name = &(nameBuffer[0]);
	ComboBox_GetText(_presetWnd(), name, nameLength);
	int index = ComboBox_FindStringExact(_presetWnd(), -1, name);
	DWORD presetKey;
	if(index != CB_ERR)
	{
		int mbResult = ::MessageBox(
							_hwnd(),
							"A preset with the specified name already exists.\r\n"
							"Would you like to overwrite it?",
							"Preset already exists",
							MB_ICONQUESTION | MB_YESNO);
		if(mbResult != IDYES)
			return;

		presetKey = ComboBox_GetItemData(_presetWnd(), index);
	}
	else
	{
		index = ComboBox_AddString(_presetWnd(), name);
		presetKey = ConfigurationStore::NextPresetKey();
		ComboBox_SetItemData(_presetWnd(), index, presetKey);
	}

	Config::GetPreset().Name = name;
	_presets()[presetKey] = Config::GetPreset();
	ComboBox_SetCurSel(_presetWnd(), index);
	_activePresetKey() = presetKey;
}

void ConfigWindow::SliderValueChanged()
{
	Config::GetPreset().BalanceSlider = GetSliderPos(_hwnd(), IDC_BALANCE);
	Config::GetPreset().CenterSlider = GetSliderPos(_hwnd(), IDC_CENTER);
	Config::GetPreset().FreqSlider = GetSliderPos(_hwnd(), IDC_FREQ);
	RefreshSliderValueDisplays();
}
