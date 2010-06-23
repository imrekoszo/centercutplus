#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

#include "globals.h"

class CenterCutEngine
{
    // constants
    enum
    {
        WindowSize = 8192,
        OverlapCount = 4,
        PostWindowPower = 2,
        HalfWindow = WindowSize/2,
        OverlapSize = OverlapCount/2,
        BytesToDouble = 0,
        DoubleToBytes = 1,
        OutputSampleCount = OverlapSize,
        MaxOutputBuffers = 32
    };

    static const double NoDivByZero;
    // /constants

    bool    _isInitialized;
    int     _outputReadSampleOffset;
    int     _outputBufferCount;  // How many buffers are actually in use (there may be more allocated than in use)
    double* _outputBuffers[MaxOutputBuffers]; // NOTE: qvector or any advanced solution here?
    int     _sampleRate;
    int     _outputDiscardBlocks;
    uint32  _inputSamplesNeeded;
    uint32  _inputPos;
    uint32  _bitRev[WindowSize];
    double  _preWindow[WindowSize];
    double  _postWindow[WindowSize];
    double  _sineTab[WindowSize];
    double  _input[WindowSize][2];
    double  _overlapC[OverlapCount-1][OverlapSize];
    double  _tempLBuffer[WindowSize];
    double  _tempRBuffer[WindowSize];
    double  _tempCBuffer[WindowSize];

    CenterCutEngine(const CenterCutEngine&) { }

public:
    CenterCutEngine() : _isInitialized(false) { }
    int Init();
    void Quit();
    int ModifySamples(uint8* samples, int sampleCount,
                      int bitsPerSample, int chanCount, int sampleRate);

private:
    void InitOutputBuffer();
    void FreeOutputBuffer();
    void Start();
    bool BPSIsValid(int bitsPerSample);
    int  ProcessSamples(uint8 *inSamples, int inSampleCount,
                        uint8 *outSamples, int bitsPerSample, int sampleRate);
};

#endif // CENTERCUTENGINE_H
