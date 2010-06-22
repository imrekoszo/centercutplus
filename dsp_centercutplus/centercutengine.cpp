#include "centercutengine.h"
#include <QVector>
#include "ffengine.h"

// initialization
const double CenterCutEngine::NoDivByZero = 0.000000000000001;

// engine implementation

int CenterCutEngine::Init()
{
    InitOutputBuffer();
    Start();
    _isInitialized = true;
    return 0;
}

void CenterCutEngine::Quit()
{

}

int CenterCutEngine::ModifySamples(uint8* samples, int sampleCount,
                                   int bitsPerSample, int chanCount, int sampleRate)
{
    return sampleCount;
}

void CenterCutEngine::InitOutputBuffer()
{
    for (int i = 0; i < MaxOutputBuffers; i++)
    {
        _outputBuffers[i] = NULL;
    }
    _outputBufferCount = 0;
    _outputReadSampleOffset = 0;
}

bool CenterCutEngine::Start()
{
    FFEngine::VDCreateBitRevTable(_bitRev, WindowSize);
    FFEngine::VDCreateHalfSineTable(_sineTab, WindowSize);

    _inputSamplesNeeded = OverlapSize;
    _inputPos = 0;

    _outputDiscardBlocks = OverlapCount - 1;

    memset(_input, 0, sizeof _input);
    memset(_overlapC, 0, sizeof _overlapC);

    QVector<double> tmpBuffer(WindowSize);
    double *tmp = &(tmpBuffer[0]);
    if (!tmp) return false;
    FFEngine::VDCreateRaisedCosineWindow(tmp, WindowSize, 1.0);
    for(uint32 i = 0; i < WindowSize; ++i)
    {
        // The correct Hartley<->FFT conversion is:
        //
        //	Fr(i) = 0.5(Hr(i) + Hi(i))
        //	Fi(i) = 0.5(Hr(i) - Hi(i))
        //
        // We omit the 0.5 in both the forward and reverse directions,
        // so we have a 0.25 to put here.

        _preWindow[i] = tmp[_bitRev[i]] * 0.5 * (2.0 / static_cast<double>(OverlapCount));
    }

    FFEngine::CreatePostWindow(_postWindow, WindowSize, PostWindowPower);

    return true;
}
