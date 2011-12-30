#pragma once


namespace ccp
{
namespace configuration
{
class ConfigController;

class IConfigView
{
    // methods
  public:
    virtual void Update(const void* origin) = 0;
    virtual void SetController(ConfigController& controller) = 0;
};

}
}
