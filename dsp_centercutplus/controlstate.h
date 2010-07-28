#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

#include <QSettings>

class ControlState
{
public:
    enum CenterManipulationMode {CMM_OFF, CMM_HIGHTOSIDES, CMM_LOWTOSIDES};
    enum BalanceMode {BM_CUTTER, BM_FILLER, BM_MONO};

    ControlState();
    ControlState(const ControlState& from);
    ControlState(int cutModeValue, int frequencyValue,
                 CenterManipulationMode centerManipulationMode,
                 int balanceValue, BalanceMode balanceMode);
    bool operator==(const ControlState& to) const;
    bool operator!=(const ControlState& to) const;
    ControlState& operator=(const ControlState& from);

    int CutModeValue() const;
    int FrequencyValue() const;
    CenterManipulationMode CenterManipulationModeValue() const;
    int BalanceValue() const;
    BalanceMode BalanceModeValue() const;

    void CutModeValue(int value);
    void FrequencyValue(int value);
    void CenterManipulationModeValue(CenterManipulationMode value);
    void BalanceValue(int value);
    void BalanceModeValue(BalanceMode value);

    void SaveToIniFile(QSettings& ini);
    bool LoadFromIniFile(const QSettings& ini);

private:
    BalanceMode _balanceModeValue;
    int _balanceValue;
    CenterManipulationMode _centerManipulationModeValue;
    int _cutModeValue;
    int _frequencyValue;
};

#endif // CONTROLSTATE_H
