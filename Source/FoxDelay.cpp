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

//sampleRate 44.1kHz = 44100 (1초당 44100개 샘플링)
//딜레이 타임 567 ms = 0.567s
//몇 샘플 전인가 계산 시 ->
//44100 * 0.567 = 25004.7 개의 인덱스만큼 전 샘플로 가야해.
//근데 소숫점이 발생했네. 인덱스는 int 니까
//25004.7 -> 25004 해야지 -> ,, 근사치로 가야지 -> 이 경우.. 오차가 생겨..
//인터폴레이션 해야해!
//노이즈가 쌓일 수 있다.

float FoxDelay::process(const float inSample, const float inDelay) noexcept
{
    //inDelay: 25004.7 번째 전의 샘플 전으로 가라! 근데 소숫점이 발생했을 때를 대비해 인터폴레이션을 하기 위해 float 로 이제 바꿔! 
    //inSample : X(n) input one channel sound
    
    //output : out the sound N ms before [mIndexWriting]
    
    //write, push
    // mBuffer[mIndexWriting] = inSample
    mBuffer.set(mIndexWriting, inSample);
    
    //lesson 10 5/2 --> popSample로 이 코드를 옮김
    //read, pop
    //딜레이 될 소리 구하기
    //int indexReading = mIndexWriting - (int)inDelay;
    
    //원형버퍼 처리. 인덱스 처음이면 맨마지막으로 가서 딜레이 구하기
    //if (indexReading <0)
    //{
    //    indexReading += mSizeBuffer;
    //}
    
    const float sampleDelayed = popSample(inDelay);
    
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

float FoxDelay::popSample(const float inDelay) const noexcept
{
    //read, pop
    //딜레이 될 소리 구하기
    //float 으로만 처리 - 리니어 인터폴레이션 할거라서
    float indexReading = (float)mIndexWriting - inDelay;
    //원형버퍼 처리. 인덱스 처음이면 맨마지막으로 가서 딜레이 구하기
    if (indexReading <0.0f)
    {
        indexReading += (float)mSizeBuffer;
    }
    
    //선형인터폴레이션 적용 안할때
    //return mBuffer[(int)indexReading];
    
    //2차방정식 인터폴레이션 적용 할 때
    //return getSampleByLerp(indexReading);
    
    //허미트 인터폴레이션 적용할 때
    return getSampleByHermite(indexReading); 
    
    
}

float FoxDelay::getSampleByLerp(const float inIndexReading) const noexcept
{   //v1 - x - v0
    //B -  X  - A
    //4 - 3.7 - 3
    //x의 앞뒤 인덱스를 먼저 찾기
    const int indexA = (int)inIndexReading; // 3.7 --> 3
    const float fraction = inIndexReading - (float)indexA; //소수점 부분만 갖기 = 3.7 - 3 = 0.7
    int indexB = indexA + 1; // 4
    //mBuffer가 원형버퍼라서, 마지막 사이즈 넘어가면 안돼니까 원형버퍼 밖을 넘어가는지 확인해야해
    if (indexB >= mSizeBuffer)
    {
        //버퍼 넘어가면 원형이니까 0 로 가기
        indexB = 0;
    }
    

    //선형 2차 방정식 용 인터폴레이션
    //return (1.0f - fraction) * mBuffer[indexA] + fraction * mBuffer[indexB];
    //식을 간소화
    return mBuffer[indexA] + (fraction * (mBuffer[indexB] - mBuffer[indexA]));
    
    
}

float FoxDelay::getSampleByHermite(const float inIndexReading) const noexcept
{
    //ABCD 포인트 4개 구하기
    //한개 다음, 두개 전 인덱스를 원형버퍼에서 구하기
    const int iB = (int)inIndexReading;
    
    int iA = iB + 1;
    if (iA >= mSizeBuffer)
    {
        iA = 0;
    }
    
    int iC = iB - 1;
    int iD = iC - 1;
    
    if (iD < 0)
    {
        iD += mSizeBuffer;
        if (iC < 0)
        {
            iC += mSizeBuffer;
        }
    }
    
    const float fraction = inIndexReading - (float)iB;
    const float slope0 = (mBuffer[iC] - mBuffer[iA]) * 0.5f;
    const float slope1 = (mBuffer[iD] - mBuffer[iB]) * 0.5f;
    const float v = mBuffer[iB] - mBuffer[iC];
    const float w = slope0 + v;
    const float a = w + v + slope1;
    const float b = w + a;
    const float stage1 = (a * fraction) - b;
    const float stage2 = (stage1 * fraction) + slope0;
    return ((stage2 * fraction) + mBuffer[iB]);
}
