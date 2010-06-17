#pragma once

enum CenterMode{ None = 0, LowToSides = 1, HighToSides = 2 };
enum BalanceMode{ Cutter = 0, Filler = 1, Mono = 2 };

struct Preset
{
	uint8 BalanceSlider;
	uint8 CenterSlider;
	uint8 FreqSlider;
	CenterMode CenterModeSetting;
	BalanceMode BalanceModeSetting;
	std::string Name;

	Preset();
	Preset(uint8 balanceSlider, uint8 centerSlider, uint8 freqSlider,
	       CenterMode centerMode, BalanceMode balanceMode, const char* name);
	Preset(const Preset& from);
	Preset& operator=(const Preset& from);
};

typedef std::map<DWORD, Preset> PresetContainer;
