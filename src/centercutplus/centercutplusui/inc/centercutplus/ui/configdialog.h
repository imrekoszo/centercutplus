#pragma once

#include <boost/ptr_container/ptr_map.hpp>

#include <centercutplus/common/types.h>
#include <centercutplus/configuration/iconfigview.h>
#include "dll.h"
#include "dialog.h"

namespace ccp
{
namespace core
{
class EngineConfig;
}
namespace configuration
{
class IConfigViewModel;
}

namespace ui
{

class CENTERCUTPLUSUI_API ConfigDialog : public Dialog, public configuration::IConfigView
{
    // typedefs
  private:
    typedef int (core::EngineConfig::*GetPercentMemFn)() const;
    typedef void (configuration::ConfigController::*SetPercentMemFn)(int, const void*);
    struct OutputSliderMetadata
    {
        GetPercentMemFn getter;
        SetPercentMemFn setter;
        UINT labelId;

        OutputSliderMetadata() : getter(NULL), setter(NULL), labelId(0u) { }

        OutputSliderMetadata(GetPercentMemFn getter, SetPercentMemFn setter, UINT labelId)
            : getter(getter), setter(setter), labelId(labelId)
        {
        }
    };

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
    void OnInitDialog();
    void OnVScroll(LPARAM lParam);
    void OnCommand(WORD wParamHi, WORD wParamLo, LPARAM lParam);
    bool IsBackendReady();
    void TryInitControls();
    void TryUnsubscribe();
    void InitOutputSliders();
    void UpdateOutputSliders();
    void SaveOutputSliderPercentValue(UINT sliderId);
    int GetOutputSliderPercentValue(UINT sliderId);
    void UpdateLabels();
    void UpdateLabel(UINT id, const tstring& newValue);
    void UpdateBypass();
    void SaveBypass();
    static boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& GetOutputSliderMetadata();
    static boost::ptr_map<UINT, OutputSliderMetadata>& InitOutputSliderMetadata();
    static bool IsOutputSliderId(UINT id);

    // data
  private:
    configuration::ConfigController* _controller;
    configuration::IConfigViewModel* _model;

  private:
    NONCOPYABLE(ConfigDialog);
};

}
}
