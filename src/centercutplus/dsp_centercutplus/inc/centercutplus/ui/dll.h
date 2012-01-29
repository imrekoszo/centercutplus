#pragma once

#ifdef DSP_CENTERCUTPLUS_EXPORTS
#define DSP_CENTERCUTPLUS_API __declspec(dllexport)
#else
#define DSP_CENTERCUTPLUS_API __declspec(dllimport)
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define NONCOPYABLE(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

