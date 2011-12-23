#include "../inc/centercutplus/core/engineconfig.h"

namespace ccp
{
namespace core
{

const int EngineConfig::kMinPercent = -100;
const int EngineConfig::kMaxPercent = 100;

namespace
{
    int SanitizePercentValue(int percentValue)
    {
        return percentValue > EngineConfig::kMaxPercent
                   ? EngineConfig::kMaxPercent
                   : percentValue < EngineConfig::kMinPercent
                         ? EngineConfig::kMinPercent
                         : percentValue;
    }

    CenterDetectionMode SanitizeCenterDetectionModeValue(CenterDetectionMode value)
    {
        return value > kCenterDetectionMode_Max || value < kCenterDetectionMode_Min
                   ? kCenterDetectionMode_Min
                   : value;
    }
}

EngineConfig::EngineConfig()
    : _leftToLeftPercent(0),
      _centerToLeftPercent(0),
      _rightToLeftPercent(0),
      _leftToRightPercent(0),
      _centerToRightPercent(0),
      _rightToRightPercent(0),
      _centerDetectionMode(kCenterDetectionMode_InPhase),
      _centerToSidesFrequencyIntervals()
{
}

EngineConfig::~EngineConfig()
{
}

CenterDetectionMode EngineConfig::centerDetectionMode() const
{
    return _centerDetectionMode;
}

const EngineConfig::FrequencyIntervalVector& EngineConfig::centerToSidesFrequencyIntervals() const
{
    return _centerToSidesFrequencyIntervals;
}

int EngineConfig::leftToLeftPercent() const
{
    return _leftToLeftPercent;
}

int EngineConfig::centerToLeftPercent() const
{
    return _centerToLeftPercent;
}

int EngineConfig::rightToLeftPercent() const
{
    return _rightToLeftPercent;
}

int EngineConfig::leftToRightPercent() const
{
    return _leftToRightPercent;
}

int EngineConfig::centerToRightPercent() const
{
    return _centerToRightPercent;
}

int EngineConfig::rightToRightPercent() const
{
    return _rightToRightPercent;
}

void EngineConfig::centerDetectionMode(CenterDetectionMode value)
{
    _centerDetectionMode = SanitizeCenterDetectionModeValue(value);
}

EngineConfig::FrequencyIntervalVector& EngineConfig::centerToSidesFrequencyIntervals()
{
    return _centerToSidesFrequencyIntervals;
}

void EngineConfig::leftToLeftPercent(int value)
{
    _leftToLeftPercent = SanitizePercentValue(value);
}

void EngineConfig::centerToLeftPercent(int value)
{
    _centerToLeftPercent = SanitizePercentValue(value);
}

void EngineConfig::rightToLeftPercent(int value)
{
    _rightToLeftPercent = SanitizePercentValue(value);
}

void EngineConfig::leftToRightPercent(int value)
{
    _leftToRightPercent = SanitizePercentValue(value);
}

void EngineConfig::centerToRightPercent(int value)
{
    _centerToRightPercent = SanitizePercentValue(value);
}

void EngineConfig::rightToRightPercent(int value)
{
    _rightToRightPercent = SanitizePercentValue(value);
}

bool EngineConfig::operator==(const EngineConfig& other) const
{
    if(this == &other)
    {
        return true;
    }

    if(_centerDetectionMode != other._centerDetectionMode ||
        _leftToLeftPercent != other._leftToLeftPercent ||
        _centerToLeftPercent != other._centerToLeftPercent ||
        _rightToLeftPercent != other._rightToLeftPercent ||
        _leftToRightPercent != other._leftToRightPercent ||
        _centerToRightPercent != other._centerToRightPercent ||
        _rightToRightPercent != other._rightToRightPercent)
    {
        return false;
    }

    if(_centerToSidesFrequencyIntervals.size() != other._centerToSidesFrequencyIntervals.size())
    {
        return false;
    }

    for(auto iterator = _centerToSidesFrequencyIntervals.begin();
        iterator < _centerToSidesFrequencyIntervals.end();
        ++iterator)
    {
        auto otherIterator = other._centerToSidesFrequencyIntervals.begin();
        for(;
            otherIterator < other._centerToSidesFrequencyIntervals.end();
            ++otherIterator)
        {
            if(*iterator == *otherIterator)
            {
                break;
            }
        }

        if(otherIterator >= other._centerToSidesFrequencyIntervals.end())
        {
            return false;
        }
    }

    return true;
}

bool EngineConfig::operator!=(const EngineConfig& other) const
{
    return !operator==(other);
}

}
}
