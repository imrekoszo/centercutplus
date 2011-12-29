#pragma once

#include <boost/utility.hpp>
#include <centercutplus/common/types.h>

class Random : boost::noncopyable
{
    // ctor
  private:
    Random() { }

    // methods
  public:
    static ccp::uint Uint(ccp::uint min, ccp::uint max);
    static int Int(int min, int max);
    static int Percent();
    static ccp::uint Frequency();

  private:
    static void Init();
};
