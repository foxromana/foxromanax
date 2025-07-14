/*
  ==============================================================================

    FoxRotaryKnob.cpp
    Created: 4 Apr 2025 8:54:28pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FoxRotaryKnob.h"
#include "FoxSize.h"

//==============================================================================
FoxRotaryKnob::FoxRotaryKnob(juce::AudioProcessorValueTreeState& inApvts,
                             const juce::String& inParamId, const bool inDrawFromMiddle)
: //bass class slider is initialized
Slider(inParamId),
//SliderAttachement
mAttachment(inApvts, inParamId, *this)
{
    
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false,
                    FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::TextBox::Height);
    
    setRotaryParameters(juce::degreesToRadians(225.0f), //로터리 시작 각도
                        juce::degreesToRadians(495.0f), //로터리 끝 각도
                        true);
    
    //custom property 추가: DrawFromMiddle - true/false. - 기본이 false 로 세팅
    //inDrawFromMiddle 값을 저장
    getProperties().set("drawFromMiddle", inDrawFromMiddle);

}

FoxRotaryKnob::~FoxRotaryKnob()
{
}

