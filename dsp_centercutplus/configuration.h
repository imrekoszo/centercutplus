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

    void SetBypassed(bool value, const void* controller = NULL);
    bool SaveCurrentControlStateAsPreset(const QString& name,
                                         bool overwriteExisting = false,
                                         const void* controller = NULL);
    bool DeleteCurrentPreset(const void* controller = NULL);
    void SelectPreset(const QString& name, const void* controller = NULL);

    void SetCutMode(int value, const void* controller = NULL);
    void SetFrequency(int value, const void* controller = NULL);
    void SetCenterManipulationMode(ControlState::CenterManipulationMode value,
                                   const void* controller = NULL);
    void SetBalance(int value, const void* controller = NULL);
    void SetBalanceMode(ControlState::BalanceMode value,
                        const void* controller = NULL);

Q_SIGNALS:
    void IsBypassedChanged(bool value, const void* controller = NULL);
    void NewPresetAdded(const QString& name, const void* controller = NULL);
    void PresetDeleted(const QString& name, const void* controller = NULL);
    void PresetSelectionChanged(const QString& name,
                                const void* controller = NULL);
    void CutModeValueChanged(int value, const void* controller);
    void FrequencyValueChanged(int value, const void* controller = NULL);
    void CenterManipulationModeValueChanged(
            ControlState::CenterManipulationMode value,
            const void* controller = NULL);
    void BalanceValueChanged(int value, const void* controller = NULL);
    void BalanceModeValueChanged(ControlState::BalanceMode value,
                                 const void* controller = NULL);

private:
    typedef QHash<QString, ControlState> PresetCollection;

    bool _isBypassed;
    PresetCollection _presets;
    ControlState _currentState;
    QString* _currentStateName;

    DISALLOW_COPY_AND_ASSIGN(Configuration);
    Q_OBJECT
};

#endif // CONFIGURATION_H
