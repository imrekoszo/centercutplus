#include "configuration.h"

Configuration::Configuration()
{
}

bool Configuration::SaveCurrentStateAsPreset(const QString& name,
                                             bool overwriteExisting //= false
                                             )
{
    if(!overwriteExisting && _presets.contains(name))
    {
        return false;
    }

    _currentStateName = &(_presets.insert(name, _currentState).key());
}
