#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "dspsettings.h"
#include <QHash>
#include <QPair>

class Configuration
{
    bool _isBypassed;

    QHash<QString, QPair<DSPSettings, QString> > _presets;
    DSPSettings _currentSettings;
    QString* _currentSettingsName;

    Configuration(const Configuration&) { }

public:
    bool IsBypassed() const { return _isBypassed; }
    QString CurrentSettingsName() const { return _currentSettingsName != NULL
                                                    ? *_currentSettingsName : ""; }
    bool CurrentSettingsIsStored() const { return _currentSettingsName != NULL; }
    DSPSettings CurrentSettings() const { return _currentSettings; }

};

#endif // CONFIGURATION_H
