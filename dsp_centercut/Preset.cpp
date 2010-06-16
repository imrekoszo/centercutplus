#include "general.h"
#include "Preset.h"

// NOTE: disable stupid enum warning
#pragma warning(disable : 4482)

Preset::Preset()
	:   BalanceSlider(MAXBALANCESLIDER/2),
	    CenterSlider(MAXCENTERSLIDER/2),
	    FreqSlider(MAXFREQSLIDER/3),
	    CenterModeSetting(CenterMode::None),
	    BalanceModeSetting(BalanceMode::Cutter),
	    Name("")
{
}

Preset::Preset(uint8 balanceSlider, uint8 centerSlider, uint8 freqSlider,
               CenterMode centerMode, BalanceMode balanceMode, const char* name)
	:   BalanceSlider(balanceSlider),
	    CenterSlider(centerSlider),
	    FreqSlider(freqSlider),
	    CenterModeSetting(centerMode),
	    BalanceModeSetting(balanceMode),
	    Name(name)
{
}

Preset::Preset(const Preset& from)
	:   BalanceSlider(from.BalanceSlider),
	    CenterSlider(from.CenterSlider),
	    FreqSlider(from.FreqSlider),
	    CenterModeSetting(from.CenterModeSetting),
	    BalanceModeSetting(from.BalanceModeSetting),
	    Name(from.Name)
{
}

Preset& Preset::operator=(const Preset& from)
{
	BalanceSlider = from.BalanceSlider;
	CenterSlider = from.CenterSlider;
	FreqSlider = from.FreqSlider;
	CenterModeSetting = from.CenterModeSetting;
	BalanceModeSetting = from.BalanceModeSetting;
	Name = from.Name;
	return *this;
}
