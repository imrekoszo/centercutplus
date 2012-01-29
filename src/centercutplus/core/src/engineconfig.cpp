#include <boost/foreach.hpp>
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
      _centerToSidesFrequencyIntervals(),
      _focusPositionPercent(0)
{
}

EngineConfig::~EngineConfig()
{
}

CenterDetectionMode EngineConfig::centerDetectionMode() const
{
    return _centerDetectionMode;
}

const FrequencyIntervalVector& EngineConfig::centerToSidesFrequencyIntervals() const
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

int EngineConfig::focusPositionPercent() const
{
    return _focusPositionPercent;
}

EngineConfig& EngineConfig::centerDetectionMode(CenterDetectionMode value)
{
    _centerDetectionMode = SanitizeCenterDetectionModeValue(value);
    return *this;
}

FrequencyIntervalVector& EngineConfig::centerToSidesFrequencyIntervals()
{
    return _centerToSidesFrequencyIntervals;
}

EngineConfig& EngineConfig::leftToLeftPercent(int value)
{
    _leftToLeftPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::centerToLeftPercent(int value)
{
    _centerToLeftPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::rightToLeftPercent(int value)
{
    _rightToLeftPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::leftToRightPercent(int value)
{
    _leftToRightPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::centerToRightPercent(int value)
{
    _centerToRightPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::rightToRightPercent(int value)
{
    _rightToRightPercent = SanitizePercentValue(value);
    return *this;
}

EngineConfig& EngineConfig::focusPositionPercent(int value)
{
    _focusPositionPercent = SanitizePercentValue(value);
    return *this;
}

//bool EngineConfig::Equals(const EngineConfig& other) const
//{
//    if(this == &other)
//    {
//        return true;
//    }
//
//    if(_centerDetectionMode != other._centerDetectionMode ||
//        _leftToLeftPercent != other._leftToLeftPercent ||
//        _centerToLeftPercent != other._centerToLeftPercent ||
//        _rightToLeftPercent != other._rightToLeftPercent ||
//        _leftToRightPercent != other._leftToRightPercent ||
//        _centerToRightPercent != other._centerToRightPercent ||
//        _rightToRightPercent != other._rightToRightPercent)
//    {
//        return false;
//    }
//
//    if(_centerToSidesFrequencyIntervals.size() != other._centerToSidesFrequencyIntervals.size())
//    {
//        return false;
//    }
//
//    BOOST_FOREACH(const FrequencyInterval& own, _centerToSidesFrequencyIntervals)
//    {
//        bool found = false;
//
//        BOOST_FOREACH(const FrequencyInterval& others, other._centerToSidesFrequencyIntervals)
//        {
//            if(own == others)
//            {
//                found = true;
//                break;
//            }
//        }
//
//        if(!found)
//        {
//            return false;
//        }
//    }
//
//    return true;
//}

}
}
