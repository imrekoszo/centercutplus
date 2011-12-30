#pragma once

#ifdef CENTERCUTPLUSUI_EXPORTS
#define CENTERCUTPLUSUI_API __declspec(dllexport)
#else
#define CENTERCUTPLUSUI_API __declspec(dllimport)
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define NONCOPYABLE(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

