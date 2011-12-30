#pragma once

#include <boost/utility.hpp>
#include <centercutplus/common/types.h>

namespace ccp
{
namespace core
{

///
/// Describes a fequency interval in Hz
/// All setter methods do sanity checks (kMin/MaxFrequency, min <= max) and silently override value if needed
///
class FrequencyInterval : boost::noncopyable
{
    // consts
  public:
    static const uint kMinFrequency;
    static const uint kMaxFrequency;

    // ctor
  public:
    FrequencyInterval();
    virtual ~FrequencyInterval();

    // methods
  public:
    uint minimum() const;
    uint maximum() const;

    void minimum(uint value);
    void maximum(uint value);

    bool IsInInterval(uint value);

   /* bool operator== (const FrequencyInterval& other) const;
    bool operator!= (const FrequencyInterval& other) const;*/

    // data
  private:
    uint _min;
    uint _max;
};

}
}
