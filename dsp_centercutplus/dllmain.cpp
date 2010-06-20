#include "globals.h"
#include <windows.h>
#include "winamp_dsp.h"
#include "centercutplus.h"

// global function implementations
// dllmain
BOOL WINAPI DllMain(HANDLE, ULONG reasonForCall, LPVOID)
{
    return CenterCutPlus::DllMain(reasonForCall);
}

// the only exported function
DSP_CENTERCUTPLUSSHARED_EXPORT winampDSPHeader* winampDSPGetHeader2()
{
    static winampDSPHeader header = {
        DSP_HDRVER,
        const_cast<char*>(CenterCutPlus::LongName()),
        CenterCutPlus::GetModule
    };
    return &header;
}
