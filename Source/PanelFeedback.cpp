/*
  ==============================================================================

    PanelFeedback.cpp
    Created: 18 Apr 2025 9:02:35pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanelFeedback.h"
#include "FoxSize.h"

//==============================================================================
PanelFeedback::PanelFeedback(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
:
PanelBase(inTitle, inAudioProcessor),
mKnobAmount(mAudioProcessor.getApvts(), FoxParamIDs::Feedback::Amount.getParamID())
{
    mLabelAmount.setText("Amount", juce::NotificationType::dontSendNotification);
    mLabelAmount.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelAmount);
    addAndMakeVisible(mKnobAmount);
}

PanelFeedback::~PanelFeedback()
{
    
}

void PanelFeedback::resized()
{
    //Size
    mLabelAmount.setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
    mKnobAmount.setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
    
    //Position
    const juce::Rectangle<int> boundLocal = getLocalBounds();
    
    const int xLabelAmount = boundLocal.getX() + FoxSize::Padding::Width;
    const int yLabelAmount = boundLocal.getY() + FoxSize::Padding::Top::Height;
    mLabelAmount.setTopLeftPosition(xLabelAmount, yLabelAmount);
    mKnobAmount.setTopLeftPosition(mLabelAmount.getX(), mLabelAmount.getBottom());
    
}
