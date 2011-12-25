#pragma once

namespace ccp
{

namespace core
{
class EngineConfig;
}

namespace configuration
{

class IConfigView;

class IConfigViewModel
{
    // methods
  public:
    virtual bool GetBypassed() const = 0;
    virtual const core::EngineConfig& GetCurrentEngineConfig() const = 0;
    // TODO: presets

    virtual void Subscribe(const IConfigView& view) const = 0;
    virtual void Unsubscribe(const IConfigView& view) const = 0;
};

}
}
