#include "centercutengine.h"
#include <QVector>
#include "ffengine.h"
#include "globals.h"
#include <math.h>

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
                ProcessSamples(samples, sampleCount, samples, bitsPerSample, sampleRate);

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

    _sampleRate = sampleRate;
    bytesPerSample = bitsPerSample / 8;
    outSampleCount = 0;
    maxOutSampleCount = inSampleCount;

    while (inSampleCount > 0)
    {
        copyCount = minimum(static_cast<int>(_inputSamplesNeeded), inSampleCount);

        ConvertSamples(BytesToDouble, inSamples, &_input[_inputPos][0],
                       copyCount, bitsPerSample, 2);

        inSamples += copyCount * bytesPerSample * 2;
        inSampleCount -= copyCount;
        _inputPos = (_inputPos + copyCount) & (WindowSize - 1);
        _inputSamplesNeeded -= copyCount;

        if (_inputSamplesNeeded == 0)
        {
            CenterCut_Run();
        }
    }

    while ((_outputBufferCount > 0) && (outSampleCount < maxOutSampleCount))
    {
        double *sampD = _outputBuffers[0];
        if (!sampD) return -1;

        copyCount = minimum(OutputSampleCount - _outputReadSampleOffset,
            maxOutSampleCount - outSampleCount);

        ConvertSamples(DoubleToBytes, outSamples, sampD + (_outputReadSampleOffset * 2),
                       copyCount, bitsPerSample, 2);

        outSamples += copyCount * bytesPerSample * 2;
        outSampleCount += copyCount;
        _outputReadSampleOffset += copyCount;
        if (_outputReadSampleOffset == OutputSampleCount)
        {
            OutputBufferReadComplete();
        }
    }

    return outSampleCount;
}

void CenterCutEngine::ConvertSamples(int type, uint8 *sampB, double *sampD,
                                     int sampleCount, int bitsPerSample, int chanCount)
{
    static const double SampleScaleInv = 32768.0;
    static const double SampleScale = 1.0 / SampleScaleInv;
    static const double SampleMin = -2147483648.0;
    static const double SampleMax = 2147483647.0;

    int bytesPerSample, shiftCount;
    int32 ixor;
    uint8 *max;

    bytesPerSample = (bitsPerSample + 7) / 8;
    shiftCount = (4 - bytesPerSample) * 8;
    ixor = (bytesPerSample == 1) ? (1 << 31) : 0;
    max = sampB + (sampleCount * bytesPerSample * chanCount);

    if (type == BytesToDouble)
    {
        int32 tempI;

        while (sampB < max)
        {
            tempI = (*((int32*)sampB) << shiftCount) ^ ixor;
            *sampD = static_cast<double>(tempI) * SampleScale;

            sampB += bytesPerSample;
            sampD += 1;
        }
    }
    else
    {
        uint8 *maxw = max - 3;
        double tempD;
        uint32 tempI;

        while (sampB < max)
        {
            tempD = *sampD * SampleScaleInv;
            if (tempD > 0.0)
            {
                if (tempD > SampleMax)
                {
                    tempD = SampleMax;
                }
                tempD += 0.5;
            }
            else
            {
                if (tempD < SampleMin)
                {
                    tempD = SampleMin;
                }
                tempD -= 0.5;
            }
            tempI = static_cast<uint32>(static_cast<int32>(tempD) ^ ixor) >> shiftCount;

            if (sampB < maxw)
            {
                *(reinterpret_cast<uint32*>(sampB)) = tempI;
            }
            else
            {
                memcpy(sampB, &tempI, bytesPerSample);
            }

            sampB += bytesPerSample;
            sampD += 1;
        }
    }
}
