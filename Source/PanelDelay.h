/*
  ==============================================================================

    PanelDelay.h
    Created: 11 Apr 2025 8:13:18pm
    Author:  juyeon hong
    Slider: Time L/R
 
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PanelBase.h"
#include "FoxRotaryKnob.h"

//==============================================================================
/*
*/
class PanelDelay  : public PanelBase
{
    public:
        PanelDelay(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
        ~PanelDelay() override;

        //slider and other component's position will be decided here
        void resized() override;

    private:
        juce::Label mLabelTime[2]; // Time for L/R
        FoxRotaryKnob mKnobTime[2]; // Time for L/R
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelDelay)
};
