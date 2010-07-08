#include "configuration.h"

#include <QList>

inline bool Configuration::IsBypassed() const
{
    return _isBypassed;
}

inline bool Configuration::CurrentStateIsPreset() const
{
    return _currentStateName !=NULL;
}

inline const QString& Configuration::CurrentStateName() const
{
    static QString emptyName("");
    return CurrentStateIsPreset() ? *_currentStateName : emptyName;
}

inline const ControlState& Configuration::CurrentState() const
{
    return _currentState;
}

inline QList<QString> Configuration::PresetNames() const
{
    return _presets.uniqueKeys();
}

bool Configuration::SaveCurrentControlStateAsPreset(
        const QString& name,
        bool overwriteExisting //= false
        )
{
    if(!overwriteExisting && _presets.contains(name))
    {
        return false;
    }

    // TODO: use LoadPreset() here
    _currentStateName = &(_presets.insert(name, _currentState).key());
    return true;
}
