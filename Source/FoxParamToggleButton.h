/*
  ==============================================================================

    FoxParamToggleButton.h
    Created: 20 Jun 2025 8:41:59pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FoxParamToggleButton  : public juce::TextButton
{
public:
    FoxParamToggleButton(juce::AudioProcessorValueTreeState& inApvts,
                         const juce::String& inParamId, const int inCmdId);
    ~FoxParamToggleButton() override;

private:
    juce::AudioProcessorValueTreeState::ButtonAttachment mAttachment;
    
    //on/off 버튼 눌렀을때 버튼의 텍스트 (true = "on", false = "off" 텍스트) 갱신
    void updateButtonText() noexcept;
    
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxParamToggleButton)
};
