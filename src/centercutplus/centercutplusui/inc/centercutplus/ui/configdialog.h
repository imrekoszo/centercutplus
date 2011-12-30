#pragma once

#include <centercutplus/configuration/iconfigview.h>
#include "dll.h"
#include "dialog.h"

namespace ccp
{

namespace configuration
{
class IConfigViewModel;
}

namespace ui
{

class CENTERCUTPLUSUI_API ConfigDialog : public Dialog, public configuration::IConfigView
{
    // ctor
  public:
    ConfigDialog();
    virtual ~ConfigDialog();

    // methods
  public:
    INT_PTR WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    void Update(const void* origin);
    void SetController(configuration::ConfigController& controller);

  private:
    void TryInitControls();
    void TryUnsubscribe();

    // data
  private:
    configuration::ConfigController* _controller;
    configuration::IConfigViewModel* _model;

  private:
    NONCOPYABLE(ConfigDialog);
};

}
}
