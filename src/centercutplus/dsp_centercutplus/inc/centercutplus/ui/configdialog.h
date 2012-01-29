#pragma once

#include <boost/ptr_container/ptr_map.hpp>

#include <centercutplus/common/types.h>
#include <centercutplus/core/centerdetectionmode.h>
#include <centercutplus/configuration/iconfigview.h>
#include "dll.h"
#include "dialog.h"

namespace ccp
{
namespace core
{
class EngineConfig;
class FrequencyInterval;
}
namespace configuration
{
class IConfigViewModel;
}

namespace ui
{

class DSP_CENTERCUTPLUS_API ConfigDialog : public Dialog, public configuration::IConfigView
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

    typedef uint (core::FrequencyInterval::*GetFreqMemFn)() const;
    typedef void (configuration::ConfigController::*SetFreqMemFn)(size_t, uint, const void*);
    struct FreqSliderMetadata
    {
        // data
      public:
        GetFreqMemFn getter;
        SetFreqMemFn setter;
        UINT labelId;
      private:
        GetFreqMemFn _getMin;
        GetFreqMemFn _getMax;
        uint _min;
        uint _max;

        // ctor
      public:
        FreqSliderMetadata();
        FreqSliderMetadata(GetFreqMemFn getter, SetFreqMemFn setter, UINT labelId, GetFreqMemFn getMin, uint max);
        FreqSliderMetadata(GetFreqMemFn getter, SetFreqMemFn setter, UINT labelId, uint min, GetFreqMemFn getMax);

        // methods
      public:
        uint Minimum(const core::FrequencyInterval& interval);
        uint Maximum(const core::FrequencyInterval& interval);
    };

    // ctor
  public:
    ConfigDialog();
    virtual ~ConfigDialog();

    // methods
  public:
    INT_PTR DlgProc(UINT message, WPARAM wParam, LPARAM lParam);
    void Update(const void* origin);
    void SetController(configuration::ConfigController& controller);

  private:
    BOOL OnInitDialog();
    BOOL OnVScroll(LPARAM lParam);
    BOOL OnCommand(WORD wParamHi, WORD wParamLo, LPARAM lParam);
    BOOL OnButtonClicked(UINT buttonId, HWND buttonHandle);
    BOOL OnDestroy();
    bool IsBackendReady();
    void TryInitControlsWithValues();
    void TryUnsubscribe();
    void InitOutputSliders();
    void UpdateOutputSliders();
    void SaveOutputSliderPercentValue(UINT sliderId);
    int GetOutputSliderPercentValue(UINT sliderId);
    void UpdateLabels();
    void UpdateLabel(UINT id, const tstring& newValue);
    void UpdateBypass();
    void SaveBypass();
    void UpdateCenterDetectionMode();
    core::CenterDetectionMode GetCenterDetectionMode();
    void SaveCenterDetectionMode();
    static boost::ptr_map<UINT, ConfigDialog::OutputSliderMetadata>& GetOutputSliderMetadata();
    static boost::ptr_map<UINT, OutputSliderMetadata>& InitOutputSliderMetadata();
    static bool IsOutputSliderId(UINT id);
    static boost::ptr_map<UINT, ConfigDialog::FreqSliderMetadata>& GetFreqSliderMetadata();
    static boost::ptr_map<UINT, FreqSliderMetadata>& InitFreqSliderMetadata();
    static bool IsFreqSliderId(UINT id);
    void UpdateFreqSection();
    LRESULT GetSelectedFreqIntervalIndex();
    void UpdateFreqLabel(UINT id, uint value);

    // data
  private:
    configuration::ConfigController* _controller;
    configuration::IConfigViewModel* _model;

  private:
    NONCOPYABLE(ConfigDialog);
};

}
}
