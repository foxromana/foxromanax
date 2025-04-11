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
                             const juce::String& inParamId)
: //bass class slider is initialized
Slider(inParamId),

//SliderAttachement
mAttachment(inApvts, inParamId, *this)
{
    
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false,
                    FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::TextBox::Height);
    
    setRotaryParameters(juce::degreesToRadians(225.0f),
                        juce::degreesToRadians(495.0f),
                        true);

}

FoxRotaryKnob::~FoxRotaryKnob()
{
}

