#pragma once

class ConfigurationStore
{
	static const char* _generalSection(){ return "General"; }
	static const char* _lastPreset(){ return "LastPreset"; }
	static const char* _presetCount(){ return "PresetCount"; }
	static const char* _bypassed(){ return "Bypassed"; }

	static const char* _presetSectionPrefix(){ return "Preset"; }
	static const char* _centerSlider(){ return "CenterSlider"; }
	static const char* _balanceSlider(){ return "BalanceSlider"; }
	static const char* _freqSlider(){ return "FreqSlider"; }
	static const char* _centerMode(){ return "CenterMode"; }
	static const char* _balanceMode(){ return "BalanceMode"; }
	static const char* _name(){ return "Name"; }

	std::string _iniPath;
public:
	ConfigurationStore(const char* iniPath);
	void Read(PresetContainer& presetBuffer, DWORD& activePresetKey, bool& bypass);
	void Store(const PresetContainer& presets, DWORD activePresetKey, bool bypass);
	static DWORD NextPresetKey()
	{
		static DWORD id = 0x00000000;
		return id++;
	}

private:
	void ReadPresets(PresetContainer& presetBuffer, DWORD& activePresetKey);
	void ReadBypass(bool& bypass);
	bool TryReadPreset(UINT index, Preset& buffer);
	void LoadDefaultPresets(PresetContainer& presetBuffer, DWORD& activePresetKey);
	void GetSectionName(UINT presetIndex, char* buffer, size_t buflen);
};
