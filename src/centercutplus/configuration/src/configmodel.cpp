#include "configmodel.h"

#include <boost/foreach.hpp>

#include <centercutplus/core/config.h>
#include <centercutplus/core/engineconfig.h>

#include "../inc/centercutplus/configuration/iconfigview.h"


namespace ccp
{
namespace configuration
{

ConfigModel::ConfigModel(core::Config& config)
    : _liveConfig(config),
      _views()
{
}

ConfigModel::~ConfigModel()
{
}

bool ConfigModel::GetBypassed() const
{
    return _liveConfig.IsBypassed();
}

const core::EngineConfig& ConfigModel::GetCurrentEngineConfig() const
{
    return _liveConfig.engineConfig();
}

void ConfigModel::Subscribe(IConfigView& view)
{
    _views.insert(&view);
}

void ConfigModel::Unsubscribe(IConfigView& view)
{
    _views.erase(&view);
}

void ConfigModel::SetBypassed(bool bypass, const void* origin)
{
    _liveConfig.Bypass(bypass);
    UpdateViews(origin);
}

void ConfigModel::SetCenterDetectionMode(core::CenterDetectionMode value, const void* origin)
{
    _liveConfig.engineConfig().centerDetectionMode(value);
    UpdateViews(origin);
}

void ConfigModel::SetLeftToLeftPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().leftToLeftPercent(value);
    UpdateViews(origin);
}

void ConfigModel::SetCenterToLeftPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().centerToLeftPercent(value);
    UpdateViews(origin);
}

void ConfigModel::SetRightToLeftPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().rightToLeftPercent(value);
    UpdateViews(origin);
}

void ConfigModel::SetLeftToRightPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().leftToRightPercent(value);
    UpdateViews(origin);
}

void ConfigModel::SetCenterToRightPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().centerToRightPercent(value);
    UpdateViews(origin);
}

void ConfigModel::SetRightToRightPercent(int value, const void* origin)
{
    _liveConfig.engineConfig().rightToRightPercent(value);
    UpdateViews(origin);
}

void ConfigModel::AddFreqInterval(const void* origin)
{
    _liveConfig.engineConfig().centerToSidesFrequencyIntervals().push_back(new core::FrequencyInterval());
    UpdateViews(origin);
}

void ConfigModel::RemoveFreqInterval(size_t index, const void* origin)
{
    core::FrequencyIntervalVector& intervals = _liveConfig.engineConfig().centerToSidesFrequencyIntervals();
    if(index < intervals.size())
    {
        intervals.erase(intervals.begin() + index);
        UpdateViews(origin);
    }
}

void ConfigModel::SetFreqMin(size_t index, uint value, const void* origin)
{
    core::FrequencyIntervalVector& intervals = _liveConfig.engineConfig().centerToSidesFrequencyIntervals();
    if(index < intervals.size())
    {
        intervals[index].minimum(value);
        UpdateViews(origin);
    }
}

void ConfigModel::SetFreqMax(size_t index, uint value, const void* origin)
{
    core::FrequencyIntervalVector& intervals = _liveConfig.engineConfig().centerToSidesFrequencyIntervals();
    if(index < intervals.size())
    {
        intervals[index].maximum(value);
        UpdateViews(origin);
    }
}

void ConfigModel::UpdateViews(const void* origin)
{
    BOOST_FOREACH(IConfigView* view, _views)
    {
        view->Update(origin);
    }
}

}
}
