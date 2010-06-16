#pragma once

class PositionHelper
{
	Preset _preset;
	double _l,_r,_c;

public:
	PositionHelper(const Preset& preset,
		const double& l,const double& r, const double& c)
		: _preset(preset), _l(l), _r(r), _c(c) { }
	double L() const;
	double R() const;
	static double GetDividerFreq(uint8 freqSlider);

private:
	PositionHelper();
	PositionHelper(const PositionHelper&);
	double LL() const;
	double LR() const;
	double RL() const;
	double RR() const;
	double ML() const;
	double MR() const;
	double C() const;
	double S() const;
	double NormalizeMultiplier(double multiplier, double max) const;
};
