/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//#include "FoxRotaryKnob.h"
#include "PanelDelay.h"
#include "PanelOutput.h"

//==============================================================================
/**
*/
class FoxRomanaXAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FoxRomanaXAudioProcessorEditor (FoxRomanaXAudioProcessor&);
    ~FoxRomanaXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FoxRomanaXAudioProcessor& audioProcessor;

    //Class 7 - slider 기초
    //juce::Slider mSliderTest;
    //juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSliderTest;
    
    //juce::Label mLabelGain;
    //FoxRotaryKnob mKnobGain;
    
    //Class 8 - Panel
    PanelDelay mPanelDelay;
    PanelOutput mPanelOutput; 
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxRomanaXAudioProcessorEditor)
};
