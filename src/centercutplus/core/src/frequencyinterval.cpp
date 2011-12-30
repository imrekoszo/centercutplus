#include "../inc/centercutplus/core/frequencyinterval.h"

namespace ccp
{
namespace core
{

const uint FrequencyInterval::kMinFrequency = 20u;
const uint FrequencyInterval::kMaxFrequency = 20000u;

FrequencyInterval::FrequencyInterval()
    : _min(kMinFrequency),
      _max(kMaxFrequency)
{
}

FrequencyInterval::~FrequencyInterval()
{
}

uint FrequencyInterval::minimum() const
{
    return _min;
}

uint FrequencyInterval::maximum() const
{
    return _max;
}

void FrequencyInterval::minimum(uint value)
{
    if(value < kMinFrequency)
    {
        _min = kMinFrequency;
    }
    else if(value > _max)
    {
        _min = _max;
    }
    else
    {
        _min = value;
    }
}

void FrequencyInterval::maximum(uint value)
{
    if(value > kMaxFrequency)
    {
        _max = kMaxFrequency;
    }
    else if(value < _min)
    {
        _max = _min;
    }
    else
    {
        _max = value;
    }
}

bool FrequencyInterval::IsInInterval(uint value)
{
    return value >= _min && value <= _max;
}

//bool FrequencyInterval::operator== (const FrequencyInterval& other) const
//{
//    return _min == other._min && _max == other._max;
//}
//
//bool FrequencyInterval::operator!= (const FrequencyInterval& other) const
//{
//    return !operator== (other);
//}

}
}
