#pragma once

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
    bool IsBypassed() const;
    void Bypass(bool bypass);
    const EngineConfig& engineConfig() const;
    EngineConfig& engineConfig();

    // data
  private:
    bool _isBypassed;
    EngineConfig _engineConfig;
};

}
}
