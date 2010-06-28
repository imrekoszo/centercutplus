#include "dspsettings.h"

DSPSettings::DSPSettings()
{
}

DSPSettings::DSPSettings(const DSPSettings& from)
    : _balanceMode(from._balanceMode),
    _balanceValue(from._balanceValue),
    _centerManipulationMode(from._centerManipulationMode),
    _cutModeValue(from._cutModeValue),
    _frequencyValue(from._frequencyValue)
{
}

DSPSettings::DSPSettings(int cutModeValue, int frequencyValue,
                         int centerManipulationMode,
                         int balanceValue, int balanceMode)
    : _balanceMode(balanceMode),
    _balanceValue(balanceValue),
    _centerManipulationMode(centerManipulationMode),
    _cutModeValue(cutModeValue),
    _frequencyValue(frequencyValue)
{
}

bool DSPSettings::operator==(const DSPSettings& to) const
{
    return _balanceMode == to._balanceMode
            && _balanceValue == to._balanceValue
            && _centerManipulationMode == to._centerManipulationMode
            && _cutModeValue == to._cutModeValue
            && _frequencyValue == to._frequencyValue;
}

void DSPSettings::operator=(const DSPSettings& from)
{
    _balanceMode = from._balanceMode;
    _balanceValue = from._balanceValue;
    _centerManipulationMode = from._centerManipulationMode;
    _cutModeValue = from._cutModeValue;
    _frequencyValue = from._frequencyValue;
}
