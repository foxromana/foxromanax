/*
  ==============================================================================

    PanelOutput.cpp
    Created: 11 Apr 2025 8:13:59pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanelOutput.h"
#include "FoxSize.h"
//==============================================================================
PanelOutput::PanelOutput(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
:
PanelBase(inTitle, inAudioProcessor),
//mAudioProcessor 는 부모 클래스의 protected 맴버
mKnobMix(mAudioProcessor.getApvts(), FoxParamIDs::Output::Mix.getParamID()),
mKnobGain(mAudioProcessor.getApvts(), FoxParamIDs::Output::Gain.getParamID(), true) //inDrawFromMiddle=true 분홍선 그리는 기준이 다름
{
    mLabelMix.setText("Mix", juce::NotificationType::dontSendNotification);
    mLabelMix.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelMix);
    addAndMakeVisible(mKnobMix);
    
    mLabelGain.setText("Gain", juce::NotificationType::dontSendNotification);
    mLabelGain.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelGain);
    addAndMakeVisible(mKnobGain);
}

PanelOutput::~PanelOutput()
{
}

void PanelOutput::resized()
{
    // Set Size //
    mLabelMix.setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
    mKnobMix.setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
    
    mLabelGain.setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
    mKnobGain.setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
    
    // Set Position //
    const juce::Rectangle<int> boundLocal = getLocalBounds();
    
    const int xLabelMix = boundLocal.getX() + FoxSize::Padding::Width;
    const int yLabelMix = boundLocal.getY() + FoxSize::Padding::Top::Height;
    mLabelMix.setTopLeftPosition(xLabelMix, yLabelMix);
    mKnobMix.setTopLeftPosition(mLabelMix.getX(), mLabelMix.getBottom());
    
    const int yLabelGain = mKnobMix.getBottom() + FoxSize::Padding::Mid::Height;
    mLabelGain.setTopLeftPosition(mKnobMix.getX(), yLabelGain);
    mKnobGain.setTopLeftPosition(mLabelGain.getX(), mLabelGain.getBottom());
}
