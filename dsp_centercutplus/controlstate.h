#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

class ControlState
{
public:
    enum CenterManipulationMode {CMM_OFF, CMM_HIGHTOSIDES, CMM_LOWTOSIDES};
    enum BalanceMode {BM_CUTTER, BM_FILLER, BM_MONO};

    ControlState();
    ControlState(const ControlState& from);
    ControlState(int cutModeValue, int frequencyValue,
                 int centerManipulationMode,
                 int balanceValue, int balanceMode);
    bool operator==(const ControlState& to) const;
    ControlState& operator=(const ControlState& from);

    int CutModeValue() const { return _cutModeValue; }
    int& CutModeValue() { return _cutModeValue; }
    int FrequencyValue() const { return _frequencyValue; }
    int& FrequencyValue() const { return _frequencyValue; }
    CenterManipulationMode CenterManipulationModeValue() const
    {
        return _centerManipulationModeValue;
    }
    CenterManipulationMode& CenterManipulationModeValue() const
    {
        return _centerManipulationModeValue;
    }
    int BalanceValue() const { return _balanceValue; }
    int& BalanceValue() const { return _balanceValue; }
    BalanceMode BalanceModeValue() const { return _balanceModValuee; }
    BalanceMode& BalanceModeValue() const { return _balanceModeValue; }

private:
    int _cutModeValue;
    int _frequencyValue;
    CenterManipulationMode _centerManipulationModeValue;
    int _balanceValue;
    BalanceMode _balanceModeValue;
};

#endif // CONTROLSTATE_H
