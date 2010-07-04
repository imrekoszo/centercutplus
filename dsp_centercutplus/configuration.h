#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// library headers
#include <QHash>
#include <QPair>

// local headers
#include "classhelpers.h"
#include "dspsettings.h"

class Configuration
{
public:
    bool IsBypassed() const { return _isBypassed; }
    const QString& CurrentSettingsName() const
    {
        static QString emptyName("");
        return _currentSettingsName != NULL ? *_currentSettingsName : emptyName;
    }
    bool CurrentSettingsIsStored() const { return _currentSettingsName !=NULL; }
    const DSPSettings& CurrentSettings() const { return _currentSettings; }

private:
    typedef QPair<DSPSettings, QString> namedsettings_type;

    bool _isBypassed;
    QHash<QString, namedsettings_type> _settings;
    DSPSettings _currentSettings;
    QString* _currentSettingsName;

    DISALLOW_COPY_AND_ASSIGN(Configuration);
};

#endif // CONFIGURATION_H
