#ifndef DSPSETTINGS_H
#define DSPSETTINGS_H

class DSPSettings
{
public:
    enum CenterManipulationMode {CMM_OFF, CMM_HIGHTOSIDES, CMM_LOWTOSIDES};
    enum BalanceMode {BM_CUTTER, BM_FILLER, BM_MONO};

    DSPSettings();
    DSPSettings(const DSPSettings& from);
    DSPSettings(int cutModValuee, int frequencyValue,
                int centerManipulationMode,
                int balanceValue, int balanceMode);
    bool operator==(const DSPSettings& to) const;
    void operator=(const DSPSettings& from);

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

#endif // DSPSETTINGS_H
