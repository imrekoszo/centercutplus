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
    bool IsBypassed() const;
    bool CurrentStateIsPreset() const;
    const QString& CurrentStateName() const;
    const ControlState& CurrentState() const;
    QList<QString> PresetNames() const;

    bool SaveCurrentControlStateAsPreset(const QString& name,
                                         bool overwriteExisting = false);
    void LoadPreset(const QString& name);
    void SetCutMode(int value, const IController* sender);
    void SetFrequency(int value, const IController* sender);
    void SetCenterManipulationMode(ControlState::CenterManipulationMode value,
                                   const IController* sender);
    void SetBalance(int value, const IController* sender);
    void SetBalanceMode(ControlState::BalanceMode value,
                        const IController* sender);

Q_SIGNALS:
    void CutModeValueChanged(int value, const IController* sender);


private:
    typedef QHash<QString, ControlState> PresetCollection;

    bool _isBypassed;
    PresetCollection _presets;
    ControlState _currentState;
    QString* _currentStateName;

    DISALLOW_COPY_AND_ASSIGN(Configuration);
};

#endif // CONFIGURATION_H
