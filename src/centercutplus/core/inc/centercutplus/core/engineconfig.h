#pragma once

#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <centercutplus/common/types.h>

#include "centerdetectionmode.h"
#include "frequencyinterval.h"
#include "types.h"

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
    int focusPositionPercent() const;

    EngineConfig& centerDetectionMode(CenterDetectionMode value);
    FrequencyIntervalVector& centerToSidesFrequencyIntervals();
    EngineConfig& leftToLeftPercent(int value);
    EngineConfig& centerToLeftPercent(int value);
    EngineConfig& rightToLeftPercent(int value);
    EngineConfig& leftToRightPercent(int value);
    EngineConfig& centerToRightPercent(int value);
    EngineConfig& rightToRightPercent(int value);
    EngineConfig& focusPositionPercent(int value);

    //bool Equals(const EngineConfig& other) const;

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
    int _focusPositionPercent;
};

}
}
