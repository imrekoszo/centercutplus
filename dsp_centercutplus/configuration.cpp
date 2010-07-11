#include "configuration.h"

#include <QList>

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

inline void Configuration::SetBypassed(bool value,
                                       const void* controller //= NULL
                                       )
{
    _isBypassed = value;
    Q_EMIT IsBypassedChanged(controller);
}

inline void Configuration::SelectPreset(const QString& name,
                                        const void* controller //= NULL
                                        )
{
    if(name == _currentStateName || !_presets.contains(name))
    {
        return;
    }

    _lastPresetName = name;
    _currentState = _presets[_lastPresetName];
    _currentStateIsPreset = true;

    Q_EMIT PresetSelectionChanged(controller);
}

bool Configuration::SaveCurrentControlStateAsPreset(
        const QString& name,
        bool overwriteExisting, //= false
        const void* controller //= NULL
        )
{
    if(!overwriteExisting && _presets.contains(name))
    {
        return false;
    }

    _lastPresetName = name;
    _presets[_lastPresetName] = _currentState;
    _currentStateIsPreset = true;

    Q_EMIT PresetAdded(_lastPresetName, controller);

    return true;
}

bool Configuration::DeleteCurrentPreset(const void* controller //= NULL
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

    Q_EMIT PresetDeleted(presetName, controller);

    return true;
}

void Configuration::SetCutMode(int value, const void* controller //= NULL
                               )
{
    _currentState.CutModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(controller);
    UpdateCurrentStateIsPreset(controller);
}

void Configuration::SetFrequency(int value, const void* controller //= NULL
                                 )
{
    _currentState.FrequencyValue() = value;
    Q_EMIT CurrentStatePropertyChanged(controller);
    UpdateCurrentStateIsPreset(controller);
}
void Configuration::SetCenterManipulationMode(
        ControlState::CenterManipulationMode value,
        const void* controller //= NULL
        )
{
    _currentState.CenterManipulationModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(controller);
    UpdateCurrentStateIsPreset(controller);
}
void Configuration::SetBalance(int value, const void* controller //= NULL
                               )
{
    _currentState.BalanceValue() = value;
    Q_EMIT CurrentStatePropertyChanged(controller);
    UpdateCurrentStateIsPreset(controller);
}
void Configuration::SetBalanceMode(ControlState::BalanceMode value,
                                   const void* controller //= NULL
                                   )
{
    _currentState.BalanceModeValue() = value;
    Q_EMIT CurrentStatePropertyChanged(controller);
    UpdateCurrentStateIsPreset(controller);
}

void Configuration::UpdateCurrentStateIsPreset(
        const void* originatingController)
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
