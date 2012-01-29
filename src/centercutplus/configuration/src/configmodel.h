#pragma once

#include <set>

#include <centercutplus/common/types.h>
#include <centercutplus/core/centerdetectionmode.h>

#include "../inc/centercutplus/configuration/iconfigviewmodel.h"


namespace ccp
{

namespace core
{
class Config;
class EngineConfig;
}
class IConfigView;

namespace configuration
{

class ConfigModel : public IConfigViewModel
{
    // ctor
  public:
    ConfigModel(core::Config& config);
    virtual ~ConfigModel();

    // methods
  public:
    bool GetBypassed() const;
    const core::EngineConfig& GetCurrentEngineConfig() const;

    void Subscribe(IConfigView& view);
    void Unsubscribe(IConfigView& view);

    void SetBypassed(bool bypass, const void* origin);
    void SetCenterDetectionMode(core::CenterDetectionMode value, const void* origin);
    void SetLeftToLeftPercent(int value, const void* origin);
    void SetCenterToLeftPercent(int value, const void* origin);
    void SetRightToLeftPercent(int value, const void* origin);
    void SetLeftToRightPercent(int value, const void* origin);
    void SetCenterToRightPercent(int value, const void* origin);
    void SetRightToRightPercent(int value, const void* origin);
    void AddFreqInterval(const void* origin);
    void RemoveFreqInterval(size_t index, const void* origin);
    void SetFreqMin(size_t index, uint value, const void* origin);
    void SetFreqMax(size_t index, uint value, const void* origin);

  private:
    void UpdateViews(const void* origin);

    // data
  private:
    core::Config& _liveConfig;
    std::set<IConfigView* const> _views;
};

}
}
