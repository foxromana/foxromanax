/*
  ==============================================================================

    FoxLookAndFeel.cpp
    Created: 27 Jun 2025 9:08:46pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FoxLookAndFeel.h"

//==============================================================================
FoxLookAndFeel::FoxLookAndFeel()
{
    //배경색
    setColour(juce::ResizableWindow::backgroundColourId, FoxColors::Main::background);
    
    //group component
    //그룹 제목 글자
    setColour(juce::GroupComponent::textColourId, FoxColors::Panel::title);
    //그룹 박스 선
    setColour(juce::GroupComponent::outlineColourId, FoxColors::Panel::outline);
    
    //버튼 색
    setColour(juce::TextButton::textColourOffId, FoxColors::Button::text);
    setColour(juce::TextButton::textColourOnId, FoxColors::Button::textToggled);
    setColour(juce::TextButton::buttonColourId, FoxColors::Button::background);
    setColour(juce::TextButton::buttonOnColourId, FoxColors::Button::backgroundToggled);
    
    //콤보박스 색
    setColour(juce::ComboBox::backgroundColourId, FoxColors::ComboBox::background);
    setColour(juce::ComboBox::textColourId, FoxColors::ComboBox::text);
    setColour(juce::ComboBox::outlineColourId, FoxColors::ComboBox::outline);
    setColour(juce::ComboBox::arrowColourId, FoxColors::ComboBox::arrow);
    
    //콤보 박스 열었을 때 뜨는 팝업메뉴 색
    setColour(juce::PopupMenu::backgroundColourId, FoxColors::PopupMenu::background);
    setColour(juce::PopupMenu::textColourId, FoxColors::PopupMenu::text);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, FoxColors::PopupMenu::backgroundHighlighted);
    setColour(juce::PopupMenu::highlightedTextColourId, FoxColors::PopupMenu::textHighlighted);
}

FoxLookAndFeel::~FoxLookAndFeel()
{
}
