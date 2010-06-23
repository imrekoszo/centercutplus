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
    FreeOutputBuffer();
    _isInitialized = false;
}

int CenterCutEngine::ModifySamples(uint8* samples, int sampleCount,
                                   int bitsPerSample, int chanCount, int sampleRate)
{
    if (    (chanCount == 2)
            && (sampleCount > 0)
            && BPSIsValid(bitsPerSample)
            && _isInitialized
            && !Config::IsBypassed())
    {
        int outSampleCount =
                CenterCutProcessSamples(samples, sampleCount, samples, bitsPerSample, sampleRate);

        if (outSampleCount >= 0)
        {
            sampleCount = outSampleCount;
        }
    }
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

void CenterCutEngine::FreeOutputBuffer()
{
    for (int i = 0; i < MaxOutputBuffers; i++)
    {
        if (_outputBuffers[i])
        {
            delete[] _outputBuffers[i];
            _outputBuffers[i] = 0;
        }
    }
}

void CenterCutEngine::Start()
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
    if (!tmp)
        return;
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
}

bool CenterCutEngine::BPSIsValid(int bitsPerSample)
{
    // Bits per sample must be between 8 and 32 bits, and a multiple of 8
    return (bitsPerSample >= 8) && (bitsPerSample <= 32) && ((bitsPerSample & 7) == 0);
}

int  CenterCutEngine::ProcessSamples(
        uint8 *inSamples, int inSampleCount, uint8 *outSamples,
        int bitsPerSample, int sampleRate)
{
    int bytesPerSample, outSampleCount, maxOutSampleCount, copyCount;

    mSampleRate = sampleRate;
    bytesPerSample = bitsPerSample / 8;
    outSampleCount = 0;
    maxOutSampleCount = inSampleCount;

    while (inSampleCount > 0)
    {
        copyCount = min(static_cast<int>(mInputSamplesNeeded), inSampleCount);

        ConvertSamples(BYTES_TO_DOUBLE, inSamples, &mInput[mInputPos][0], copyCount, bitsPerSample, 2);

        inSamples += copyCount * bytesPerSample * 2;
        inSampleCount -= copyCount;
        mInputPos = (mInputPos + copyCount) & (kWindowSize-1);
        mInputSamplesNeeded -= copyCount;

        if (mInputSamplesNeeded == 0)
        {
            CenterCut_Run();
        }
    }

    while ((mOutputBufferCount > 0) && (outSampleCount < maxOutSampleCount))
    {
        double *sampD = mOutputBuffer[0];
        if (!sampD) return -1;

        copyCount = min(mOutputSampleCount - mOutputReadSampleOffset,
            maxOutSampleCount - outSampleCount);

        ConvertSamples(DOUBLE_TO_BYTES, outSamples, sampD + (mOutputReadSampleOffset * 2), copyCount, bitsPerSample, 2);

        outSamples += copyCount * bytesPerSample * 2;
        outSampleCount += copyCount;
        mOutputReadSampleOffset += copyCount;
        if (mOutputReadSampleOffset == mOutputSampleCount)
        {
            OutputBufferReadComplete();
        }
    }

    return outSampleCount;
}
