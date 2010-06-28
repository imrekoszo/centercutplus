#ifndef DSPSETTINGS_H
#define DSPSETTINGS_H

class DSPSettings
{
    int _cutModeValue;

    int _frequencyValue;
    int _centerManipulationMode;

    int _balanceValue;
    int _balanceMode;

public:
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
    int CenterManipulationMode() const { return _centerManipulationMode; }
    int& CenterManipulationMode() const { return _centerManipulationMode; }
    int BalanceValue() const { return _balanceValue; }
    int& BalanceValue() const { return _balanceValue; }
    int BalanceMode() const { return _balanceMode; }
    int& BalanceMode() const { return _balanceMode; }
public:
    enum {CMM_OFF, CMM_HIGHTOSIDES, CMM_LOWTOSIDES};
    enum {BM_CUTTER, BM_FILLER, BM_MONO};
};

#endif // DSPSETTINGS_H
