#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

#include "globals.h"

class CenterCutEngine
{
public:
    CenterCutEngine();
    void Init();
    void Quit();
    int ModifySamples(uint8* samples, int sampleCount,
                      int bitsPerSample, int chanCount, int sampleRate);
};

#endif // CENTERCUTENGINE_H
