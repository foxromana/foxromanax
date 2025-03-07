/*
  ==============================================================================

    FoxParameters.cpp
    Created: 21 Feb 2025 8:34:17pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "FoxParameters.h"

//for Gain
//string-from-value function UI
static juce::String stringFromDecibels(float inValue, int)
{
    return juce::String(inValue, 1) + " dB"; // 1: 0.0 2: 0.00 3: 0.000
}

//for Delay
// ms = 1/1000 second UI
static juce::String stringFromMilliSeconds(float inValue, int)
{
    //check if it's less than 1000 ( 1s). till 999 ms
    if(inValue < 1000.0f) // ~999 ms
    {
        return juce::String(inValue, 1) + "ms";
    }
    else // 1s ~
    {
        return juce::String(inValue * 0.001f, 4) + " s"; // convert 1000.x ms -> 1.xxx s
    }
}

//template function for make dynamic casting easier!!
template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& inApvts,
                          const juce::ParameterID& inParamID, T& inDestination)
{
    //cast input value into destination's type and save it into destination
    //if dest should be apple, cut pear into apple and replace the dest.
    inDestination = dynamic_cast<T>(inApvts.getParameter(inParamID.getParamID()));
    jassert(inDestination);
}


FoxParameters::FoxParameters(juce::AudioProcessorValueTreeState& inApvts) : mApvts(inApvts)
{
    //RangedAudioParameter is always normalized ( 0 ~ 1 )..
    //getParamID() = string name "Gain" is returned
    
    
    //mParamGain =  dynamic_cast<juce::AudioParameterFloat*>(mApvts.getParameter(FoxParamIDs::Output::Gain.getParamID()));
    castParameter(mApvts, FoxParamIDs::Output::Gain, mParamGain);
    //mParamTest =  dynamic_cast<juce::AudioParameterFloat*>(mApvts.getParameter(FoxParamIDs::Test.getParamID()));
    castParameter(mApvts, FoxParamIDs::Test, mParamTest);
    
    //Dalay
    for(int i = 0 ; i <2 ;i++)
    {
        castParameter(mApvts,FoxParamIDs::Delay::Time[i], mParamTime[i]);
    }
    
}

FoxParameters::~FoxParameters()
{
    
}

//Static! only defined once.
juce::AudioProcessorValueTreeState::ParameterLayout FoxParameters::initParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //std::make_unique - smart pointer, not used? then removed in the memory automatically
    
    //add Gain parameter information
    layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Output::Gain, //type of parameter
                                                           "Gain", // parameter name
                                                           juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),// range: min dB~ max dB, move unit 0.1
                                                           0.0f, // default value
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels) // ## dB in UI
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Test, //type of parameter
                                                           "Test", // parameter name
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f),// range: min dB~ max dB move unit 0.1
                                                           0.0f));
    
    //Delay
    for(int i=0 ; i<2 ; ++i)
    {
        const juce::String nameTime = (i == 0) ? "Delay Time L" : "Delay Time R";
        layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Delay::Time[i],
                                                               nameTime,
                                                               juce::NormalisableRange<float>(kTimeMin, kTimeMax, 0.1f),
                                                               500.0f,
                                                               juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliSeconds)
                                                               ));
    }
    
    // juce::audioparameter ( RangedAudioParamter is parent. it's always range from 0 to 1 )
    // - int : ..?
    // - bool : mute button (toggle)
    // - float : processing
    // - choice : mode selection
    
    return layout;
    
}

//called only once
void FoxParameters::prepare(const double inSampleRate) noexcept
{
    mSampleRate = inSampleRate;
    const double timeLinearSmoothing = 0.05; //50 ms  should go up to target value in 0.05 second
    
    //reset:define the number of steps for smoothing
    mValueGain.reset(mSampleRate, timeLinearSmoothing); // mSampleRate * 0.05 = 44100 * 0.05 = 220 steps needed!
    
    //test
    mValueTest.reset(mSampleRate, timeLinearSmoothing); // mSampleRate * 0.05 = 44100 * 0.05 = 220 steps needed!
    
    
    //prepare() is called in the begining so needed to init target and current value
    update(); // target value initialized
    reset(); // current value initialized
}

void FoxParameters::smoothen() noexcept
{
    mValueTest.getNextValue();
    mValueGain.getNextValue();
}

//set target
void FoxParameters::update() noexcept
{
    
    //dB -> ratio calculation
    
    //old way
    //mValueGain = juce::Decibels::decibelsToGain(mParamGain->get()); ( dB -> ratio processing )
    
    // new way
    //called only once
    
    const float gain = juce::Decibels::decibelsToGain(mParamGain->get());
    
    mValueGain.setTargetValue(gain);
    mValueTest.setTargetValue(mParamTest->get());
    
    //Delay: ms -> numSamples
    for(int i=0; i<2 ;++i)
    {
        const double timeSec = mParamTime[i]->get() * 0.001; // ms -> s
        mValueTime[i] = (float)(timeSec * mSampleRate); 
    }
    
}

//from target, set the current value. no smoothing needed
void FoxParameters::reset() noexcept
{
    // make current value as a target value!
    //because no need smoothing in reset 
    //Go directly to the target value! no need to smoothe
    mValueTest.setCurrentAndTargetValue(mValueTest.getTargetValue());
    
    //Go directly to the target value! no need to smoothe!! as it's reset!
    mValueGain.setCurrentAndTargetValue(mValueGain.getTargetValue());
}


float FoxParameters::getValueTest() const noexcept
{
    return mValueTest.getCurrentValue();
}

float FoxParameters::getValueGain() const noexcept
{
    //return mValueGain;
    return mValueGain.getCurrentValue();
}

float FoxParameters::getValueTime(const int inChannel) const noexcept
{
    return mValueTime[inChannel];
}
