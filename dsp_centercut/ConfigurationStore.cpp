#include "general.h"
#include "ConfigurationStore.h"

// NOTE: disable stupid enum warning
#pragma warning(disable : 4482)

ConfigurationStore::ConfigurationStore(const char* iniPath)
	: _iniPath(iniPath)
{
}

void ConfigurationStore::Read(PresetContainer& presetBuffer, DWORD& activePresetKey, bool& bypass)
{
	ReadPresets(presetBuffer, activePresetKey);
	ReadBypass(bypass);
}

void ConfigurationStore:: Store(const PresetContainer& presets, DWORD activePresetKey, bool bypass)
{
	const char* path = _iniPath.c_str();
	char buf[256];

	sprintf_s(buf, 256, "%u", bypass ? 1 : 0);
	::WritePrivateProfileString(_generalSection(), _bypassed(), buf, path);

	sprintf_s(buf, 256, "%u", presets.size());
	::WritePrivateProfileString(_generalSection(), _presetCount(), buf, path);

	UINT index = 0;
	for(PresetContainer::const_iterator iter = presets.begin(); iter != presets.end(); iter++)
	{
		if(activePresetKey == iter->first)
		{
			sprintf_s(buf, 256, "%u", index);
			::WritePrivateProfileString(_generalSection(), _lastPreset(), buf, path);
		}

		const Preset& preset = iter->second;
		char section[64];
		GetSectionName(index, section, 64);

		sprintf_s(buf, 256, "%u", preset.CenterSlider);
		::WritePrivateProfileString(section, _centerSlider(), buf, path);

		sprintf_s(buf, 256, "%u", preset.BalanceSlider);
		::WritePrivateProfileString(section, _balanceSlider(), buf, path);

		sprintf_s(buf, 256, "%u", preset.FreqSlider);
		::WritePrivateProfileString(section, _freqSlider(), buf, path);

		sprintf_s(buf, 256, "%i", static_cast<int>(preset.CenterModeSetting));
		::WritePrivateProfileString(section, _centerMode(), buf, path);

		sprintf_s(buf, 256, "%i", static_cast<int>(preset.BalanceModeSetting));
		::WritePrivateProfileString(section, _balanceMode(), buf, path);

		::WritePrivateProfileString(section, _name(), preset.Name.c_str(), path);
		++index;
	}
}

void ConfigurationStore::ReadPresets(PresetContainer& presetBuffer, DWORD& activePresetKey)
{
	presetBuffer.clear();

	UINT storedPresetCount = ::GetPrivateProfileInt(_generalSection(), _presetCount(), 0, _iniPath.c_str());
	UINT storedActivePreset = ::GetPrivateProfileInt(_generalSection(), _lastPreset(), 0, _iniPath.c_str());

	Preset buffer;
	for(UINT i = 0; i < storedPresetCount; i++)
	{
		if(!TryReadPreset(i, buffer))
			continue;

		DWORD key = NextPresetKey();
		presetBuffer[key] = buffer;

		if(i == storedActivePreset || presetBuffer.size() == 1)
		{
			activePresetKey = key;
		}
	}

	if(presetBuffer.size() < 1)
	{
		LoadDefaultPresets(presetBuffer, activePresetKey);
	}
}

void ConfigurationStore::ReadBypass(bool& bypass)
{
	bypass =
		(0 != ::GetPrivateProfileInt(_generalSection(), _bypassed(), 1, _iniPath.c_str()));
}

bool ConfigurationStore::TryReadPreset(UINT index, Preset& buffer)
{
	const char* path = _iniPath.c_str();
	char sectionName[64];
	GetSectionName(index, sectionName, 64);

	UINT balanceSlider = ::GetPrivateProfileInt(sectionName, _balanceSlider(), static_cast<UINT>(-1), path);
	UINT centerSlider = ::GetPrivateProfileInt(sectionName, _centerSlider(), static_cast<UINT>(-1), path);
	UINT freqSlider = ::GetPrivateProfileInt(sectionName, _freqSlider(), static_cast<UINT>(-1), path);
	BalanceMode balanceMode = static_cast<BalanceMode>(::GetPrivateProfileInt(sectionName, _balanceMode(), static_cast<UINT>(-1), path));
	CenterMode centerMode = static_cast<CenterMode>(::GetPrivateProfileInt(sectionName, _centerMode(), static_cast<UINT>(-1), path));
	char name[256];
	::GetPrivateProfileString(sectionName, _name(), "", name, 256, path);

	if(balanceSlider > MAXBALANCESLIDER || centerSlider > MAXCENTERSLIDER
		    || (balanceMode != BalanceMode::Cutter && balanceMode != BalanceMode::Filler && balanceMode != BalanceMode::Mono)
		    || (centerMode != CenterMode::None && centerMode != CenterMode::HighToSides && centerMode != CenterMode::LowToSides)
		    || strlen(name) < 1)
		return false;

	buffer.BalanceSlider = static_cast<uint8>(balanceSlider);
	buffer.CenterSlider = static_cast<uint8>(centerSlider);
	buffer.FreqSlider = static_cast<uint8>(freqSlider);
	buffer.BalanceModeSetting = balanceMode;
	buffer.CenterModeSetting = centerMode;
	buffer.Name = name;

	return true;
}

void ConfigurationStore::GetSectionName(UINT presetIndex, char* buffer, size_t buflen)
{
	sprintf_s(buffer, buflen, "%s%u", _presetSectionPrefix(), presetIndex);
}

void ConfigurationStore::LoadDefaultPresets(PresetContainer& presetBuffer, DWORD& activePresetKey)
{
	Preset preset;
	preset.BalanceSlider = MAXCENTERSLIDER/2;
	preset.BalanceModeSetting = BalanceMode::Filler;

	// normal
	preset.CenterModeSetting = CenterMode::None;

	preset.Name = "Sides";
	preset.CenterSlider = 0;
	activePresetKey = NextPresetKey();
	presetBuffer[activePresetKey] = preset; // 'Sides' will be active by default

	preset.Name = "Center";
	preset.CenterSlider = MAXCENTERSLIDER;
	presetBuffer[NextPresetKey()] = preset;

	// low to sides
	preset.CenterModeSetting = CenterMode::LowToSides;

	preset.Name = "Sides (Low to sides)";
	preset.CenterSlider = 0;
	presetBuffer[NextPresetKey()] = preset;

	preset.Name = "Center (Low to sides)";
	preset.CenterSlider = MAXCENTERSLIDER;
	presetBuffer[NextPresetKey()] = preset;

	// high to sides
	preset.CenterModeSetting = CenterMode::HighToSides;

	preset.Name = "Sides (High to sides)";
	preset.CenterSlider = 0;
	presetBuffer[NextPresetKey()] = preset;

	preset.Name = "Center (High to sides)";
	preset.CenterSlider = MAXCENTERSLIDER;
	presetBuffer[NextPresetKey()] = preset;

	// one side
	preset.CenterModeSetting = CenterMode::None;
	preset.BalanceModeSetting = BalanceMode::Mono;
	preset.CenterSlider = 0;

	preset.Name = "Left";
	preset.BalanceSlider = 0;
	presetBuffer[NextPresetKey()] = preset;

	preset.Name = "Right";
	preset.BalanceSlider = MAXBALANCESLIDER;
	presetBuffer[NextPresetKey()] = preset;
}
