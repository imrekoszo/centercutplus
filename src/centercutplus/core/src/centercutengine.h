#include "../inc/centercutplus/core/types.h"

namespace ccp
{
namespace core
{

class CenterCutEngine : boost::noncopyable
{
    // ctor
  public:
    CenterCutEngine();
    virtual ~CenterCutEngine();

    // methods
  public:
    void Init();
    void DeInit();
    int ModifySamples(struct winampDSPModule *thisModule, byte *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
  private:
    int ProcessSamples(byte *inSamples, int inSampleCount, byte *outSamples, int bitsPerSample, int sampleRate);
    void ConvertSamples(int type, byte *sampB, double *sampD, int sampleCount, int bitsPerSample, int chanCount);
    void OutputBufferReadComplete();
    bool OutputBufferBeginWrite();
    bool BPSIsValid(int bitsPerSample);
    bool CenterCut_Run();
    void OutputBufferInit();
    void OutputBufferFree();
    bool CenterCut_Start();
    void CenterCut_Finish();
};

}
}
