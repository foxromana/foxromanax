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
class PanelDelay  : public PanelBase,
                    private juce::Button::Listener
{
    public:
        PanelDelay(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor, juce::Button& inButtonTempo);
        ~PanelDelay() override;

        //slider and other component's position will be decided here
        void resized() override;

    private:
        juce::Label mLabelTime[2]; // Time for L/R
        FoxRotaryKnob mKnobTime[2]; // Time for L/R
    
        juce::Label mLabelNote[2];
        FoxRotaryKnob mKnobNote[2];
    
        juce::Button& mButtonTempo;
    
        //tempo on/off button listner
        void buttonClicked(juce::Button*) override;
        void setVisibleLabelsAndKnobs(const bool inTempo) noexcept;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelDelay)
};
