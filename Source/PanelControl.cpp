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
#include "FoxHelper.h"

//==============================================================================
PanelControl::PanelControl(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
: //Base clasee
 PanelBase(inTitle, inAudioProcessor),
mPresetComboBox(mAudioProcessor.getPresetManager()),
mButtonTempo(mAudioProcessor.getApvts(), FoxParamIDs::Control::Tempo.getParamID(), FoxHelper::CmdIdButtons::Tempo),
mButtonLink(mAudioProcessor.getApvts(), FoxParamIDs::Control::Link.getParamID(), FoxHelper::CmdIdButtons::Link)
{
    mLabelPreset.setText("Preset", juce::NotificationType::dontSendNotification);
    mLabelPreset.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelPreset);
    addAndMakeVisible(mPresetComboBox);
    
    mLabelTempo.setText("Tempo", juce::NotificationType::dontSendNotification);
    mLabelTempo.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelTempo);
    addAndMakeVisible(mButtonTempo);
    
    mLabelLink.setText("Link", juce::NotificationType::dontSendNotification);
    mLabelLink.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLabelLink);
    addAndMakeVisible(mButtonLink);
}

PanelControl::~PanelControl()
{
}


void PanelControl::resized()
{
    //Set Size
    mLabelPreset.setSize(FoxSize::PresetComboBox::Width, FoxSize::Label::Height);
    mPresetComboBox.setSize(FoxSize::PresetComboBox::Width, FoxSize::PresetComboBox::Height);
    mLabelTempo.setSize(FoxSize::PresetComboBox::Width, FoxSize::Label::Height);
    mButtonTempo.setSize(FoxSize::ParamToggleButton::Width, FoxSize::ParamToggleButton::Height);

    mLabelLink.setSize(FoxSize::PresetComboBox::Width, FoxSize::Label::Height);
    mButtonLink.setSize(FoxSize::ParamToggleButton::Width, FoxSize::ParamToggleButton::Height);

    
    //Position
    const juce::Rectangle<int> boundLocal = getLocalBounds();
    
    const int xLabelPreset = boundLocal.getX() + FoxSize::Padding::Width;
    const int yLabelPreset = boundLocal.getY() + FoxSize::Padding::Top::Height;
    mLabelPreset.setTopLeftPosition(xLabelPreset, yLabelPreset);
    mPresetComboBox.setTopLeftPosition(mLabelPreset.getX(), mLabelPreset.getBottom());
    
    const int yLabelTempo = mPresetComboBox.getBottom() + FoxSize::Padding::Mid::Height;
    mLabelTempo.setTopLeftPosition(mLabelPreset.getX(), yLabelTempo);
    const int xButtonTempo = boundLocal.getCentreX() - (mButtonTempo.getWidth() / 2);
    mButtonTempo.setTopLeftPosition(xButtonTempo, mLabelTempo.getBottom());
    
    const int yLabelLink = mButtonTempo.getBottom() + FoxSize::Padding::Mid::Height;
    mLabelLink.setTopLeftPosition(mLabelPreset.getX(), yLabelLink);
    const int xButtonLink = boundLocal.getCentreX() - (mButtonLink.getWidth() / 2);
    mButtonLink.setTopLeftPosition(xButtonLink, mLabelLink.getBottom());
    
}

juce::Button& PanelControl::getButtonTempo() noexcept
{
    return mButtonTempo;
}
