#pragma once

#include <cstdlib>

#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

#include <centercutplus/common/types.h>

namespace ccp
{

namespace core
{
class Config;
enum CenterDetectionMode;
}

namespace configuration
{
class IConfigViewModel;

class ConfigController : boost::noncopyable
{
    // types
  private:
    class Impl;

    // ctor
  public:
    explicit ConfigController(core::Config& config);
    virtual ~ConfigController();

    // methods
  public:
    IConfigViewModel* Model();
    void SetBypassed(bool bypass, const void* origin = NULL);
    void SetCenterDetectionMode(core::CenterDetectionMode value, const void* origin = NULL);
    void SetLeftToLeftPercent(int value, const void* origin = NULL);
    void SetCenterToLeftPercent(int value, const void* origin = NULL);
    void SetRightToLeftPercent(int value, const void* origin = NULL);
    void SetLeftToRightPercent(int value, const void* origin = NULL);
    void SetCenterToRightPercent(int value, const void* origin = NULL);
    void SetRightToRightPercent(int value, const void* origin = NULL);
    void AddFreqInterval(const void* origin = NULL);
    void RemoveFreqInterval(size_t index, const void* origin = NULL);
    void SetFreqMin(size_t index, uint value, const void* origin = NULL);
    void SetFreqMax(size_t index, uint value, const void* origin = NULL);

    // data
  private:
    boost::scoped_ptr<Impl> _impl;
};

}
}
