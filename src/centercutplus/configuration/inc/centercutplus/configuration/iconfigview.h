#pragma once


namespace ccp
{
namespace configuration
{

class IConfigView
{
    // methods
  public:
    virtual void Update(const void* origin) = 0;
};

}
}
