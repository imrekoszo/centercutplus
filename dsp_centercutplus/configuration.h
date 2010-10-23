#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// library headers
#include <QHash>
#include <QList>
#include <QPair>
#include <QSettings>

// local headers
#include "classhelpers.h"
#include "controlstate.h"

class Configuration : QObject
{
public:
    Configuration();

    void Load(const QString& iniFilePath);
    void Save();

    bool IsBypassed() const;
    bool CurrentStateIsPreset() const;
    const QString& LastPresetName() const;
    const ControlState& CurrentState() const;
    QList<QString> PresetNames() const;

    void SetBypassed(
            bool value,
            const void* originatingController = NULL);
    void SelectPreset(
            const QString& name,
            const void* originatingController = NULL);
    bool SaveCurrentControlStateAsPreset(
            const QString& name,
            bool overwriteExisting = false,
            const void* originatingController = NULL);
    bool DeleteCurrentPreset(
            const void* originatingController = NULL);

    void SetCutMode(
            int value,
            const void* originatingController = NULL);
    void SetFrequency(
            int value,
            const void* originatingController = NULL);
    void SetCenterManipulationMode(
            ControlState::CenterManipulationMode value,
            const void* originatingController = NULL);
    void SetBalance(
            int value,
            const void* originatingController = NULL);
    void SetBalanceMode(
            ControlState::BalanceMode value,
            const void* originatingController = NULL);

Q_SIGNALS:
    void IsBypassedChanged(const void* originatingController);
    void PresetAdded(QString name, const void* originatingController);
    void PresetDeleted(QString name, const void* originatingController);
    void PresetSelectionChanged(const void* originatingController);
    void CurrentStatePropertyChanged(const void* originatingController);

private:
    typedef QHash<QString, ControlState> PresetCollection;

    void LoadFromIniFile();
    void SaveToIniFile();

    void UpdateCurrentStateIsPreset(const void* originatingController);

    QString _iniFilePath;
    bool _isBypassed;
    PresetCollection _presets;
    ControlState _currentState;
    QString _lastPresetName;
    bool _currentStateIsPreset;

    Q_OBJECT
    DISALLOW_COPY_AND_ASSIGN(Configuration);
};

#endif // CONFIGURATION_H
