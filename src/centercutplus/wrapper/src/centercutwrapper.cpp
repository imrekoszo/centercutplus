#include "../inc/centercutplus/wrapper/centercutwrapper.h"

#include <centercutplus/core/config.h>
#include <centercutplus/configuration/iconfigview.h>
#include <centercutplus/configuration/configcontroller.h>

namespace ccp
{
namespace wrapper
{

CenterCutWrapper::CenterCutWrapper()
    : _config(new core::Config()),
      _configController(new configuration::ConfigController(*_config))
{
}

CenterCutWrapper::~CenterCutWrapper()
{
}

void CenterCutWrapper::AddView(configuration::IConfigView& view)
{
    view.SetController(*_configController);
}

}
}
