#ifndef CENTERCUTPLUS_H
#define CENTERCUTPLUS_H

// standard headers
#include <stdint.h>

// library headers
#include <QMutex>
#include <windows.h>

// local headers
#include "centercutengine.h"
#include "classhelpers.h"

// forward declarations
struct winampDSPModule;

// main application class
class CenterCutPlus
{
public:
    CenterCutPlus() { }
    static const char* LongName()  { return "Center Cut Plus v10.25.0.0"; }
    static const char* ShortName() { return "Center Cut Plus"; }
    static BOOL DllMain(ULONG reasonForCall);
    static winampDSPModule* GetModule(int which);

private:
    static void About(winampDSPModule* thisModule);
    static int Init(winampDSPModule* thisModule);
    static void Quit(winampDSPModule* thisModule);
    static int ModifySamples(winampDSPModule* thisModule, uint8_t* samples,
                             int sampleCount, int bitsPerSample, int chanCount,
                             int sampleRate);

    int InternalInit(winampDSPModule* thisModule);
    void InternalQuit(winampDSPModule* thisModule);
    int InternalModifySamples(winampDSPModule* thisModule, uint8_t* samples,
                              int sampleCount, int bitsPerSample, int chanCount,
                              int sampleRate);

    QMutex _mutex;
    CenterCutEngine _engine;

    DISALLOW_COPY_AND_ASSIGN(CenterCutPlus);
};

#endif // CENTERCUTPLUS_H
