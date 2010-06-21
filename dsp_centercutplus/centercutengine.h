#ifndef CENTERCUTENGINE_H
#define CENTERCUTENGINE_H

#include "globals.h"
#include <QVector>

class CenterCutEngine
{
    CenterCutEngine(const CenterCutEngine&) { }

    static const int mOutputMaxBuffers;
    int              mOutputReadSampleOffset;
    int              mOutputBufferCount;  // How many buffers are actually in use
                                          // (there may be more allocated than in use)

    // TODO: QVector here? QVector<QPointer<QVector> > > ?
    QVector<QVector<double> > mOutputBuffer;

public:
    CenterCutEngine() { }
    int Init();
    void Quit();
    int ModifySamples(uint8* samples, int sampleCount,
                      int bitsPerSample, int chanCount, int sampleRate);

private:
    void OutputBufferInit();
};

#endif // CENTERCUTENGINE_H
