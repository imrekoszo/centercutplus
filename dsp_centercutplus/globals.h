#ifndef DSP_CENTERCUTPLUS_GLOBAL_H
#define DSP_CENTERCUTPLUS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DSP_CENTERCUTPLUS_LIBRARY)
#   define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_EXPORT
#else
#   define DSP_CENTERCUTPLUSSHARED_EXPORT Q_DECL_IMPORT
#endif

/*
#ifndef maximum
#   define maximum(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef minimum
#   define minimum(a,b)            (((a) < (b)) ? (a) : (b))
#endif
*/

#endif // DSP_CENTERCUTPLUS_GLOBAL_H
