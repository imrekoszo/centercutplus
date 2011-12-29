#include "../inc/centercutplus/core/config.h"

namespace ccp
{
namespace core
{

Config::Config()
    : _isBypassed(true),
      _engineConfig()
{
}

Config::~Config()
{
}

bool Config::IsBypassed() const
{
    return _isBypassed;
}

void Config::Bypass(bool bypass)
{
    _isBypassed = bypass;
}

const EngineConfig& Config::engineConfig() const
{
    return _engineConfig;
}

EngineConfig& Config::engineConfig()
{
    return _engineConfig;
}

}
}
