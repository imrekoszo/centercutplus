#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

// standard headers
#include <deque>
#include <stdint.h>

// library headers
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_array.hpp>

// local headers
#include "classhelpers.h"
#include "globals.h"

// forward declarations
class Configuration;

// class-wide constants
namespace consts
{
    namespace cce
    {
        const size_t WindowSize = 8192;
        const size_t OverlapCount = 4;
        const size_t OverlapSize = OverlapCount/2;
        const int MaxOutputBuffers = 32;
    }
}

class CentercutplustestsTest;
class DSP_CENTERCUTPLUSSHARED_EXPORT CenterCutEngine
{
    friend class CentercutplustestsTest;
public:
    CenterCutEngine();
    int Init(const Configuration * configuration);
    void Quit();
    int ModifySamples(uint8_t* samples, int sampleCount,
                      int bitsPerSample, int chanCount, int sampleRate);

private:
    template<size_t Tsize>
    class uint32Array_type : public boost::array<uint32_t, Tsize> {};

    template<size_t Tsize>
    class doubleArray_type : public boost::array<double, Tsize> {};

    typedef boost::multi_array<double, 2> double2Array_type;

    typedef boost::shared_array<double> outbuffer_type;
    typedef std::deque<outbuffer_type> outputbuffercollection_type;

    void InitOutputBuffers();
    void FreeOutputBuffers();
    void OutputBufferReadComplete();
    bool OutputBufferBeginWrite();
    void Start();
    bool BPSIsValid(int bitsPerSample);
    int  ProcessSamples(uint8_t *inSamples, int inSampleCount,
                        uint8_t *outSamples, int bitsPerSample, int sampleRate);
    void ConvertSamples(int type, uint8_t *sampB, double *sampD,
                        int sampleCount, int bitsPerSample, int chanCount);
    void Run(); // TODO: rename?

    // private fields
    const Configuration * _configuration;
    bool    _isInitialized;

    int     _outputReadSampleOffset;
    int     _outputBuffersInUse;  // (there may be more allocated than in use)
    outputbuffercollection_type _outputBuffers;

    int     _sampleRate;
    int     _outputDiscardBlocks;
    unsigned  _inputSamplesNeeded;
    unsigned  _inputPos;

    uint32Array_type<consts::cce::WindowSize> _bitRev;
    doubleArray_type<consts::cce::WindowSize> _preWindow;
    doubleArray_type<consts::cce::WindowSize> _postWindow;
    doubleArray_type<consts::cce::WindowSize> _sineTab;
    double2Array_type _input;
    double2Array_type _overlapC;
    doubleArray_type<consts::cce::WindowSize> _tempLBuffer;
    doubleArray_type<consts::cce::WindowSize> _tempRBuffer;
    doubleArray_type<consts::cce::WindowSize> _tempCBuffer;

    DISALLOW_COPY_AND_ASSIGN(CenterCutEngine);
};

#endif // CENTERCUTENGINE_H
