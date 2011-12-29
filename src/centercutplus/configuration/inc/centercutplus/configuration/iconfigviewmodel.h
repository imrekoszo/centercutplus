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

    virtual void Subscribe(IConfigView& view) = 0;
    virtual void Unsubscribe(IConfigView& view) = 0;
};

}
}
