#include "../inc/centercutplus/configuration/configcontroller.h"

#include "configmodel.h"

namespace ccp
{
namespace configuration
{

class ConfigController::Impl
{
    // ctor
    explicit Impl(core::Config& config) : _model(config) { }

    // data
    ConfigModel _model;

    //friend
    friend class ConfigController;
};

ConfigController::ConfigController(core::Config& config)
    : _impl(new Impl(config))
{
}

ConfigController::~ConfigController()
{
}

const IConfigViewModel& ConfigController::Model() const
{
    return _impl->_model;
}

void ConfigController::SetBypassed(bool bypass, const void* origin)
{
    _impl->_model.SetBypassed(bypass, origin);
}

void ConfigController::SetCenterDetectionMode(core::CenterDetectionMode value, const void* origin)
{
    _impl->_model.SetCenterDetectionMode(value, origin);
}

void ConfigController::SetLeftToLeftPercent(int value, const void* origin)
{
    _impl->_model.SetLeftToLeftPercent(value, origin);
}

void ConfigController::SetCenterToLeftPercent(int value, const void* origin)
{
    _impl->_model.SetCenterToLeftPercent(value, origin);
}

void ConfigController::SetRightToLeftPercent(int value, const void* origin)
{
    _impl->_model.SetRightToLeftPercent(value, origin);
}

void ConfigController::SetLeftToRightPercent(int value, const void* origin)
{
    _impl->_model.SetLeftToRightPercent(value, origin);
}

void ConfigController::SetCenterToRightPercent(int value, const void* origin)
{
    _impl->_model.SetCenterToRightPercent(value, origin);
}

void ConfigController::SetRightToRightPercent(int value, const void* origin)
{
    _impl->_model.SetRightToRightPercent(value, origin);
}

}
}
