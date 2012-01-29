#include "../inc/centercutplus/configuration/configcontroller.h"

#include <centercutplus/core/centerdetectionmode.h>

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

IConfigViewModel* ConfigController::Model()
{
    return &(_impl->_model);
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

void ConfigController::AddFreqInterval(const void* origin)
{
    _impl->_model.AddFreqInterval(origin);
}

void ConfigController::RemoveFreqInterval(size_t index, const void* origin)
{
    _impl->_model.RemoveFreqInterval(index, origin);
}

void ConfigController::SetFreqMin(size_t index, uint value, const void* origin)
{
    _impl->_model.SetFreqMin(index, value, origin);
}

void ConfigController::SetFreqMax(size_t index, uint value, const void* origin)
{
    _impl->_model.SetFreqMax(index, value, origin);
}

void ConfigController::SetFocusPositionPercent(int value, const void* origin)
{
    _impl->_model.SetFocusPositionPercent(value, origin);
}

}
}
