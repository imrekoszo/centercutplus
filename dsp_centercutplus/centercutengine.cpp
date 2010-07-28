// implements class
#include "centercutengine.h"

// standard headers
#include <math.h>
#include <stdint.h>
#include <string.h>

// local headers
#include "ffengine.h"
#include "globals.h"


// locals
namespace
{
    const int kPostWindowPower = 2;
    const int kHalfWindow = consts::cce::WindowSize/2;
    const int kOutputSampleCount = consts::cce::OverlapSize;
    const double kNoDivByZero = 0.000000000000001;

    enum { BytesToDouble = 0, DoubleToBytes = 1};

    template <typename T>
    inline T min(T a, T b) { return a < b ? a : b; }
}

CenterCutEngine::CenterCutEngine() :
        _isInitialized(false),
        _input(boost::extents[consts::cce::WindowSize][2],
               boost::c_storage_order()),
        _overlapC(boost::extents[consts::cce::OverlapCount - 1]
                                [consts::cce::OverlapSize],
                  boost::c_storage_order())
{
}

int CenterCutEngine::Init()
{
    InitOutputBuffers();
    Start();
    _isInitialized = true;
    return 0;
}

void CenterCutEngine::Quit()
{
    FreeOutputBuffers();
    _isInitialized = false;
}

int CenterCutEngine::ModifySamples(uint8_t* samples, int sampleCount,
                                   int bitsPerSample, int chanCount,
                                   int sampleRate)
{
    if (    (chanCount == 2)
            && (sampleCount > 0)
            && BPSIsValid(bitsPerSample)
            && _isInitialized
            && false  //&& !Config::IsBypassed() // TODO
            )
    {
        int outSampleCount = ProcessSamples(samples, sampleCount, samples,
                                            bitsPerSample, sampleRate);

        if (outSampleCount >= 0)
        {
            sampleCount = outSampleCount;
        }
    }
    return sampleCount;
}

void CenterCutEngine::InitOutputBuffers()
{
    FreeOutputBuffers();
    _outputReadSampleOffset = 0;
}

void CenterCutEngine::FreeOutputBuffers()
{
    _outputBuffers = outputbuffercollection_type();
}

void CenterCutEngine::OutputBufferReadComplete()
{
    --_outputBuffersInUse;
    _outputReadSampleOffset = 0;
    if (_outputBuffersInUse > 0)
    {
        _outputBuffers.push_back(_outputBuffers.front());
        _outputBuffers.pop_front();
    }
}

bool  CenterCutEngine::OutputBufferBeginWrite()
{
    if (_outputBuffersInUse == consts::cce::MaxOutputBuffers)
    {
        return false;
    }

    _outputBuffers.push_back(outbuffer_type(new double[kOutputSampleCount*2]));

    ++_outputBuffersInUse;
    return true;
}

void CenterCutEngine::Start()
{
    FFEngine::VDCreateBitRevTable(_bitRev.begin(), consts::cce::WindowSize);
    FFEngine::VDCreateHalfSineTable(_sineTab.begin(), consts::cce::WindowSize);

    _inputSamplesNeeded = consts::cce::OverlapSize;
    _inputPos = 0;

    _outputDiscardBlocks = consts::cce::OverlapCount - 1;

    memset(_input.origin(), 0,
           _input.num_elements() * sizeof(double2Array_type::element));
    memset(_overlapC.origin(), 0,
           _overlapC.num_elements() * sizeof(double2Array_type::element));

    doubleArray_type<consts::cce::WindowSize> tmpBuffer;
    FFEngine::VDCreateRaisedCosineWindow(tmpBuffer.begin(), tmpBuffer.size(),
                                         1.0);
    size_t len = _preWindow.size();
    for(size_t i = 0; i < len; ++i)
    {
        // The correct Hartley<->FFT conversion is:
        //
        //	Fr(i) = 0.5(Hr(i) + Hi(i))
        //	Fi(i) = 0.5(Hr(i) - Hi(i))
        //
        // We omit the 0.5 in both the forward and reverse directions,
        // so we have a 0.25 to put here.

        _preWindow[i] = tmpBuffer[_bitRev[i]] * 0.5 *
                        (2.0 / static_cast<double>(consts::cce::OverlapCount));
    }

    FFEngine::CreatePostWindow(_postWindow.begin(), consts::cce::WindowSize,
                               kPostWindowPower);
}

bool CenterCutEngine::BPSIsValid(int bitsPerSample)
{
    // Bits per sample must be between 8 and 32 bits, and a multiple of 8
    return (bitsPerSample >= 8) &&
           (bitsPerSample <= 32) &&
           ((bitsPerSample & 7) == 0);
}

int  CenterCutEngine::ProcessSamples(uint8_t *inSamples, int inSampleCount,
                                     uint8_t *outSamples, int bitsPerSample,
                                     int sampleRate)
{
    int bytesPerSample, outSampleCount, maxOutSampleCount, copyCount;

    _sampleRate = sampleRate;
    bytesPerSample = bitsPerSample / 8;
    outSampleCount = 0;
    maxOutSampleCount = inSampleCount;

    while (inSampleCount > 0)
    {
        copyCount = min(static_cast<int>(_inputSamplesNeeded), inSampleCount);

        ConvertSamples(BytesToDouble, inSamples, &_input[_inputPos][0],
                       copyCount, bitsPerSample, 2);

        inSamples += copyCount * bytesPerSample * 2;
        inSampleCount -= copyCount;
        _inputPos = (_inputPos + copyCount) & (consts::cce::WindowSize - 1);
        _inputSamplesNeeded -= copyCount;

        if (_inputSamplesNeeded == 0)
        {
            Run();
        }
    }

    while ((_outputBuffersInUse > 0) && (outSampleCount < maxOutSampleCount))
    {
        double *sampD = _outputBuffers.front().get();

        copyCount = min(kOutputSampleCount - _outputReadSampleOffset,
                        maxOutSampleCount - outSampleCount);

        ConvertSamples(DoubleToBytes, outSamples, sampD + (_outputReadSampleOffset * 2),
                       copyCount, bitsPerSample, 2);

        outSamples += copyCount * bytesPerSample * 2;
        outSampleCount += copyCount;
        _outputReadSampleOffset += copyCount;
        if (_outputReadSampleOffset == kOutputSampleCount)
        {
            OutputBufferReadComplete();
        }
    }

    return outSampleCount;
}

void CenterCutEngine::ConvertSamples(int type, uint8_t *sampB, double *sampD,
                                     int sampleCount, int bitsPerSample, int chanCount)
{
    static const double SampleScaleInv = 32768.0;
    static const double SampleScale = 1.0 / SampleScaleInv;
    static const double SampleMin = -2147483648.0;
    static const double SampleMax = 2147483647.0;

    int bytesPerSample, shiftCount;
    int32_t ixor;
    uint8_t *max;

    bytesPerSample = (bitsPerSample + 7) / 8;
    shiftCount = (4 - bytesPerSample) * 8;
    ixor = (bytesPerSample == 1) ? (1 << 31) : 0;
    max = sampB + (sampleCount * bytesPerSample * chanCount);

    if (type == BytesToDouble)
    {
        int32_t tempI;

        while (sampB < max)
        {
            tempI = (*(reinterpret_cast<int32_t*>(sampB)) << shiftCount) ^ ixor;
            *sampD = static_cast<double>(tempI) * SampleScale;

            sampB += bytesPerSample;
            sampD += 1;
        }
    }
    else
    {
        uint8_t *maxw = max - 3;
        double tempD;
        uint32_t tempI;

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
            tempI = static_cast<uint32_t>(static_cast<int32_t>(tempD) ^ ixor) >> shiftCount;

            if (sampB < maxw)
            {
                *(reinterpret_cast<uint32_t*>(sampB)) = tempI;
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
{/*
    // copy to temporary buffer and FHT
    for(size_t i = 0; i < consts::cce::WindowSize; ++i)
    {
        const unsigned j = _bitRev[i];
        const unsigned k = (j + _inputPos) & (consts::cce::WindowSize - 1);
        const double w = _preWindow[i];

        _tempLBuffer[i] = _input[k][0] * w;
        _tempRBuffer[i] = _input[k][1] * w;
    }

    FFEngine::VDComputeFHT(_tempLBuffer.begin(), consts::cce::WindowSize,
                           _sineTab.begin());
    FFEngine::VDComputeFHT(_tempRBuffer.begin(), consts::cce::WindowSize,
                           _sineTab.begin());

    // read configuration from UI
    Preset preset = Config::GetPreset();
    int dividerFreq = static_cast<int>(
            (PositionHelper::GetDividerFreq(preset.FreqSlider)
                / (static_cast<double>(_sampleRate) / consts::cce::WindowSize)) + 0.5);

    // perform stereo separation
    _tempCBuffer[0] = 0;
    _tempCBuffer[1] = 0;
    for(int i = 1; i < kHalfWindow; ++i)
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
            double lR = _tempLBuffer[i] + _tempLBuffer[consts::cce::WindowSize - i];
            double lI = _tempLBuffer[i] - _tempLBuffer[consts::cce::WindowSize - i];
            double rR = _tempRBuffer[i] + _tempRBuffer[consts::cce::WindowSize - i];
            double rI = _tempRBuffer[i] - _tempRBuffer[consts::cce::WindowSize - i];

            double sumR = lR + rR;
            double sumI = lI + rI;
            double diffR = lR - rR;
            double diffI = lI - rI;

            double sumSq = sumR*sumR + sumI*sumI;
            double diffSq = diffR*diffR + diffI*diffI;
            double alpha = 0.0;

            if (sumSq > kNoDivByZero)
            {
                alpha = 0.5 - sqrt(diffSq/sumSq)*0.5;
            }

            cR = sumR * alpha;
            cI = sumI * alpha;
        }

        _tempCBuffer[_bitRev[i             ]] = cR + cI;
        _tempCBuffer[_bitRev[consts::cce::WindowSize - i]] = cR - cI;
    }

    // reconstitute left/right/center channels
    FFEngine::VDComputeFHT(_tempCBuffer.begin(), consts::cce::WindowSize,
                           _sineTab.begin());

    // apply post-window
    for (size_t i = 0; i < consts::cce::WindowSize; ++i)
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
        size_t currentBlockIndex, nextBlockIndex, blockOffset;

        if (!OutputBufferBeginWrite())
        {
            return; // TODO: exception?
        }
        double *outBuffer = _outputBuffers[_outputBuffersInUse - 1].get();

        for(size_t i = 0; i < consts::cce::OverlapSize; ++i)
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
            blockOffset = consts::cce::OverlapSize;
            while (nextBlockIndex < consts::cce::OverlapCount - 1)
            {
                _overlapC[currentBlockIndex][i] =
                    _overlapC[nextBlockIndex][i] + _tempCBuffer[blockOffset +i];

                ++currentBlockIndex;
                ++nextBlockIndex;
                blockOffset += consts::cce::OverlapSize;
            }
            _overlapC[currentBlockIndex][i] = _tempCBuffer[blockOffset + i];
        }
    }

    _inputSamplesNeeded = consts::cce::OverlapSize;

    return;*/
}
