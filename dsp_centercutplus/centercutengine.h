#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

// standard headers
#include <stdint.h>

// library headers
#include <boost/array.hpp>

// local headers
#include "classhelpers.h"

// class-wide constants
namespace consts
{
    namespace cce
    {
        const std::size_t WindowSize = 8192;
        const int OverlapCount = 4;
        const int OverlapSize = OverlapCount/2;
        const int MaxOutputBuffers = 32;
    }
}

/*
#include <boost/multi_array.hpp>
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    typedef boost::multi_array<int, 2> intarray2d;

    intarray2d myarr1(boost::extents[10][2], boost::c_storage_order());
    intarray2d myarr2(boost::extents[10][2], boost::fortran_storage_order());

    int value = 0;
    for(intarray2d::index i = 0; i < 10; ++i)
        for(intarray2d::index j = 0; j < 2; ++j)
            myarr1[i][j] = myarr2[i][j] = ++value;


    return a.exec();
}

  */


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
    boost::array<uint32_t, consts::cce::WindowSize> _bitRev;
    boost::array<double, consts::cce::WindowSize> _preWindow;
    boost::array<double, consts::cce::WindowSize> _postWindow;
    boost::array<double, consts::cce::WindowSize> _sineTab;
    double  _input[consts::cce::WindowSize][2];
    double  _overlapC[consts::cce::OverlapCount - 1][consts::cce::OverlapSize];
    boost::array<double, consts::cce::WindowSize> _tempLBuffer;
    boost::array<double, consts::cce::WindowSize> _tempRBuffer;
    boost::array<double, consts::cce::WindowSize> _tempCBuffer;

    DISALLOW_COPY_AND_ASSIGN(CenterCutEngine);
};

#endif // CENTERCUTENGINE_H
