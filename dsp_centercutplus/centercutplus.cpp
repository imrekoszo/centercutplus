// implements class
#include "centercutplus.h"

// standard headers
#include <stdint.h>

// library headers
#include <windows.h>
#include <QMessageBox>

// local headers
#include "winamp_dsp.h"


// locals
namespace
{
    CenterCutPlus& Instance()
    {
        static CenterCutPlus* instance = new CenterCutPlus();
        return *instance;
    }
}

// CenterCutPlus implementation
BOOL CenterCutPlus::DllMain(ULONG reasonForCall)
{
    if(reasonForCall == DLL_PROCESS_DETACH)
    {
        Instance()._mutex.lock();
    }
    return TRUE;
}

winampDSPModule* CenterCutPlus::GetModule(int which)
{
    static winampDSPModule module = {
        const_cast<char*>(ShortName()),
        NULL,
        NULL,
        About,
        Init,
        ModifySamples,
        Quit
    };
    return which == 0 ? &module : NULL;
}

void CenterCutPlus::About(winampDSPModule* thisModule)
{
    QString text = QString(LongName()) +
                   "\nCopyright 2010 Ngetal\n"
                   "https://code.google.com/p/centercutplus/\n\n"
                   "Based on dsp_centercut v1.4.0\n"
                   "Copyright 2004-2007 Moitah\n"
                   "http://www.moitah.net\n\n"
                   "Based on VirtualDub's Center Cut filter by Avery Lee.";

    ::MessageBoxA(thisModule->hwndParent, text.toStdString().c_str(),
                  "About", MB_OK);
}

int CenterCutPlus::Init(winampDSPModule* thisModule)
{
    return Instance().InternalInit(thisModule);
}

int CenterCutPlus::InternalInit(winampDSPModule* /*thisModule*/)
{
    QMutexLocker lock(&_mutex);

    // TODO: create config window

    return _engine.Init();
}

void CenterCutPlus::Quit(winampDSPModule* thisModule)
{
    return Instance().InternalQuit(thisModule);
}

void CenterCutPlus::InternalQuit(winampDSPModule* /*thisModule*/)
{
    QMutexLocker lock(&_mutex);

    // TODO: destroy config window
    // TODO: DelayDllUnload(tököm);

    _engine.Quit();
}

int CenterCutPlus::ModifySamples(winampDSPModule* thisModule, uint8_t* samples,
                                 int sampleCount, int bitsPerSample,
                                 int chanCount, int sampleRate)
{
    return Instance().InternalModifySamples(thisModule, samples, sampleCount,
                                            bitsPerSample, chanCount,
                                            sampleRate);
}

int CenterCutPlus::InternalModifySamples(winampDSPModule* /*thisModule*/,
                                         uint8_t* samples, int sampleCount,
                                         int bitsPerSample, int chanCount,
                                         int sampleRate)
{
    QMutexLocker lock(&_mutex);
    return _engine.ModifySamples(samples, sampleCount, bitsPerSample, chanCount,
                                 sampleRate);
}
