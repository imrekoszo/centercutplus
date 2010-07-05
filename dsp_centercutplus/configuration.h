#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// library headers
#include <QHash>
#include <QList>
#include <QPair>

// local headers
#include "classhelpers.h"
#include "controlstate.h"

class Configuration
{
public:
    bool IsBypassed() const { return _isBypassed; }

    bool CurrentStateIsPreset() const { return _currentStateName !=NULL; }
    const QString& CurrentStateName() const
    {
        static QString emptyName("");
        return CurrentStateIsPreset() ? *_currentStateName : emptyName;
    }
    const ControlState& CurrentState() const { return _currentState; }

    QList<QString> PresetNames() const { return _presets.uniqueKeys(); }

    bool SaveCurrentStateAsPreset(const QString& name,
                                  bool overwriteExisting = false);

private:
    typedef QHash<QString, ControlState> PresetCollection;

    bool _isBypassed;
    PresetCollection _presets;
    ControlState _currentState;
    QString* _currentStateName;

    DISALLOW_COPY_AND_ASSIGN(Configuration);
};

#endif // CONFIGURATION_H
