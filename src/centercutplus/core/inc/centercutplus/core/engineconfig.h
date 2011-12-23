#pragma once

#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <centercutplus/common/types.h>

#include "centerdetectionmode.h"
#include "frequencyinterval.h"

namespace ccp
{
namespace core
{

///
/// Describes all settings that affect signal processing when the engine is not bypassed.
/// All setter methods do sanity checks (kMin/MaxPercent) and silently override value if needed
///
class EngineConfig : boost::noncopyable
{
    // typedefs
  public:
    typedef boost::ptr_vector<FrequencyInterval> FrequencyIntervalVector;

    // consts
  public:
    static const int kMinPercent;
    static const int kMaxPercent;

    // ctor
  public:
    EngineConfig();
    virtual ~EngineConfig();

    // methods
  public:
    CenterDetectionMode centerDetectionMode() const;
    const FrequencyIntervalVector& centerToSidesFrequencyIntervals() const;
    int leftToLeftPercent() const;
    int centerToLeftPercent() const;
    int rightToLeftPercent() const;
    int leftToRightPercent() const;
    int centerToRightPercent() const;
    int rightToRightPercent() const;

    void centerDetectionMode(CenterDetectionMode value);
    FrequencyIntervalVector& centerToSidesFrequencyIntervals();
    void leftToLeftPercent(int value);
    void centerToLeftPercent(int value);
    void rightToLeftPercent(int value);
    void leftToRightPercent(int value);
    void centerToRightPercent(int value);
    void rightToRightPercent(int value);

    // can be expensive, use with care
    bool operator==(const EngineConfig& other) const;

    // can be expensive, use with care
    bool operator!=(const EngineConfig& other) const;

    // data
  private:
    CenterDetectionMode _centerDetectionMode;
    FrequencyIntervalVector _centerToSidesFrequencyIntervals;
    int _leftToLeftPercent;
    int _centerToLeftPercent;
    int _rightToLeftPercent;
    int _leftToRightPercent;
    int _centerToRightPercent;
    int _rightToRightPercent;
};

}
}
