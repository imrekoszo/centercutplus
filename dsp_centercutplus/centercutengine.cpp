// implements class
#include "centercutengine.h"

// standard headers
#include <math.h>

// local headers
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

void CenterCutEngine::OutputBufferReadComplete()
{
    --_outputBufferCount;
    _outputReadSampleOffset = 0;
    if (_outputBufferCount > 0)
    {
        double *moveToEnd = _outputBuffers[0];

        // Shift the buffers so that the current one for reading is at index 0
        for (int i = 1; i < MaxOutputBuffers; ++i)
        {
            _outputBuffers[i - 1] = _outputBuffers[i];
        }
        _outputBuffers[MaxOutputBuffers - 1] = NULL;

        // Move the previous first buffer to the end (first null pointer)
        for (int i = 0; i < MaxOutputBuffers; ++i)
        {
            if (!_outputBuffers[i])
            {
                _outputBuffers[i] = moveToEnd;
                break;
            }
        }
    }
}

bool  CenterCutEngine::OutputBufferBeginWrite()
{
    if (_outputBufferCount == MaxOutputBuffers)
    {
        return false;
    }

    int i = _outputBufferCount;
    if (!_outputBuffers[i])
    {
        // No buffer exists at this index, make a new one
        _outputBuffers[i] = new double[OutputSampleCount*2];
        if (!_outputBuffers[i])
        {
            return false;
        }
    }

    ++_outputBufferCount;
    return true;
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
            Run();
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

void CenterCutEngine::Run()
{
    // copy to temporary buffer and FHT
    for(int i = 0; i < WindowSize; ++i)
    {
        const unsigned j = _bitRev[i];
        const unsigned k = (j + _inputPos) & (WindowSize - 1);
        const double w = _preWindow[i];

        _tempLBuffer[i] = _input[k][0] * w;
        _tempRBuffer[i] = _input[k][1] * w;
    }

    FFEngine::VDComputeFHT(_tempLBuffer, WindowSize, _sineTab);
    FFEngine::VDComputeFHT(_tempRBuffer, WindowSize, _sineTab);

    // read configuration from UI
    Preset preset = Config::GetPreset();
    int dividerFreq = static_cast<int>(
            (PositionHelper::GetDividerFreq(preset.FreqSlider)
                / (static_cast<double>(_sampleRate) / WindowSize)) + 0.5);

    // perform stereo separation
    _tempCBuffer[0] = 0;
    _tempCBuffer[1] = 0;
    for(int i = 1; i < HalfWindow; ++i)
    {
        bool keepCurrentInCenter =
                preset.CenterModeSetting == CenterMode::None
                || preset.CenterModeSetting == CenterMode::LowToSides
                    && (i >= dividerFreq)
                || preset.CenterModeSetting == CenterMode::HighToSides
                    && (i < dividerFreq);

        double cR = 0.0;
        double cI = 0.0;

        if(keepCurrentInCenter)
        {
            double lR = _tempLBuffer[i] + _tempLBuffer[WindowSize - i];
            double lI = _tempLBuffer[i] - _tempLBuffer[WindowSize - i];
            double rR = _tempRBuffer[i] + _tempRBuffer[WindowSize - i];
            double rI = _tempRBuffer[i] - _tempRBuffer[WindowSize - i];

            double sumR = lR + rR;
            double sumI = lI + rI;
            double diffR = lR - rR;
            double diffI = lI - rI;

            double sumSq = sumR*sumR + sumI*sumI;
            double diffSq = diffR*diffR + diffI*diffI;
            double alpha = 0.0;

            if (sumSq > NoDivByZero)
            {
                alpha = 0.5 - sqrt(diffSq/sumSq)*0.5;
            }

            cR = sumR * alpha;
            cI = sumI * alpha;
        }

        _tempCBuffer[_bitRev[i             ]] = cR + cI;
        _tempCBuffer[_bitRev[WindowSize - i]] = cR - cI;
    }

    // reconstitute left/right/center channels

    FFEngine::VDComputeFHT(_tempCBuffer, WindowSize, _sineTab);

    // apply post-window

    for (int i = 0; i < WindowSize; ++i)
    {
        _tempCBuffer[i] *= _postWindow[i];
    }

    // writeout


    if (_outputDiscardBlocks > 0)
    {
        _outputDiscardBlocks--;
    }
    else
    {
        int currentBlockIndex, nextBlockIndex, blockOffset;

        if (!OutputBufferBeginWrite()) return; // TODO: exception?
        double *outBuffer = _outputBuffers[_outputBufferCount - 1];
        if (!outBuffer) return; // TODO: exception?

        for(int i = 0; i < OverlapSize; ++i)
        {
            double c = _overlapC[0][i] + _tempCBuffer[i];
            double l = _input[_inputPos + i][0] - c;
            double r = _input[_inputPos + i][1] - c;

            PositionHelper positionHelper(preset, l, r, c);
            outBuffer[0] = positionHelper.L();
            outBuffer[1] = positionHelper.R();
            outBuffer += 2;

            // overlapping

            currentBlockIndex = 0;
            nextBlockIndex = 1;
            blockOffset = OverlapSize;
            while (nextBlockIndex < OverlapCount - 1)
            {
                _overlapC[currentBlockIndex][i] =
                    _overlapC[nextBlockIndex][i] + _tempCBuffer[blockOffset +i];

                ++currentBlockIndex;
                ++nextBlockIndex;
                blockOffset += OverlapSize;
            }
            _overlapC[currentBlockIndex][i] = _tempCBuffer[blockOffset + i];
        }
    }

    _inputSamplesNeeded = OverlapSize;

    return;
}
