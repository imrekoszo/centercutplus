#include "../inc/centercutplus/core/config.h"

namespace ccp
{
namespace core
{

Config::Config()
    : _isBypassed(true),
      _engineConfig(new EngineConfig())
{
}

Config::~Config()
{
}

bool Config::isBypassed() const
{
    return _isBypassed;
}

const boost::shared_ptr<EngineConfig>& Config::engineConfig() const
{
    return _engineConfig;
}

boost::shared_ptr<EngineConfig>& Config::engineConfig()
{
    return _engineConfig;
}

}
}
