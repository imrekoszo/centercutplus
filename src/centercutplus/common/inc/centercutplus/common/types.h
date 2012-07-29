#pragma once

#include <string>
#include <sstream>

#include <tchar.h>

namespace ccp
{

typedef unsigned int        uint;
typedef unsigned char       byte;

//typedef signed __int64      slong;
//typedef unsigned __int64    ulong;
//typedef signed short        sshort;
//typedef unsigned short      ushort;
//typedef signed char         sbyte;
//typedef unsigned char       ubyte;

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;

}
