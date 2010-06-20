#include "centercutplus.h"
#include "winamp_dsp.h"
#include <QMessageBox>
#include "qwinwidget.h"

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
    QWinWidget parent(thisModule->hwndParent, NULL, 0);
    QString title = "About";
    QString text = QString(LongName()) +
                   "\nCopyright 2010 Ngetal\n"
                   "https://code.google.com/p/centercutplus/\n\n"
                   "Based on dsp_centercut v1.4.0\n"
                   "Copyright 2004-2007 Moitah\n"
                   "http://www.moitah.net\n\n"
                   "Based on VirtualDub's Center Cut filter by Avery Lee.";
    parent.showCentered();
    QMessageBox::about(&parent, title, text);
}

int CenterCutPlus::Init(winampDSPModule* thisModule)
{
    QMutexLocker lock(&_mutex);

    // TODO: fill it!

    return 0;
}

void CenterCutPlus::Quit(winampDSPModule* thisModule)
{
    QMutexLocker lock(&_mutex);

    // TODO: fill it!
}

int CenterCutPlus::ModifySamples(winampDSPModule* thisModule, uint8* samples, int sampleCount,
                                 int bitsPerSample, int chanCount, int sampleRate)
{
    QMutexLocker lock(&_mutex);

    // TODO: fill it!

    return sampleCount;
}
