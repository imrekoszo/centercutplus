// implements class
#include "centercutplus.h"

// standard headers
#include <stdint.h>

// library headers
#include <windows.h>
#include <QMessageBox>
#include <boost/scoped_array.hpp>

// local headers
#include "winamp_dsp.h"
#include "configuration.h"
#include "centercutplusdialog.h"


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

int CenterCutPlus::InternalInit(winampDSPModule* thisModule)
{
    QMutexLocker lock(&_mutex);

    _configuration.Load(GetIniFilePath(thisModule->hDllInstance));
    _centerCutPlusDialog.reset(new CenterCutPlusDialog(&_configuration));
    ::SetParent(_centerCutPlusDialog->winId(), thisModule->hwndParent);

    return _engine.Init(&_configuration);
}

void CenterCutPlus::Quit(winampDSPModule* thisModule)
{
    Instance().InternalQuit(thisModule);
}

void CenterCutPlus::InternalQuit(winampDSPModule* /*thisModule*/)
{
    QMutexLocker lock(&_mutex);

    _centerCutPlusDialog.reset();
    // TODO: DelayDllUnload(tököm);

    _engine.Quit();
    _configuration.Save();
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

QString CenterCutPlus::GetIniFilePath(HMODULE hModule)
{
    const uint32_t bufferInitialSize = MAX_PATH + 10;

    // static approach
    wchar_t buffer[bufferInitialSize];
    buffer[bufferInitialSize - 1] = 0;
    DWORD result = ::GetModuleFileNameW(hModule, buffer, bufferInitialSize);

    if(result == 0)
    {
        return QString();
    }
    else if (result < bufferInitialSize)
    {
        return QString::fromWCharArray(buffer);
    }

    // dynamic approach
    uint32_t i = 1, size;
    boost::scoped_array<wchar_t> dynBuffer(NULL);
    do
    {
        size = bufferInitialSize * ++i;
        dynBuffer.reset(new wchar_t[size]);
        dynBuffer[size - 1] = 0;
        result = ::GetModuleFileNameW(hModule, dynBuffer.get(), size);
    } while (result == size);

    return QString::fromWCharArray(dynBuffer.get());
}
