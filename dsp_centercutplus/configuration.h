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
    const QString& LastPresetName() const;
    const ControlState& CurrentState() const;
    QList<QString> PresetNames() const;

    void SetBypassed(bool value, const void* controller = NULL);
    void SelectPreset(const QString& name, const void* controller = NULL);
    bool SaveCurrentControlStateAsPreset(const QString& name,
                                         bool overwriteExisting = false,
                                         const void* controller = NULL);
    bool DeleteCurrentPreset(const void* controller = NULL);

    void SetCutMode(int value, const void* controller = NULL);
    void SetFrequency(int value, const void* controller = NULL);
    void SetCenterManipulationMode(ControlState::CenterManipulationMode value,
                                   const void* controller = NULL);
    void SetBalance(int value, const void* controller = NULL);
    void SetBalanceMode(ControlState::BalanceMode value,
                        const void* controller = NULL);

Q_SIGNALS:
    void IsBypassedChanged(const void* controller);
    void PresetAdded(QString name, const void* controller);
    void PresetDeleted(QString name, const void* controller);
    void PresetSelectionChanged(const void* controller);
    void CurrentStatePropertyChanged(const void* controller);

private:
    typedef QHash<QString, ControlState> PresetCollection;

    void UpdateCurrentStateIsPreset(const void* originatingController);

    bool _isBypassed;
    PresetCollection _presets;
    ControlState _currentState;
    QString _lastPresetName;
    bool _currentStateIsPreset;

    Q_OBJECT
    DISALLOW_COPY_AND_ASSIGN(Configuration);
};

#endif // CONFIGURATION_H
