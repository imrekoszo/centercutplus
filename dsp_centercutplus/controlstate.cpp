#include "controlstate.h"

#include <QSettings>

namespace
{
    const char* kCutModeName = "cutMode";
    const char* kFrequencyName = "frequency";
    const char* kCenterManipulationModeName = "centerManipulation";
    const char* kBalanceName = "balance";
    const char* kBalanceModeName = "balanceMode";
}

ControlState::ControlState()
    : _balanceModeValue(BM_FILLER),
    _balanceValue(0),
    _centerManipulationModeValue(CMM_OFF),
    _cutModeValue(0),
    _frequencyValue()       // TODO: get starting frequency slider value!
{
}

ControlState::ControlState(const ControlState& from)
    : _balanceModeValue(from._balanceModeValue),
    _balanceValue(from._balanceValue),
    _centerManipulationModeValue(from._centerManipulationModeValue),
    _cutModeValue(from._cutModeValue),
    _frequencyValue(from._frequencyValue)
{
}

ControlState::ControlState(int cutModeValue, int frequencyValue,
                           CenterManipulationMode centerManipulationModeValue,
                           int balanceValue, BalanceMode balanceModeValue)
    : _balanceModeValue(balanceModeValue),
    _balanceValue(balanceValue),
    _centerManipulationModeValue(centerManipulationModeValue),
    _cutModeValue(cutModeValue),
    _frequencyValue(frequencyValue)
{
}

bool ControlState::operator==(const ControlState& to) const
{
    return !(this->operator !=(to));
}

bool ControlState::operator!=(const ControlState& to) const
{
    return  this != &to &&
            (_balanceModeValue != to._balanceModeValue
            || _balanceValue != to._balanceValue
            || _centerManipulationModeValue != to._centerManipulationModeValue
            || _cutModeValue != to._cutModeValue
            || _frequencyValue != to._frequencyValue);
}

ControlState& ControlState::operator=(const ControlState& from)
{
    if(this != &from) // Gracefully handle self assignment
    {
        _balanceModeValue = from._balanceModeValue;
        _balanceValue = from._balanceValue;
        _centerManipulationModeValue = from._centerManipulationModeValue;
        _cutModeValue = from._cutModeValue;
        _frequencyValue = from._frequencyValue;
    }
    return *this;
}

inline int ControlState::CutModeValue() const
{
    return _cutModeValue;
}

inline int ControlState::FrequencyValue() const
{
    return _frequencyValue;
}

inline ControlState::CenterManipulationMode
        ControlState::CenterManipulationModeValue() const
{
    return _centerManipulationModeValue;
}

inline int ControlState::BalanceValue() const
{
    return _balanceValue;
}

inline ControlState::BalanceMode ControlState::BalanceModeValue() const
{
    return _balanceModeValue;
}

inline void ControlState::CutModeValue(int value)
{
    // TODO: range checking
    _cutModeValue = value;
}

inline void ControlState::FrequencyValue(int value)
{
    // TODO: range checking
    _frequencyValue = value;
}

inline void ControlState::CenterManipulationModeValue(
        CenterManipulationMode value)
{
    if(value == CMM_HIGHTOSIDES || value == CMM_LOWTOSIDES || value == CMM_OFF)
    {
        _centerManipulationModeValue = value;
    }
}

inline void ControlState::BalanceValue(int value)
{
    // TODO: range checking
    _balanceValue = value;
}

inline void ControlState::BalanceModeValue(BalanceMode value)
{
    if(value == BM_CUTTER || value == BM_FILLER || value == BM_MONO)
    {
        _balanceModeValue = value;
    }
}

void ControlState::SaveToIniFile(QSettings& ini)
{
    ini.setValue(kBalanceModeName, static_cast<int>(_balanceModeValue));
    ini.setValue(kBalanceName, _balanceValue);
    ini.setValue(kCenterManipulationModeName,
                 static_cast<int>(_centerManipulationModeValue));
    ini.setValue(kCutModeName, _cutModeValue);
    ini.setValue(kFrequencyName, _frequencyValue);
}

bool ControlState::LoadFromIniFile(const QSettings& ini)
{
    int balmode, cmmode, cutmode, bal, freq;
    bool allok = true;
    bool ok = true;

    balmode = ini.value(kBalanceModeName).toInt(&ok);
    allok &= ok;
    ini.value(kBalanceName).toInt(&ok);
    allok &= ok;
    ini.value(kCenterManipulationModeName).toInt(&ok);
    allok &= ok;
    ini.value(kCutModeName).toInt(&ok);
    allok &= ok;
    ini.value(kFrequencyName).toInt(&ok);
    allok &= ok;

    if(!allok)
    {
        return false;
    }

    // TODO: range checking return false?

    _cutModeValue = cutmode;
    _frequencyValue = freq;
    _centerManipulationModeValue = static_cast<CenterManipulationMode>(cmmode);
    _balanceValue = bal;
    _balanceModeValue = static_cast<BalanceMode>(balmode);

    return true;
}
