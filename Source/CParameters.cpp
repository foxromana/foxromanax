/*
  ==============================================================================

    CParameters.cpp
    Created: 21 Feb 2025 8:34:17pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "CParameters.h"

CParameters::CParameters(juce::AudioProcessorValueTreeState& inApvts)
://Apvts
mApvts(inApvts)
{
    //RangedAudioParameter is always normalized ( 0 ~ 1 )..
    //getParamID() = string name "Gain" is returned
    mParamGain =  dynamic_cast<juce::AudioParameterFloat*>(mApvts.getParameter(MyParamIDs::Gain.getParamID()));
    //juce::RangedAudioParameter* gain = mApvts.getParameter(MyParamIDs::Gain.getParamID());

}

CParameters::~CParameters()
{
    
}

//Static! only defined once.
juce::AudioProcessorValueTreeState::ParameterLayout CParameters::initParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //std::make_unique - smart pointer, not used? then removed in the memory automatically
    
    //add Gain parameter information
    layout.add(std::make_unique<juce::AudioParameterFloat>(MyParamIDs::Gain, //type of parameter
                                                           "Gain", // parameter name
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.05f),// range: 0~1, move unit 0.1
                                                           0.5f)); // default 0.5
    
    // juce::audioparameter ( RangedAudioParamter is parent. it's always range from 0 to 1 )
    // - int : ..?
    // - bool : mute button (toggle)
    // - float : processing
    // - choice : mode selection
    
    return layout;
    
}

float CParameters::getValueGain() const noexcept
{
    return mParamGain->get();
}
