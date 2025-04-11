/*
  ==============================================================================

    PanelDelay.cpp
    Created: 11 Apr 2025 8:13:18pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanelDelay.h"
#include "FoxSize.h"

//==============================================================================
PanelDelay::PanelDelay(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor)
:
PanelBase(inTitle, inAudioProcessor),
mKnobTime { {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Time[0].getParamID()},
            {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Time[1].getParamID()} }

{
    for( int i =0; i<2; ++i)
    {
        const juce::String titleTime = (i==0)? "Time L" : "Time R";
        mLabelTime[i].setText(titleTime,juce::NotificationType::dontSendNotification);
        mLabelTime[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(mLabelTime[i]);
        addAndMakeVisible(mKnobTime[i]);
    }
}

PanelDelay::~PanelDelay()
{
}

void PanelDelay::resized()
{
    //크기
    for(int i=0 ; i<2 ; ++i)
    {
        //Child components size
        mLabelTime[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
        mKnobTime[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
    }
    //위치
    //나의 크기와 위치 불러오기
    const juce::Rectangle<int> boundLocal = getLocalBounds();
    //이 안에서 라벨과 노브의 위치 정해주기
    for(int i=0 ; i<2 ; ++i)
    {
        const int xLabelTime = boundLocal.getX() + FoxSize::Padding::Width;
        const int yLabelTime = (i==0)? boundLocal.getY() + FoxSize::Padding::Top::Height :
                                        mKnobTime[0].getBottom() + FoxSize::Padding::Mid::Height;
        mLabelTime[i].setTopLeftPosition(xLabelTime, yLabelTime);
        mKnobTime[i].setTopLeftPosition(mLabelTime[i].getX(), mLabelTime[i].getBottom());
    }
}
