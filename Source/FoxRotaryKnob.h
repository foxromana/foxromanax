/*
  ==============================================================================

    FoxRotaryKnob.h
    Created: 4 Apr 2025 8:54:28pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FoxRotaryKnob  : public juce::Slider
{
public:
    FoxRotaryKnob(juce::AudioProcessorValueTreeState& inApvts,
                  const juce::String& inParamID,
                  const bool inDrawFromMiddle = false);
    //inDrawFromMiddle 가 true 인 경우는 gain
    //gain 의 경우, 분홍선의 기준이 맨왼쪽이 아니라 중앙이다 (0.0dB) - 특별한 경우
    //분홍선 채우는 방법이 달라야 함, 0.0db 위치부터 분홍선을 채움 (inDrawFromMiddle)
    
    ~FoxRotaryKnob() override;

private:
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxRotaryKnob)
};
