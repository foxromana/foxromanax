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
    
    //cross fading
    float mDelayCurrentCf;
    float mFadeInCf;
    float mIncrementCf;
    float popSampleCrossFading(const float inDelayTarget) noexcept;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxDelay)
};
