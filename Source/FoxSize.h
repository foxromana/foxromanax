/*
  ==============================================================================

    FoxSize.h
    Created: 4 Apr 2025 8:59:09pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

namespace FoxSize
{
    namespace Padding //여백
    {
        static constexpr int Width = 15;
        
        namespace Top
        {
            static constexpr int Height = 25;
        }
        
        namespace Mid
        {
            static constexpr int Height = 15;
        }
        
        namespace Bottom
        {
            static constexpr int Height = 20;
        }
    }
    
    namespace Label //이름표
    {
        static constexpr int Height = 20;
    }
    
    namespace RotaryKnob //동글 노브
    {
        static constexpr int Width = 75;
        
        namespace TextBox
        {
            static constexpr int Height = Label::Height;
        }
        static constexpr int Height = Width + TextBox::Height;
    }

    namespace PresetComboBox
    {
        static constexpr int Width = 85;
        static constexpr int Height = 23;
    }

    namespace ParamToggleButton
    {
        static constexpr int Width = 35;
        static constexpr int Height = 22;
    }
    namespace Panel
        {
            namespace Delay
            {
                static constexpr int Width = Padding::Width + RotaryKnob::Width + Padding::Width;
                static constexpr int Height = Padding::Top::Height +
                                              Label::Height +
                                              RotaryKnob::Height +
                                              Padding::Bottom::Height +
                                              Padding::Top::Height +
                                              Label::Height +
                                              RotaryKnob::Height +
                                              Padding::Bottom::Height;
            }
            
            namespace Output
            {
                static constexpr int Width = Padding::Width + RotaryKnob::Width + Padding::Width;
                static constexpr int Height = Delay::Height;
            }
    
            namespace Feedback
            {
                static constexpr int Width = Padding::Width + RotaryKnob::Width + Padding::Width;
                static constexpr int Height = Delay::Height;
            }
            
            namespace Control
            {
                static constexpr int Width = Padding::Width + PresetComboBox::Width + Padding::Width;
                static constexpr int Height = Delay::Height;
            }
        }
        
        namespace GUI
        {
            namespace Header
            {
                static constexpr int Height = 40;
            }
            
            static constexpr int Width = Panel::Control::Width + Panel::Delay::Width + Panel::Feedback::Width + Panel::Output::Width;
            static constexpr int Height = Header::Height + Panel::Delay::Height;
        }

        namespace Font
        {
            static constexpr float Height = 15.0f;
        }
}
