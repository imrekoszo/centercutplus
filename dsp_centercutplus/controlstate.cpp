#include "controlstate.h"

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
                           int centerManipulationModeValue,
                           int balanceValue, int balanceModeValue)
    : _balanceModeValue(balanceModeValue),
    _balanceValue(balanceValue),
    _centerManipulationModeValue(centerManipulationModeValue),
    _cutModeValue(cutModeValue),
    _frequencyValue(frequencyValue)
{
}

bool ControlState::operator==(const ControlState& to) const
{
    return  this == &to ||
            (_balanceModeValue == to._balanceModeValue
            && _balanceValue == to._balanceValue
            && _centerManipulationModeValue == to._centerManipulationModeValue
            && _cutModeValue == to._cutModeValue
            && _frequencyValue == to._frequencyValue);
}

ControlState& ControlState::operator=(const ControlState& from)
{
    if(this != &to) // Gracefully handle self assignment
    {
        _balanceModeValue = from._balanceModeValue;
        _balanceValue = from._balanceValue;
        _centerManipulationModeValue = from._centerManipulationModeValue;
        _cutModeValue = from._cutModeValue;
        _frequencyValue = from._frequencyValue;
    }
    return *this;
}
