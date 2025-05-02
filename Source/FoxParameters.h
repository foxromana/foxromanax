/*
  ==============================================================================

    FoxParameters.h
    Created: 21 Feb 2025 8:34:17pm
    Author:  Fox Romana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FoxSmoother.h"

//class 10
struct FactoryPreset
{
    juce::String mName;
    float mTime[2]; //Delay time
    float mFeedback;
    float mMix;
    float mGain; 
};

namespace FoxParamIDs
{
    const juce::ParameterID Test("Test", 1);

    namespace Output
    {
        // Gain : parameter name, 1: version ( logic pro requires version which is not 0 ! )
        const juce::ParameterID Gain("Gain", 1);
    
        //Lesson 6 - mix for delay
        const juce::ParameterID Mix("Mix", 1);
    }
    namespace Delay
    {
        const juce::ParameterID Time[2] {{"TimeL", 1}, {"TimeR", 1}};
    }

    //Class 8
    namespace Feedback
    {
        const juce::ParameterID Amount("Amount", 1);
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
    
    //Gain
    float getValueGain() const noexcept; // no except : no exeption handling. because this function should be fast fast fast.
   
    //Test
    float getValueTest() const noexcept;
    
    static constexpr float kTimeMin = 5.0f;
    static constexpr float kTimeMax = 2000.0f;
    float getValueTime(const int inChannel) const noexcept;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout initParameterLayout();
    
    //Lesson 6 - mix
    float getValueMix() const noexcept;
    
    //class 8 - feedback
    float getValueAmount() const noexcept; 
    
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
    
    //Lesson 5 - Delay - but zipper noise
    //float mValueTime[2];
    //Lesson 6 - Delay with exponential smoothing
    FoxSmoother mValueTime[2];
    
    //for testing Lesson 5 - linear smoothing
    juce::AudioParameterFloat* mParamTest;
    //juce::LinearSmoothedValue<float> mValueTest;

    
    //Lesson 6 - Exponential smoothing
    FoxSmoother mValueTest;
    
    //Lesson 6 - mix
    juce::AudioParameterFloat* mParamMix;
    juce::LinearSmoothedValue<float> mValueMix;
    
    //class 8 - feedback
    juce::AudioParameterFloat* mParamAmount;
    juce::LinearSmoothedValue<float> mValueAmount;
    
    //safety
    //if there is no user's constructor, system create temporary copied constructor inside.
    //this function doesn't allow creating the copied constructor.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxParameters);
};
