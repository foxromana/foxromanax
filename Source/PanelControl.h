/*
  ==============================================================================

    PanelControl.h
    Created: 2 May 2025 8:43:43pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PanelBase.h"
#include "FoxPresetComboBox.h"
//==============================================================================
/*
*/
class PanelControl  : public PanelBase
{
public:
    PanelControl(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
    ~PanelControl() override;

    void resized() override;

private:
    juce::Label mLabelPreset;
    FoxPresetComboBox mPresetComboBox; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelControl)
};
