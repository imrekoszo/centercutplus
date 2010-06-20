#include "dsp_centercutplus_global.h"
#include <windows.h>

bool    mInitialized = false;
HANDLE  hMutexRunning = 0;

void Lock(bool bRunning)
{
    if (bRunning)
        ::WaitForSingleObject(hMutexRunning, INFINITE);
    else
        ::ReleaseMutex(hMutexRunning);
}

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            mDllHandle = static_cast<HINSTANCE>(hInst);
            hMutexRunning = ::CreateMutex(NULL, FALSE, NULL);
            break;
        case DLL_PROCESS_DETACH:
            Lock(true);
            break;
    }
    return TRUE;
}
