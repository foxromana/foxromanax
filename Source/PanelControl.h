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
#include "FoxParamToggleButton.h"
//==============================================================================
/*
*/
class PanelControl  : public PanelBase
{
public:
    PanelControl(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
    ~PanelControl() override;

    void resized() override;

    //외부(toggle Button)에서 템포 버튼 갖다 쓸 수 있게
    juce::Button& getButtonTempo() noexcept;
    
private:
    juce::Label mLabelPreset;
    FoxPresetComboBox mPresetComboBox;
    
    juce::Label mLabelTempo;
    FoxParamToggleButton mButtonTempo;
    
    //좌우 싱크 용 버튼
    juce::Label mLabelLink;
    FoxParamToggleButton mButtonLink;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelControl)
};
