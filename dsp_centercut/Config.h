#pragma once
#include "Preset.h"

class Config
{
private:
	Config();
public:
	static Preset& GetPreset()
	{
		static Preset preset;
		return preset;
	}

	static bool& IsBypassed()
	{
		static bool isBypassed;
		return isBypassed;
	}
};
