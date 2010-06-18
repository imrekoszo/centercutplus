#ifndef DSP_CENTERCUTPLUS_GLOBAL_H
#define DSP_CENTERCUTPLUS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <windows.h>

#if defined(DSP_CENTERCUTPLUS_LIBRARY)
#  define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_IMPORT
#endif

// common types
typedef signed __int64      sint64;
typedef unsigned __int64	uint64;
typedef signed int			sint32;
typedef unsigned int		uint32;
typedef signed short		sint16;
typedef unsigned short		uint16;
typedef signed char			sint8;
typedef unsigned char		uint8;

typedef sint64				int64;
typedef sint32				int32;
typedef sint16				int16;
typedef sint8				int8;

#endif // DSP_CENTERCUTPLUS_GLOBAL_H
