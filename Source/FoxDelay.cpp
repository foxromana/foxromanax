/*
  ==============================================================================

    FoxDelay.cpp
    Created: 7 Mar 2025 8:47:17pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "FoxDelay.h"
#include "FoxParameters.h"

FoxDelay::FoxDelay()
:mIndexWriting(0)
{
    
}

FoxDelay::~FoxDelay()
{
    
}

void FoxDelay::prepare(const double inSampleRate) noexcept
{
    //clear circular buffer
    mBuffer.clear();
    
    const double maxDelayTimeSec = FoxParameters::kTimeMax * 0.001 ; // ex) 2000ms -> 2s
    mSizeBuffer = (int)(inSampleRate * maxDelayTimeSec) + 1; // 44000 * 2 second = 88001 size!
    mBuffer.resize(mSizeBuffer); // update mBuffer size into 88001
    
    reset(); // all 0 in mBuffer
}

void FoxDelay::reset()noexcept
{
    mBuffer.fill(0.0f); //memset all init to 0
}

float FoxDelay::process(const float inSample, const float inDelay) noexcept
{
    //inSample : X(n) input one channel sound
    
    //output : out the sound N ms before [mIndexWriting]
    
    //write, push
    // mBuffer[mIndexWriting] = inSample
    mBuffer.set(mIndexWriting, inSample);
    
    //read, pop
    //딜레이 될 소리 구하기
    int indexReading = mIndexWriting - (int)inDelay;
    
    //원형버퍼 처리. 인덱스 처음이면 맨마지막으로 가서 딜레이 구하기
    if (indexReading <0)
    {
        indexReading += mSizeBuffer;
    }
    
    const float sampleDelayed = mBuffer[indexReading];
    
    //update
    //다음에 process 호출될때 다음 인덱스로
    ++mIndexWriting;
    
    //원형버퍼 처리. 인덱스 마지막이면 처음으로 돌리기
    if(mIndexWriting >= mSizeBuffer)
    {
        mIndexWriting = 0;
    }
    
    return sampleDelayed;
}


