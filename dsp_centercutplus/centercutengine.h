#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

// standard headers
#include <stdint.h>

// local headers
#include "classhelpers.h"

// class-wide constants
namespace consts
{
    namespace cce
    {
        const int WindowSize = 8192;
        const int OverlapCount = 4;
        const int OverlapSize = OverlapCount/2;
        const int MaxOutputBuffers = 32;
    }
}

class CenterCutEngine
{
public:
    CenterCutEngine() : _isInitialized(false) { }
    int Init();
    void Quit();
    int ModifySamples(uint8_t* samples, int sampleCount,
                      int bitsPerSample, int chanCount, int sampleRate);

private:
    // TODO: move output buffer to separate class?
    void InitOutputBuffer();
    void FreeOutputBuffer();
    void OutputBufferReadComplete();
    bool OutputBufferBeginWrite();
    void Start();
    bool BPSIsValid(int bitsPerSample);
    int  ProcessSamples(uint8_t *inSamples, int inSampleCount,
                        uint8_t *outSamples, int bitsPerSample, int sampleRate);
    void ConvertSamples(int type, uint8_t *sampB, double *sampD,
                        int sampleCount, int bitsPerSample, int chanCount);
    void Run(); // TODO: rename?

    bool    _isInitialized;
    int     _outputReadSampleOffset;
    int     _outputBufferCount;  // How many buffers are actually in use (there may be more allocated than in use)
    double* _outputBuffers[consts::cce::MaxOutputBuffers]; // NOTE: qvector or any advanced solution here?
    int     _sampleRate;
    int     _outputDiscardBlocks;
    uint32_t  _inputSamplesNeeded;
    uint32_t  _inputPos;
    uint32_t  _bitRev[consts::cce::WindowSize];
    double  _preWindow[consts::cce::WindowSize];
    double  _postWindow[consts::cce::WindowSize];
    double  _sineTab[consts::cce::WindowSize];
    double  _input[consts::cce::WindowSize][2];
    double  _overlapC[consts::cce::OverlapCount - 1][consts::cce::OverlapSize];
    double  _tempLBuffer[consts::cce::WindowSize];
    double  _tempRBuffer[consts::cce::WindowSize];
    double  _tempCBuffer[consts::cce::WindowSize];

    DISALLOW_COPY_AND_ASSIGN(CenterCutEngine);
};

#endif // CENTERCUTENGINE_H
