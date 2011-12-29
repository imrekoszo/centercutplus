#pragma once

#include <boost/utility.hpp>
#include <centercutplus/configuration/iconfigview.h>

class View : public ccp::configuration::IConfigView, boost::noncopyable
{
    // ctor
  public:
    View() : Updated(false), Origin(NULL) { }

    // methods
  public:
    void Update(const void* origin) { Updated = true; Origin = origin; }

    // data
  public:
    bool Updated;
    const void* Origin;
};
