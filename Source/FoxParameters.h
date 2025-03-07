/*
  ==============================================================================

    FoxParameters.h
    Created: 21 Feb 2025 8:34:17pm
    Author:  Fox Romana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace FoxParamIDs
{
    const juce::ParameterID Test("Test", 1);

    namespace Output
    {
        // Gain : parameter name, 1: version ( logic pro requires version which is not 0 ! )
        const juce::ParameterID Gain("Gain", 1);
    }
    namespace Delay
    {
    const juce::ParameterID Time[2] {{"TimeL", 1}, {"TimeR", 1}};
    }
}
class FoxParameters
{
    public:
    FoxParameters(juce::AudioProcessorValueTreeState& inApvts);
    ~FoxParameters();
    
    //for smoothe changing 
    void prepare(const double inSampleRate) noexcept;
    void smoothen() noexcept; 
    void update() noexcept;//normalize dB -> ratio ( -1 ~ 1 ) initialize Target value
    void reset() noexcept; //Initialize Current value
    
    float getValueGain() const noexcept; // no except : no exeption handling. because this function should be fast fast fast.
    float getValueTest() const noexcept;
    
    static constexpr float kTimeMin = 5.0f;
    static constexpr float kTimeMax = 2000.0f;
    float getValueTime(const int inChannel) const noexcept;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout initParameterLayout();
    
    private:
    juce::AudioProcessorValueTreeState& mApvts;
    
    // this should be double to store .xxxxxxxxxxx many numbers
    double mSampleRate;
    
    //Gain
    juce::AudioParameterFloat* mParamGain; // for avoiding casting / mapping all the time. do this job beforehand.
    
    //smoothed Gain
    juce::LinearSmoothedValue<float> mValueGain;
    
    //float mValueGain; // ratio
        
    //Delay
    juce::AudioParameterFloat* mParamTime[2];
    float mValueTime[2];
    
    
    //for testing
    juce::AudioParameterFloat* mParamTest;
    juce::LinearSmoothedValue<float> mValueTest;

    
    
    //safety
    //if there is no user's constructor, system create temporary copied constructor inside.
    //this function doesn't allow creating the copied constructor.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxParameters);
};
