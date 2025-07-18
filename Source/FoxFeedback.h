/*
  ==============================================================================

    FoxFeedback.h
    Created: 18 Apr 2025 8:09:26pm
    Author:  juyeon hong

  ==============================================================================
*/
// delay line은 원형버퍼를 썼지만
// feedback 은 바로 직전의 wet 신호 (feedback에서 pop한) 1개 온리만 필요
// 이건 다음 신호의 인풋으로 꺼내져야 함.
// 각 채널(L, R) 당 FoxFeedback 오브젝트 한개씩 가짐
#pragma once

#include <JuceHeader.h>

class FoxFeedback
{
    public:
    FoxFeedback();
    ~FoxFeedback();
    
    void prepare() noexcept;
    void reset() noexcept;
    
    //inAmount -> 0~100% 볼륨 얼마나 작아질지
    void pushSample(const float inSample, const float inAmount = 1.0f) noexcept;
    float popSample() const noexcept;
    
    private:
        //push 된 wet
        float mSampleFeedback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxFeedback)
};
