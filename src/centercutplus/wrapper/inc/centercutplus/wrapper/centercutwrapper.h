#pragma once

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

#include <centercutplus/core/config.h>

namespace ccp
{

namespace core
{
class Config;
}

namespace configuration
{
class ConfigController;
class IConfigView;
}

namespace wrapper
{

class CenterCutWrapper : boost::noncopyable
{
    // ctor
  public:
    CenterCutWrapper();
    virtual ~CenterCutWrapper();

    // methods
  public:
    void AddView(configuration::IConfigView& view);

    // data
  private:
    // TODO: have a reference to the engine instead of the config
    boost::scoped_ptr<core::Config> _config;
    boost::scoped_ptr<configuration::ConfigController> _configController;
};

}
}
