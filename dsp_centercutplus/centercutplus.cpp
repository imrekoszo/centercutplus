#include "centercutplus.h"
#include "winamp_dsp.h"
#include <QMessageBox>

// static init
QMutex CenterCutPlus::_mutex;
CenterCutEngine CenterCutPlus::_engine;

// CenterCutPlus implementation
BOOL CenterCutPlus::DllMain(ULONG reasonForCall)
{
    if(reasonForCall == DLL_PROCESS_DETACH)
    {
        _mutex.lock();
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

    ::MessageBoxA(thisModule->hwndParent, text.toStdString().c_str(), "About", MB_OK);
}

int CenterCutPlus::Init(winampDSPModule*)
{
    QMutexLocker lock(&_mutex);

    // TODO: create config window

    return _engine.Init();
}

void CenterCutPlus::Quit(winampDSPModule*)
{
    QMutexLocker lock(&_mutex);

    // TODO: destroy config window

    _engine.Quit();
}

int CenterCutPlus::ModifySamples(winampDSPModule*, uint8* samples, int sampleCount,
                                 int bitsPerSample, int chanCount, int sampleRate)
{
    QMutexLocker lock(&_mutex);
    return _engine.ModifySamples(samples, sampleCount, bitsPerSample, chanCount, sampleRate);
}
