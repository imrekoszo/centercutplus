#ifndef DSP_CENTERCUTPLUS_GLOBAL_H
#define DSP_CENTERCUTPLUS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DSP_CENTERCUTPLUS_LIBRARY)
#   define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_EXPORT
#else
#   define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_IMPORT
#endif

// common types
//typedef signed __int64      sint64;
//typedef unsigned __int64    uint64;

typedef signed int          int32;
typedef unsigned int        uint32;

typedef signed short        int16;
typedef unsigned short      uint16;

typedef signed char         int8;
typedef unsigned char       uint8;

#endif // DSP_CENTERCUTPLUS_GLOBAL_H
