#include "configuration.h"

#include <QList>
#include <QSettings>


namespace
{
    const char* kBypassedName = "isBypassed";
    const char* kPresetsGroupName = "Presets";
    const char* kPresetPrefix = "preset";
    const char* kPresetNameName = "name";
}

Configuration::Configuration()
    : _iniFilePath(""),
      _isBypassed(true),
      _presets(),
      _currentState(),
      _lastPresetName(""),
      _currentStateIsPreset(false)
{
}

inline void Configuration::Init(const QString& iniFilePath)
{
    _iniFilePath = iniFilePath;
    LoadFromIniFile();
}

inline bool Configuration::IsBypassed() const
{
    return _isBypassed;
}

inline bool Configuration::CurrentStateIsPreset() const
{
    return _currentStateIsPreset;
}

inline const QString& Configuration::LastPresetName() const
{
    return _lastPresetName;
}

inline const ControlState& Configuration::CurrentState() const
{
    return _currentState;
}

inline QList<QString> Configuration::PresetNames() const
{
    return _presets.uniqueKeys();
}

////////////////////////////////////////////////////////////////////////////////

inline void Configuration::SetBypassed(
        bool value,
        const void* originatingController //= NULL
        )
{
    _isBypassed = value;
    Q_EMIT IsBypassedChanged(originatingController);
}

inline void Configuration::SelectPreset(
        const QString& name,
        const void* originatingController //= NULL
        )
{
    if(name == _currentStateName || !_presets.contains(name))
    {
        return;
    }

    _lastPresetName = name;
    _currentState = _presets[_lastPresetName];
    _currentStateIsPreset = true;

    Q_EMIT PresetSelectionChanged(originatingController);
}

bool Configuration::SaveCurrentControlStateAsPreset(
        const QString& name,
        bool overwriteExisting, //= false
        const void* originatingController //= NULL
        )
{
    if(!overwriteExisting && _presets.contains(name))
    {
        return false;
    }

    _lastPresetName = name;
    _presets[_lastPresetName] = _currentState;
    _currentStateIsPreset = true;

    Q_EMIT PresetAdded(_lastPresetName, originatingController);

    return true;
}

bool Configuration::DeleteCurrentPreset(
        const void* originatingController //= NULL
        )
{
    if(!CurrentStateIsPreset())
    {
        return false;
    }

    QString presetName = _lastPresetName;
    _currentStateIsPreset = false;
    int removedCount = _presets.remove(presetName);

    Q_ASSERT(removedCount == 1);

    Q_EMIT PresetDeleted(presetName, originatingController);

    return true;
}

void Configuration::SetCutMode(
        int value,
        const void* originatingController //= NULL
        )
{
    _currentState.CutModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(originatingController);
    UpdateCurrentStateIsPreset(originatingController);
}

void Configuration::SetFrequency(
        int value,
        const void* originatingController //= NULL
        )
{
    _currentState.FrequencyValue() = value;
    Q_EMIT CurrentStatePropertyChanged(originatingController);
    UpdateCurrentStateIsPreset(originatingController);
}

void Configuration::SetCenterManipulationMode(
        ControlState::CenterManipulationMode value,
        const void* originatingController //= NULL
        )
{
    _currentState.CenterManipulationModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(originatingController);
    UpdateCurrentStateIsPreset(originatingController);
}

void Configuration::SetBalance(
        int value,
        const void* originatingController //= NULL
        )
{
    _currentState.BalanceValue() = value;
    Q_EMIT CurrentStatePropertyChanged(originatingController);
    UpdateCurrentStateIsPreset(originatingController);
}

void Configuration::SetBalanceMode(
        ControlState::BalanceMode value,
        const void* originatingController //= NULL
        )
{
    _currentState.BalanceModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(originatingController);
    UpdateCurrentStateIsPreset(originatingController);
}

void Configuration::LoadFromIniFile()
{
    QSettings iniSettings(_iniFilePath, QSettings::IniFormat);
}

void Configuration::SaveToIniFile()
{
    QSettings iniSettings(_iniFilePath, QSettings::IniFormat);
    iniSettings.setValue(kBypassedName, _isBypassed);
}

void Configuration::SaveStateToIniFile(const ControlState& state,
                                       QSettings& ini)
{
    ini.setValue(kBalanceModeName, (int)state.BalanceModeValue());
    ini.setValue(kBalanceName, state.BalanceValue());
    ini.setValue();
    ini.setValue();
    iniSettings.setValue();
}ini

ControlState Configuration::LoadStateFromIniFile(const QSettings& ini)
{

}

void Configuration::UpdateCurrentStateIsPreset(
        const void* originatingController
        )
{
    bool signalIsNeeded = false;

    bool currentStateIsPreset = _presets.values().contains(_currentState);
    signalIsNeeded = (currentStateIsPreset != _currentStateIsPreset);
    _currentStateIsPreset = currentStateIsPreset;

    if(_currentStateIsPreset)
    {
        if(_currentState != _presets[_lastPresetName])
        {
            _lastPresetName = _presets.key(_currentState);
            signalIsNeeded = true;
        }
    }

    if(signalIsNeeded)
    {
        Q_EMIT PresetSelectionChanged(originatingController);
    }
}
