/*
  ==============================================================================

    PanelBase.h
    Created: 11 Apr 2025 8:04:30pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
 juce::GroupComponent : default로 둥근 사각형
*/
class PanelBase  : public juce::GroupComponent
{
    public:
        //inTitle : Panel's name
        //inAudioProcessor : my audio processor object address
        PanelBase(const juce::String& inTitle, FoxRomanaXAudioProcessor& inAudioProcessor);
        ~PanelBase() override;

    protected: //상속한 애들만 쓸 수 있는 맴버변수
        FoxRomanaXAudioProcessor& mAudioProcessor;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelBase)
};
