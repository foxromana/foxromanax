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
#include "FoxHelper.h"

//==============================================================================
PanelDelay::PanelDelay(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor, juce::Button& inButtonTempo)
:
PanelBase(inTitle, inAudioProcessor),
mKnobTime { {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Time[0].getParamID()},
            {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Time[1].getParamID()} },
mKnobNote { {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Note[0].getParamID()},
            {mAudioProcessor.getApvts(), FoxParamIDs::Delay::Note[1].getParamID()} },
//button
mButtonTempo(inButtonTempo)
{
    for( int i =0; i<2; ++i)
    {
        const juce::String titleTime = (i==0)? "Time L" : "Time R";
        mLabelTime[i].setText(titleTime,juce::NotificationType::dontSendNotification);
        mLabelTime[i].setJustificationType(juce::Justification::centred);
        
        addChildComponent(mLabelTime[i]);
        addChildComponent(mKnobTime[i]);
        
        const juce::String titleNote = (i == 0) ? "Note L" : "Note R";
        mLabelNote[i].setText(titleNote, juce::NotificationType::dontSendNotification);
        mLabelNote[i].setJustificationType(juce::Justification::centred);
        
        addChildComponent(mLabelNote[i]);
        addChildComponent(mKnobNote[i]);
        
    }
    setVisibleLabelsAndKnobs(mButtonTempo.getToggleState());
    mButtonTempo.addListener(this); //panal delay에 tempo 버튼 리스너 로 등록
}

PanelDelay::~PanelDelay()
{
    //리스너 제거
    mButtonTempo.removeListener(this);
}

void PanelDelay::resized()
{
    //크기
    for(int i=0 ; i<2 ; ++i)
    {
        //Child components size
        mLabelTime[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
        mKnobTime[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
        
        mLabelNote[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
        mKnobNote[i].setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
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
        
        mLabelNote[i].setTopLeftPosition(mLabelTime[i].getPosition());
        mKnobNote[i].setTopLeftPosition(mKnobTime[i].getPosition());
    }
}

//리스너로 인해 이 함수 호출
void PanelDelay::buttonClicked(juce::Button* inButton)
{
    //버튼 고유 번호 식별. 어느 버튼에 대한 리스터가 호출?
    switch (inButton->getCommandID())
    {
        case FoxHelper::CmdIdButtons::Tempo :
            {
                setVisibleLabelsAndKnobs(inButton->getToggleState());
                break;
            }
        default:
            break;
    }
}

void PanelDelay::setVisibleLabelsAndKnobs(const bool inTempo) noexcept
{
    for(int i=0 ; i<2 ; ++i)
    {
        mLabelTime[i].setVisible(!inTempo);
        mKnobTime[i].setVisible(!inTempo);
        
        mLabelNote[i].setVisible(inTempo);
        mKnobNote[i].setVisible(inTempo);
    }
    
    /*
    if (inTempo == true)
    {
        for(int i=0 ; i<2 ; ++i)
        {
            mLabelTime[i].setVisible(false);
            mKnobTime[i].setVisible(false);
            
            mLabelNote[i].setVisible(true);
            mKnobNote[i].setVisible(true);
        }
    }
    else
    {
        for(int i=0 ; i<2 ; ++i)
        {
            mLabelTime[i].setVisible(true);
            mKnobTime[i].setVisible(true);
            
            mLabelNote[i].setVisible(false);
            mKnobNote[i].setVisible(false);
        }
    }
     */
    
    
    
}
