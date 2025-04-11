/*
  ==============================================================================

    PanelOutput.h
    Created: 11 Apr 2025 8:13:59pm
    Author:  juyeon hong
    Slider: Mix, output Gain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PanelBase.h"
#include "FoxRotaryKnob.h"

//==============================================================================
/*
*/
class PanelOutput  : public PanelBase
{
public:
    PanelOutput(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
    ~PanelOutput() override;
    void resized() override;

private:
    juce::Label mLabelMix;
    FoxRotaryKnob mKnobMix;
    
    juce::Label mLabelGain;
    FoxRotaryKnob mKnobGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelOutput)
};
