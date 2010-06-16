#include "general.h"
#include "PositionHelper.h"
#include <math.h>
#include "Config.h"

// NOTE: disable stupid enum warning
#pragma warning(disable : 4482)

double PositionHelper::L() const
{
	return (_l*LL() + _r*RL())*S() + _c*C();
}

double PositionHelper::R() const
{
	return (_l*LR() + _r*RR())*S() + _c*C();
}

double PositionHelper::GetDividerFreq(uint8 freqSlider)
{
	return 20 * pow(10.0, static_cast<double>(freqSlider) / (static_cast<double>(MAXFREQSLIDER) / 3.0));
}

double PositionHelper::LL() const
{
	return _preset.BalanceModeSetting == BalanceMode::Mono
			? ML()
			: NormalizeMultiplier(
						static_cast<double>(2*MAXBALANCESLIDER - 2*_preset.BalanceSlider),
						static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::LR() const
{
	return _preset.BalanceModeSetting == BalanceMode::Mono
			? ML()
			: _preset.BalanceModeSetting == BalanceMode::Cutter
				? 0.0
				: NormalizeMultiplier(
						static_cast<double>(MAXBALANCESLIDER - 2*_preset.BalanceSlider),
						static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::RL() const
{
	return _preset.BalanceModeSetting == BalanceMode::Mono
			? MR()
			: _preset.BalanceModeSetting == BalanceMode::Cutter
				? 0.0
				: NormalizeMultiplier(
						static_cast<double>(2*_preset.BalanceSlider - MAXBALANCESLIDER),
						static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::RR() const
{
	return _preset.BalanceModeSetting == BalanceMode::Mono
			? MR()
			: NormalizeMultiplier(
						static_cast<double>(2*_preset.BalanceSlider),
						static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::ML() const
{
	return NormalizeMultiplier(
					static_cast<double>(MAXBALANCESLIDER - _preset.BalanceSlider),
					static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::MR() const
{
	return NormalizeMultiplier(
					static_cast<double>(_preset.BalanceSlider),
					static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::C() const
{
	return NormalizeMultiplier(
					static_cast<double>(_preset.CenterSlider),
					static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::S() const
{
	return NormalizeMultiplier(
					static_cast<double>(MAXBALANCESLIDER - _preset.CenterSlider),
					static_cast<double>(MAXBALANCESLIDER));
}

double PositionHelper::NormalizeMultiplier(double multiplier, double max) const
{
	if(multiplier > max) multiplier = max;
	else if(multiplier < 0.0) multiplier = 0.0;
	double result = log10(9.0*multiplier/max + 1.0);
	if(result < 0.0) result = 0.0;
	return result;
}
