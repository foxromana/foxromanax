/*
  ==============================================================================

    PanelControl.cpp
    Created: 2 May 2025 8:43:43pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanelControl.h"
#include "FoxSize.h"

//==============================================================================
PanelControl::PanelControl(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
: //Base clasee
 PanelBase(inTitle, inAudioProcessor)
{
    mLabelPreset.setText("Preset", juce::NotificationType::dontSendNotification);
    mLabelPreset.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelPreset);
    addAndMakeVisible(mPresetComboBox);
}

PanelControl::~PanelControl()
{
}


void PanelControl::resized()
{
    //Set Size
    mLabelPreset.setSize(FoxSize::PresetComboBox::Width, FoxSize::Label::Height);
    mPresetComboBox.setSize(FoxSize::PresetComboBox::Width, FoxSize::PresetComboBox::Height);
    //Position
    const juce::Rectangle<int> boundLocal = getLocalBounds();
    
    const int xLabelPreset = boundLocal.getX() + FoxSize::Padding::Width;
    const int yLabelPreset = boundLocal.getY() + FoxSize::Padding::Top::Height;
    mLabelPreset.setTopLeftPosition(xLabelPreset, yLabelPreset);
    mPresetComboBox.setTopLeftPosition(mLabelPreset.getX(), mLabelPreset.getBottom());
}
