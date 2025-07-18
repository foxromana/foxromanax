/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FoxSize.h"

//component hierarchy
// GUI 는 다 component
//최상위 계층 : plugineditor
//상위 계층 컴포넌트에서는 subcomponents (차일드 컴포넌트) "추가"(=등록)를 한다

//==============================================================================
FoxRomanaXAudioProcessorEditor::FoxRomanaXAudioProcessorEditor (FoxRomanaXAudioProcessor& p)
: AudioProcessorEditor (&p),
audioProcessor (p),
//class 7 basic
//mAttachmentSliderTest(audioProcessor.getApvts(),FoxParamIDs::Test.getParamID(), mSliderTest)
//class 7 advanced with FoxRotary Knob
//mKnobGain(audioProcessor.getApvts(), FoxParamIDs::Output::Gain.getParamID())
//class 10 control panel added
mPanelControl("Control", audioProcessor),
mPanelDelay("Delay", audioProcessor, mPanelControl.getButtonTempo()),
mPanelOutput("Output", audioProcessor),
//class 9 feedback panel added
mPanelFeedback("Feedback", audioProcessor)
{
    //class 7
    //setSize (700, 500);
    
    //class 8
    setSize(FoxSize::GUI::Width, FoxSize::GUI::Height);
    
    //Class 7-1 - basic
    //Slider style : node, mouse movement
    //mSliderTest.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    //Slider
    //addAndMakeVisible(mSliderTest); //플러그인 에디터 컴포넌트에 슬라이더 컴포넌트 등록
    
    //Class 7-2 - Advanced with Fox Rotary Knob
    //addAndMakeVisible(mKnobGain);
    //Label
    //mLabelGain.setText("Gain", juce::NotificationType::dontSendNotification);
    //mLabelGain.setJustificationType(juce::Justification::centred); //center
    //Attach Label
    //addAndMakeVisible(mLabelGain);
    
    //Class 10
    addAndMakeVisible(mPanelControl);
    
    //Class 8
    addAndMakeVisible(mPanelDelay);
    addAndMakeVisible(mPanelOutput);
    
    //Class 9 - Feedback panel
    addAndMakeVisible(mPanelFeedback);
    
    //맨 마지막에 호출
    //위의 add 된 그룹 컴포넌트들이 아래 lookandfeel에 적용되도록
    setLookAndFeel(&mLookAndFeel);
}

FoxRomanaXAudioProcessorEditor::~FoxRomanaXAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void FoxRomanaXAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("I am Fox Romana X!", getLocalBounds(), juce::Justification::centred, 1);
}

void FoxRomanaXAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //Class 7-1 basic
    //전체 슬라이더 크기
    //mSliderTest.setSize(100, 100);
    
    //슬라이더 텍스트박스 위치, 사이즈
    //false : read/write,
    //true: read only
    //mSliderTest.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow,false,100,20);
    
    //위치
    //mSliderTest.setTopLeftPosition(0,0);
    
    //Class 7-2 Advanced with Fox Rotary
    //크기
    //mLabelGain.setSize(FoxSize::RotaryKnob::Width, FoxSize::Label::Height);
    //mKnobGain.setSize(FoxSize::RotaryKnob::Width, FoxSize::RotaryKnob::Height);
    //위치
    //mLabelGain.setTopLeftPosition(0,0);
    //mKnobGain.setTopLeftPosition(mLabelGain.getX(), mLabelGain.getBottom());
        
    //Class 8
    //크기
    mPanelControl.setSize(FoxSize::Panel::Control::Width, FoxSize::Panel::Control::Height);
    mPanelDelay.setSize(FoxSize::Panel::Delay::Width, FoxSize::Panel::Delay::Height);
    mPanelOutput.setSize(FoxSize::Panel::Output::Width, FoxSize::Panel::Output::Height);
    mPanelFeedback.setSize(FoxSize::Panel::Feedback::Width, FoxSize::Panel::Feedback::Height);
    
    //위치 - 헤더 높이 만큼 밑으로 내려
    mPanelControl.setTopLeftPosition(0,FoxSize::GUI::Header::Height);
    mPanelDelay.setTopLeftPosition(mPanelControl.getRight(),mPanelControl.getY());
    mPanelFeedback.setTopLeftPosition(mPanelDelay.getRight(), mPanelDelay.getY());
    mPanelOutput.setTopLeftPosition(mPanelFeedback.getRight(),mPanelDelay.getY());
}
