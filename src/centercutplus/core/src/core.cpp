#include "core.h"

namespace ccp
{

void Lock(bool bRunning) {
	if (bRunning) {
		WaitForSingleObject(hMutexRunning, INFINITE);
	}
	else {
		ReleaseMutex(hMutexRunning);
	}
}


int Init_CenterCut(struct winampDSPModule *thisModule) {
	Lock(true);
	OutputBufferInit();
	CenterCut_Start();
	ConfigWindow::CreateConfigWindow(mDllHandle, thisModule->hwndParent);
	mInitialized = true;
	Lock(false);
	return 0;
}

void Quit_CenterCut(struct winampDSPModule *thisModule) {
	Lock(true);
	CenterCut_Finish();
	OutputBufferFree();
	ConfigWindow::DestroyConfigWindow();
	mInitialized = false;
	DelayDLLUnload(thisModule->hDllInstance);
	Lock(false);
}

void About(struct winampDSPModule *thisModule) {
	ConfigWindow::ShowAboutBox(thisModule->hwndParent);
}


int ModifySamples(struct winampDSPModule *thisModule, uint8 *samples, int sampleCount, int bitsPerSample, int chanCount,
				  int sampleRate)
{
	Lock(true);
	if ((chanCount == 2) && (sampleCount > 0) && BPSIsValid(bitsPerSample) && mInitialized && !Config::IsBypassed()) {
		int outSampleCount = CenterCutProcessSamples(samples, sampleCount, samples, bitsPerSample, sampleRate);

		if (outSampleCount >= 0) {
			sampleCount = outSampleCount;
		}
	}
	Lock(false);

	return sampleCount;
}

int CenterCutProcessSamples(uint8 *inSamples, int inSampleCount, uint8 *outSamples, int bitsPerSample, int sampleRate) {
	int bytesPerSample, outSampleCount, maxOutSampleCount, copyCount;

	mSampleRate = sampleRate;
	bytesPerSample = bitsPerSample / 8;
	outSampleCount = 0;
	maxOutSampleCount = inSampleCount;

	while (inSampleCount > 0) {
		copyCount = min(static_cast<int>(mInputSamplesNeeded), inSampleCount);

		ConvertSamples(BYTES_TO_DOUBLE, inSamples, &mInput[mInputPos][0], copyCount, bitsPerSample, 2);

		inSamples += copyCount * bytesPerSample * 2;
		inSampleCount -= copyCount;
		mInputPos = (mInputPos + copyCount) & (kWindowSize-1);
		mInputSamplesNeeded -= copyCount;

		if (mInputSamplesNeeded == 0) {
			CenterCut_Run();
		}
	}

	while ((mOutputBufferCount > 0) && (outSampleCount < maxOutSampleCount)) {
		double *sampD = mOutputBuffer[0];
		if (!sampD) return -1;

		copyCount = min(mOutputSampleCount - mOutputReadSampleOffset,
			maxOutSampleCount - outSampleCount);

		ConvertSamples(DOUBLE_TO_BYTES, outSamples, sampD + (mOutputReadSampleOffset * 2), copyCount, bitsPerSample, 2);

		outSamples += copyCount * bytesPerSample * 2;
		outSampleCount += copyCount;
		mOutputReadSampleOffset += copyCount;
		if (mOutputReadSampleOffset == mOutputSampleCount) {
			OutputBufferReadComplete();
		}
	}

	return outSampleCount;
}

void ConvertSamples(int type, uint8 *sampB, double *sampD, int sampleCount, int bitsPerSample, int chanCount) {
	const double SampleScaleInv = 32768.0;
	const double SampleScale = 1.0 / SampleScaleInv;
	const double SampleMin = -2147483648.0;
	const double SampleMax = 2147483647.0;

	int bytesPerSample, shiftCount;
	sint32 xor;
	uint8 *max;

	bytesPerSample = (bitsPerSample + 7) / 8;
	shiftCount = (4 - bytesPerSample) * 8;
	xor = (bytesPerSample == 1) ? (1 << 31) : 0;
	max = sampB + (sampleCount * bytesPerSample * chanCount);

	if (type == BYTES_TO_DOUBLE) {
		sint32 tempI;

		while (sampB < max) {
			tempI = (*((sint32*)sampB) << shiftCount) ^ xor;
			*sampD = static_cast<double>(tempI) * SampleScale;

			sampB += bytesPerSample;
			sampD += 1;
		}
	}
	else {
		uint8 *maxw = max - 3;
		double tempD;
		uint32 tempI;

		while (sampB < max) {
			tempD = *sampD * SampleScaleInv;
			if (tempD > 0.0) {
				if (tempD > SampleMax) {
					tempD = SampleMax;
				}
				tempD += 0.5;
			}
			else {
				if (tempD < SampleMin) {
					tempD = SampleMin;
				}
				tempD -= 0.5;
			}
			tempI = static_cast<uint32>(static_cast<sint32>(tempD) ^ xor) >> shiftCount;

			if (sampB < maxw) {
				*(reinterpret_cast<uint32*>(sampB)) = tempI;
			}
			else {
				memcpy(sampB, &tempI, bytesPerSample);
			}

			sampB += bytesPerSample;
			sampD += 1;
		}
	}
}

void OutputBufferInit() {
	for (int i = 0; i < mOutputMaxBuffers; i++) {
		mOutputBuffer[i] = 0;
	}
	mOutputBufferCount = 0;
	mOutputReadSampleOffset = 0;
}

void OutputBufferFree() {
	for (int i = 0; i < mOutputMaxBuffers; i++) {
		if (mOutputBuffer[i]) {
			delete[] mOutputBuffer[i];
			mOutputBuffer[i] = 0;
		}
	}
}

void OutputBufferReadComplete() {
	mOutputBufferCount--;
	mOutputReadSampleOffset = 0;
	if (mOutputBufferCount > 0) {
		int i;
		double *moveToEnd = mOutputBuffer[0];

		// Shift the buffers so that the current one for reading is at index 0
		for (i = 1; i < mOutputMaxBuffers; i++) {
			mOutputBuffer[i - 1] = mOutputBuffer[i];
		}
		mOutputBuffer[mOutputMaxBuffers - 1] = 0;

		// Move the previous first buffer to the end (first null pointer)
		for (i = 0; i < mOutputMaxBuffers; i++) {
			if (!mOutputBuffer[i]) {
				mOutputBuffer[i] = moveToEnd;
				break;
			}
		}
	}
}

bool OutputBufferBeginWrite() {
	if (mOutputBufferCount == mOutputMaxBuffers) {
		return false;
	}

	int i = mOutputBufferCount;
	if (!mOutputBuffer[i]) {
		// No buffer exists at this index, make a new one
		mOutputBuffer[i] = new double[mOutputSampleCount * 2];
		if (!mOutputBuffer[i]) {
			return false;
		}
	}

	mOutputBufferCount++;
	return true;
}

bool BPSIsValid(int bitsPerSample) {
	// Bits per sample must be between 8 and 32 bits, and a multiple of 8
	return (bitsPerSample >= 8) && (bitsPerSample <= 32) && ((bitsPerSample & 7) == 0);
}



bool CenterCut_Start() {
	VDCreateBitRevTable(mBitRev, kWindowSize);
	VDCreateHalfSineTable(mSineTab, kWindowSize);

	mInputSamplesNeeded = kOverlapSize;
	mInputPos = 0;

	mOutputDiscardBlocks = kOverlapCount - 1;

	memset(mInput, 0, sizeof mInput);
	memset(mOverlapC, 0, sizeof mOverlapC);

	std::vector<double> tmpBuffer(kWindowSize);
	double *tmp = &(tmpBuffer[0]);
	if (!tmp) return false;
	VDCreateRaisedCosineWindow(tmp, kWindowSize, 1.0);
	for(unsigned i=0; i<kWindowSize; ++i) {
		// The correct Hartley<->FFT conversion is:
		//
		//	Fr(i) = 0.5(Hr(i) + Hi(i))
		//	Fi(i) = 0.5(Hr(i) - Hi(i))
		//
		// We omit the 0.5 in both the forward and reverse directions,
		// so we have a 0.25 to put here.

		mPreWindow[i] = tmp[mBitRev[i]] * 0.5 * (2.0 / static_cast<double>(kOverlapCount));
	}

	CreatePostWindow(mPostWindow, kWindowSize, kPostWindowPower);

	return true;
}

void CenterCut_Finish() {
}

bool CenterCut_Run()
{
	int i;

	// copy to temporary buffer and FHT
	for(i=0; i<kWindowSize; ++i) {
		const unsigned j = mBitRev[i];
		const unsigned k = (j + mInputPos) & (kWindowSize-1);
		const double w = mPreWindow[i];

		mTempLBuffer[i] = mInput[k][0] * w;
		mTempRBuffer[i] = mInput[k][1] * w;
	}

	VDComputeFHT(mTempLBuffer, kWindowSize, mSineTab);
	VDComputeFHT(mTempRBuffer, kWindowSize, mSineTab);

	// read configuration from UI
	Preset preset = Config::GetPreset();
	int dividerFreq = static_cast<int>(
		(PositionHelper::GetDividerFreq(preset.FreqSlider)/(static_cast<double>(mSampleRate) / kWindowSize)) + 0.5);

	// perform stereo separation
	mTempCBuffer[0] = 0;
	mTempCBuffer[1] = 0;
	for(i = 1; i < kHalfWindow; i++)
	{
		// itt lehet a hiba!!
		//bool keepCurrentInCenter = !preset.BassToSides || (i >= freqBelowToSides);
		bool keepCurrentInCenter = preset.CenterModeSetting == CenterMode::None
									|| preset.CenterModeSetting == CenterMode::LowToSides && (i >= dividerFreq)
									|| preset.CenterModeSetting == CenterMode::HighToSides && (i < dividerFreq);
		double cR = 0.0;
		double cI = 0.0;

		if(keepCurrentInCenter)
		{
			double lR = mTempLBuffer[i] + mTempLBuffer[kWindowSize-i];
			double lI = mTempLBuffer[i] - mTempLBuffer[kWindowSize-i];
			double rR = mTempRBuffer[i] + mTempRBuffer[kWindowSize-i];
			double rI = mTempRBuffer[i] - mTempRBuffer[kWindowSize-i];

			double sumR = lR + rR;
			double sumI = lI + rI;
			double diffR = lR - rR;
			double diffI = lI - rI;

			double sumSq = sumR*sumR + sumI*sumI;
			double diffSq = diffR*diffR + diffI*diffI;
			double alpha = 0.0;

			if (sumSq > nodivbyzero) {
				alpha = 0.5 - sqrt(diffSq / sumSq) * 0.5;
			}

			cR = sumR * alpha;
			cI = sumI * alpha;
		}

		mTempCBuffer[mBitRev[i            ]] = cR + cI;
		mTempCBuffer[mBitRev[kWindowSize-i]] = cR - cI;
	}

	// reconstitute left/right/center channels

	VDComputeFHT(mTempCBuffer, kWindowSize, mSineTab);

	// apply post-window

	for (i=0; i<kWindowSize; i++) {
		mTempCBuffer[i] *= mPostWindow[i];
	}

	// writeout


	if (mOutputDiscardBlocks > 0) {
		mOutputDiscardBlocks--;
	}
	else {
		int currentBlockIndex, nextBlockIndex, blockOffset;

		if (!OutputBufferBeginWrite()) return false;
		double *outBuffer = mOutputBuffer[mOutputBufferCount - 1];
		if (!outBuffer) return false;

		for(i=0; i<kOverlapSize; ++i) {
			double c = mOverlapC[0][i] + mTempCBuffer[i];
			double l = mInput[mInputPos+i][0] - c;
			double r = mInput[mInputPos+i][1] - c;

			PositionHelper positionHelper(preset, l, r, c);
			outBuffer[0] = positionHelper.L();
			outBuffer[1] = positionHelper.R();
			outBuffer += 2;

			// overlapping

			currentBlockIndex = 0;
			nextBlockIndex = 1;
			blockOffset = kOverlapSize;
			while (nextBlockIndex < kOverlapCount - 1) {
				mOverlapC[currentBlockIndex][i] = mOverlapC[nextBlockIndex][i] +
					mTempCBuffer[blockOffset + i];
				
				currentBlockIndex++;
				nextBlockIndex++;
				blockOffset += kOverlapSize;
			}
			mOverlapC[currentBlockIndex][i] = mTempCBuffer[blockOffset + i];
		}
	}

	mInputSamplesNeeded = kOverlapSize;

	return true;
}

}
