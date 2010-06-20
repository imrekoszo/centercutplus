#include "centercutengine.h"

// initialization
const int CenterCutEngine::mOutputMaxBuffers = 32;

// engine implementation

int CenterCutEngine::Init()
{
    OutputBufferInit();
}

void CenterCutEngine::Quit()
{

}

int CenterCutEngine::ModifySamples(uint8* samples, int sampleCount,
                                   int bitsPerSample, int chanCount, int sampleRate)
{
    return sampleCount;
}

// TODO: redo this with QVector?
void CenterCutEngine::OutputBufferInit()
{
    for (int i = 0; i < mOutputMaxBuffers; i++)
    {
        mOutputBuffer[i] = 0;
    }
    mOutputBufferCount = 0;
    mOutputReadSampleOffset = 0;
}
