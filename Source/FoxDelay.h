/*
  ==============================================================================

    FoxDelay.h
    Created: 7 Mar 2025 8:47:17pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FoxDelay
{
    public:
        FoxDelay();
        ~FoxDelay();
    
    void prepare(const double inSampleRate) noexcept;
    void reset() noexcept;
    
    void preProcess(const float inDelay) noexcept; 
    float process(const float inSample, const float inDelay) noexcept;
    
    private:
    //원형버퍼
    //similar to STL::vector
    juce::Array<float> mBuffer;
    int mSizeBuffer;
    
    int mIndexWriting; //where to store current data 
    
    //lesson 10 ? 5/2
    float popSample(const float inDelay) const noexcept; 
     
    //2차방정식 인터폴레이션
    //inIndexReading : B - X - A  일때, A 와 B 사이의 X의 위치(x축)
    float getSampleByLerp(const float inIndexReading) const noexcept;
    
    //4개 점 기준 허미트 인터폴레이션 더 부드러운 인터폴레이션
    float getSampleByHermite(const float inIndexReading) const noexcept;
    
    //딜레이 적용할때 점점 교차. 직선으로
    //cross fading
    float mDelayCurrentCf;
    float mFadeInCf;
    float mIncrementCf;
    float popSampleCrossFading(const float inDelayTarget) noexcept;
    
    
    //DUCKING. 곡선으로 딜레이 적용
    // 내려가는 값= 커런트
    // 타겟이 올라가면 - 올라가는 커브. 타겟이 내려가면 - 내려가는 커브 .
    // current = ( current * ( 1 - coefficient)) + (target * coefficient)
    // coefficient = 1 - std::exp(-1 / 시간)
    // 시간 : 타겟값이 63% 시점까지 가는 시간
    // current += ( target - current ) * coefficient
    float mDelayCurrentDk; //현재 딜레이 타임 값
    float mDelayTargetDk; // 새로 들어온 딜레이 타임 값 . 내려갈땐 0 올라갈땐 1
    float mFadeCurrentDk; // 0까지 아니면 1 까지 가기 위한 중간 값.
    float mFadeTargetDk; // 올라갈지 (1) or 내려갈지 (0) 지정해 주는 변수. 0~ 1 사이 값 온리.
    float mCoefficientDk;
    float mWaitDk;// wait time을 0~1 까지 쪼개서 기다릴건데, 0~1 사이 값 현재까지 얼마나 기다렸는지
    float mWaitIncrementDk; //x 축 값 얼만씩 더할것인지
    float popSampleDucking() noexcept;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxDelay)
};
