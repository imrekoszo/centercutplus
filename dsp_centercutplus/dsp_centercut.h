#pragma once

bool			mInitialized = false;
HANDLE			hMutexRunning = 0;
HINSTANCE		hDummy;

const int		kWindowSize = 8192;
const int		kOverlapCount = 4;
const int		kPostWindowPower = 2;  // Maximum power including pre-window is kOverlapCount-1,
									   // which means this can be kOverlapCount-2 at most
const int		kHalfWindow = kWindowSize / 2;
const int		kOverlapSize = kWindowSize / kOverlapCount;

const double	twopi = 6.283185307179586476925286766559;
const double	invsqrt2 = 0.70710678118654752440084436210485;
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
uint32			mInputSamplesNeeded;
uint32			mInputPos;
unsigned		mBitRev[kWindowSize];
double			mPreWindow[kWindowSize];
double			mPostWindow[kWindowSize];
double			mSineTab[kWindowSize];
double			mInput[kWindowSize][2];
double			mOverlapC[kOverlapCount-1][kOverlapSize];
double			mTempLBuffer[kWindowSize];
double			mTempRBuffer[kWindowSize];
double			mTempCBuffer[kWindowSize];

HINSTANCE		mDllHandle;

void Lock(bool bRunning);
winampDSPModule *GetModule(int which);
void About(struct winampDSPModule *thisModule);
int Init_CenterCut(struct winampDSPModule *thisModule);
int Init_Other(struct winampDSPModule *thisModule);
void Quit_CenterCut(struct winampDSPModule *thisModule);
void Quit_Other(struct winampDSPModule *thisModule);
void DelayDLLUnload(HMODULE hDllInstance);
/*int ModifySamples_Sides(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
int ModifySamples_Center(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
int ModifySamples_SidesBTS(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
int ModifySamples_CenterBTS(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);*/
int ModifySamples_Classic(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
int ModifySamples(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount, int sampleRate);
int CenterCutProcessSamples(uint8 *inSamples, int inSampleCount, uint8 *outSamples, int bitsPerSample, int sampleRate);
void ConvertSamples(int type, uint8 *sampB, double *sampD, int sampleCount, int bitsPerSample, int chanCount);
void OutputBufferInit();
void OutputBufferFree();
void OutputBufferReadComplete();
bool OutputBufferBeginWrite();
bool BPSIsValid(int bitsPerSample);
bool CenterCut_Start();
void CenterCut_Finish();
bool CenterCut_Run();


#if defined(MODE_CLASSIC)

#define MODE_0 modClassic
#define MODE_1 modUlti
#define TITLE "Center Cut Plus v10.24.0.0 Classic"

#else

#define MODE_0 modUlti
#define MODE_1 modClassic
#define TITLE "Center Cut Plus v10.24.0.0"

#endif

winampDSPHeader dspHeader = { DSP_HDRVER, TITLE, GetModule };

winampDSPModule modClassic = {
	"Classic Vocal Remover",
	NULL,
	NULL,
	About,
	Init_Other,
	ModifySamples_Classic,
	Quit_Other
};

winampDSPModule modUlti = {
	"Center Cut Plus",
	NULL,
	NULL,
	About,
	Init_CenterCut,
	ModifySamples,
	Quit_CenterCut
};
