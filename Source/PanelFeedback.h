/*
  ==============================================================================

    PanelFeedback.h
    Created: 18 Apr 2025 9:02:35pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PanelBase.h"
#include "FoxRotaryKnob.h"

//==============================================================================
/*
*/
class PanelFeedback  : public PanelBase
{
public:
    PanelFeedback(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
    ~PanelFeedback() override;

    void resized() override;

private:
    juce::Label mLabelAmount;
    FoxRotaryKnob mKnobAmount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelFeedback)
};
