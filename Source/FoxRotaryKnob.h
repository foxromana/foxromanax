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
                  const juce::String& inParamID);
    ~FoxRotaryKnob() override;

private:
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxRotaryKnob)
};
