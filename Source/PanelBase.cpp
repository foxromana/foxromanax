/*
  ==============================================================================

    PanelBase.cpp
    Created: 11 Apr 2025 8:04:30pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanelBase.h"

//==============================================================================
PanelBase::PanelBase(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
//param1: component name, param2: and name's label
: juce::GroupComponent(inTitle, inTitle),
mAudioProcessor(inAudioProcessor)
{
    //가운데 박스 라벨(제목)이름 위치 지정
    setTextLabelPosition(juce::Justification::horizontallyCentred);
}

PanelBase::~PanelBase()
{
}
