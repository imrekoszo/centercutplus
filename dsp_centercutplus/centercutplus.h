#ifndef CENTERCUTPLUS_H
#define CENTERCUTPLUS_H

#include <windows.h>
#include <QMutex>
#include "globals.h"
#include "centercutengine.h"

struct winampDSPModule;

// main application class
class CenterCutPlus
{
    // TODO: move mutex into engine class
    static QMutex _mutex;
    static CenterCutEngine _engine;

    // public properties
public:
    static const char* LongName()  { return "Center Cut Plus v10.25.0.0"; }
    static const char* ShortName() { return "Center Cut Plus"; }

    // public methods
public:
    static BOOL DllMain(ULONG reasonForCall);
    static winampDSPModule* GetModule(int which);

    // private methods
private:
    static void About(winampDSPModule* thisModule);
    static int Init(winampDSPModule* thisModule);
    static void Quit(winampDSPModule* thisModule);
    static int ModifySamples(winampDSPModule* thisModule, uint8* samples, int sampleCount,
                             int bitsPerSample, int chanCount, int sampleRate);
};

#endif // CENTERCUTPLUS_H
