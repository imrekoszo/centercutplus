#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "engineconfig.h"

namespace ccp
{
namespace core
{

class Config : boost::noncopyable
{
    // ctor
  public:
    Config();
    virtual ~Config();

    // methods
  public:
    bool isBypassed() const;
    const boost::shared_ptr<EngineConfig>& engineConfig() const;
    boost::shared_ptr<EngineConfig>& engineConfig();

    // data
  private:
    bool _isBypassed;
    boost::shared_ptr<EngineConfig> _engineConfig;
};

}
}
