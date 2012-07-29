#pragma once

#include <Windows.h>
#include <centercutplus/common/types.h>

namespace ccp
{
// wrapper
HINSTANCE		mDllHandle;

winampDSPModule *GetModule(int which);
void DelayDLLUnload(HMODULE hDllInstance);

// /wrapper

// integration
HANDLE			hMutexRunning = 0;
void Lock(bool bRunning);


bool			mInitialized = false;

void About(struct winampDSPModule *thisModule);
int Init_CenterCut(struct winampDSPModule *thisModule);
void Quit_CenterCut(struct winampDSPModule *thisModule);
int ModifySamples(struct winampDSPModule *thisModule, byte *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
// /integration



void OutputBufferInit();
void OutputBufferFree();
bool CenterCut_Start();
void CenterCut_Finish();



HINSTANCE		hDummy;

const int		kWindowSize = 8192;
const int		kOverlapCount = 4;
const int		kPostWindowPower = 2;  // Maximum power including pre-window is kOverlapCount-1,
									   // which means this can be kOverlapCount-2 at most
const int		kHalfWindow = kWindowSize / 2;
const int		kOverlapSize = kWindowSize / kOverlapCount;

const double	nodivbyzero = 0.000000000000001;

const int		BYTES_TO_DOUBLE = 0;
const int		DOUBLE_TO_BYTES = 1;

const int		mOutputSampleCount = kOverlapSize;
const int		mOutputMaxBuffers = 32;
int				mOutputReadSampleOffset;
int				mOutputBufferCount;  // How many buffers are actually in use (there may be more
									 // allocated than in use)
double			*mOutputBuffer[mOutputMaxBuffers];

int				mSampleRate;
int				mOutputDiscardBlocks;
uint			mInputSamplesNeeded;
uint			mInputPos;
unsigned		mBitRev[kWindowSize];
double			mPreWindow[kWindowSize];
double			mPostWindow[kWindowSize];
double			mSineTab[kWindowSize];
double			mInput[kWindowSize][2];
double			mOverlapC[kOverlapCount-1][kOverlapSize];
double			mTempLBuffer[kWindowSize];
double			mTempRBuffer[kWindowSize];
double			mTempCBuffer[kWindowSize];

int CenterCutProcessSamples(byte *inSamples, int inSampleCount, byte *outSamples, int bitsPerSample, int sampleRate);
void ConvertSamples(int type, byte *sampB, double *sampD, int sampleCount, int bitsPerSample, int chanCount);

void OutputBufferReadComplete();
bool OutputBufferBeginWrite();
bool BPSIsValid(int bitsPerSample);

bool CenterCut_Run();

}
