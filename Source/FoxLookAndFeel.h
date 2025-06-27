/*
  ==============================================================================

    FoxLookAndFeel.h
    Created: 27 Jun 2025 9:08:46pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace FoxColors
{
    namespace Main
    {
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour header { 40, 40, 40 };
    }
    
    namespace Panel
    {
        const juce::Colour title { 120, 115, 110 };
        const juce::Colour outline { 215, 210, 205 };
    }
    
    namespace RotaryKnob
    {
        const juce::Colour trackBackground { 205, 200, 195 };
        const juce::Colour trackActive { 177, 101, 135 };
        const juce::Colour outline { 255, 250, 245 };
        const juce::Colour gradientTop { 250, 245, 240 };
        const juce::Colour gradientBottom { 240, 235, 230 };
        const juce::Colour dial { 100, 100, 100 };
        const juce::Colour dropShadow { 195, 190, 185 };
        const juce::Colour label { 80, 80, 80 };
        const juce::Colour textBoxBackground { 80, 80, 80 };
        const juce::Colour value { 240, 240, 240 };
        const juce::Colour caret { 255, 255, 255 };
        const juce::Colour selected { 0, 206, 209 }; // dark turquoise
    }
    
    namespace Button
    {
        const juce::Colour text { 80, 80, 80 };
        const juce::Colour textToggled { 40, 40, 40 };
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour backgroundToggled { 255, 250, 245 };
        const juce::Colour outline { 215, 210, 205 };
    }
    
    namespace ComboBox
    {
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour text { 80, 80, 80 };
        const juce::Colour outline { 169, 169, 169 };
        const juce::Colour arrow { 105, 105, 105 };
    }
    
    namespace PopupMenu
    {
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour text  { 80, 80, 80 };
        const juce::Colour backgroundHighlighted { 177, 101, 135 };
        const juce::Colour textHighlighted { 240, 240, 240 };
    }
    
    namespace LevelMeter
    {
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour tickLine { 200, 200, 200 };
        const juce::Colour tickLabel { 80, 80, 80 };
        const juce::Colour tooLoud { 226, 74, 81 };
        const juce::Colour levelOK { 50, 205, 50 }; // Lime green
    }
}

//==============================================================================
/*
 Juce 에서 제공하는 LookAndFeel (UI 레이아웃) 현시점 가장 최신버전 V4
*/
class FoxLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    FoxLookAndFeel();
    ~FoxLookAndFeel() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxLookAndFeel)
};
