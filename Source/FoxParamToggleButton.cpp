/*
  ==============================================================================

    FoxParamToggleButton.cpp
    Created: 20 Jun 2025 8:41:59pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FoxParamToggleButton.h"

//==============================================================================
FoxParamToggleButton::FoxParamToggleButton(juce::AudioProcessorValueTreeState& inApvts,
                                           const juce::String& inParamId, const int inCmdId)
:
mAttachment(inApvts, inParamId, *this)
{
    setCommandToTrigger(nullptr, inCmdId, false);
    
    setClickingTogglesState(true); // 버튼의 토글기능 활성화 - 토글모드 온
    updateButtonText();
    
    //버튼 클릭 이벤트에 대한 콜백 함수 - 람다 함수
    onClick = [this]()
    {
        updateButtonText();
    };
}

FoxParamToggleButton::~FoxParamToggleButton()
{
}

void FoxParamToggleButton::updateButtonText() noexcept
{
    setButtonText(getToggleState()? "ON" : "OFF");
}
