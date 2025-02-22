/*
  ==============================================================================

    CParameters.h
    Created: 21 Feb 2025 8:34:17pm
    Author:  Fox Romana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace MyParamIDs
{
    // Gain : parameter name, 1: version ( logic pro requires version which is not 0 ! )
    const juce::ParameterID Gain("Gain", 1);
}

class CParameters
{
    public:
    CParameters(juce::AudioProcessorValueTreeState& inApvts);
    ~CParameters();
    
    float getValueGain() const noexcept; // no except : no exeption handling. because this function should be fast fast fast.
    
    static juce::AudioProcessorValueTreeState::ParameterLayout initParameterLayout();
    private:
        juce::AudioProcessorValueTreeState& mApvts;
        juce::AudioParameterFloat* mParamGain; // for avoiding casting / mapping all the time. do this job beforehand.
    
    //safety
    //if there is no user's constructor, system create temporary copied constructor inside.
    //this function doesn't allow creating the copied constructor.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CParameters);
};
