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
//1000ms이 넘어가면 s초 단위로 변경
//input이 숫자
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

//class 8 : value from string
//input이 String
//만약 단위까지 넣어 입력하면 적용시켜주기 ex "10 ms" ->
static float millisecondFromString(const juce::String& inText)
{
    const float value = inText.getFloatValue(); //"300" -> 300.0f 로 자동변경!
    
    //마지막 글자가 ms 냐 ? 대소문자 구분 없이.
    if(inText.endsWithIgnoreCase("ms")==false)
    {
        //ms 최소값보다 작은 값을 입력해버렸거나, 초단위로 입력을 한 경우 --> 무조건 초 단위로 고려
        if(value < FoxParameters::kTimeMin || inText.endsWithIgnoreCase("s"))
        {
            return value * 1000.0f;
        }
    }
    
    return value;
}


//Lesson 6 - mix : set unit.
static juce::String stringFromPercent(float inValue, int)
{
    return juce::String((int)inValue) + " %"; // 10 -> 10%
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


FoxParameters::FoxParameters(juce::AudioProcessorValueTreeState& inApvts)
: mApvts(inApvts)
{
    //RangedAudioParameter is always normalized ( 0 ~ 1 )..
    //getParamID() = string name "Gain" is returned
    
    
    //mParamGain =  dynamic_cast<juce::AudioParameterFloat*>(mApvts.getParameter(FoxParamIDs::Output::Gain.getParamID()));
    castParameter(mApvts, FoxParamIDs::Output::Gain, mParamGain);
    //mParamTest =  dynamic_cast<juce::AudioParameterFloat*>(mApvts.getParameter(FoxParamIDs::Test.getParamID()));
    castParameter(mApvts, FoxParamIDs::Test, mParamTest);
    
    //Lesson 4~5 Dalay
    for(int i = 0 ; i <2 ;i++)
    {
        castParameter(mApvts,FoxParamIDs::Delay::Time[i], mParamTime[i]);
    }
    
    //Lesson 6 Mix
    castParameter(mApvts, FoxParamIDs::Output::Mix, mParamMix);
    
    //class 8 feedback
    castParameter(mApvts, FoxParamIDs::Feedback::Amount, mParamAmount);
    
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
    
    //Lesson 4-5 - Delay
    for(int i=0 ; i<2 ; ++i)
    {
        const juce::String nameTime = (i == 0) ? "Delay Time L" : "Delay Time R";
        layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Delay::Time[i],
                                                               nameTime,
                                                               juce::NormalisableRange<float>(kTimeMin, kTimeMax, 0.1f),
                                                               500.0f,
                                                               juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliSeconds)
                                                                   .withValueFromStringFunction(millisecondFromString)
                                                               //stringFromMilliSeconds, millisecondFromString -> 체인 콜링 방식. 자기를 리턴.. 같은 방식 적용 주르륵 연결
                                                               ));
    }
    
    //Lesson 6 - mix
    //need to be percent(%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Output::Mix, //type of parameter
                                                           "Mix", // parameter name
                                                           juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                           50.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));

    //Lesson 8 - feedback
    //need to be percent(%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(FoxParamIDs::Feedback::Amount,
                                                           "Amount",
                                                           juce::NormalisableRange<float>(0.0f, 95.0f, 1.0f), // if max is 100이면 삑 소리남
                                                           50.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));
    
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
    
    //Lesson 6 Delay with exponential smoothing
    mValueTime[0].reset(mSampleRate, timeLinearSmoothing); //Left
    mValueTime[1].reset(mSampleRate, timeLinearSmoothing); //Right
    
    //Lesson 6 Mix
    mValueMix.reset(mSampleRate, timeLinearSmoothing);
    
    //Lesson 8 Feedback
    mValueAmount.reset(mSampleRate, timeLinearSmoothing);
    
    
    //prepare() is called in the begining so needed to init target and current value
    update(); // target value initialized
    reset(); // current value initialized
}

void FoxParameters::smoothen() noexcept
{
    //Lesson 5 - linear smoothing
    //mValueTest.getNextValue();
    
    //Lesson 6 - exponential smoothing
    mValueTest.smoothen();
    mValueGain.getNextValue();
    //delay
    mValueTime[0].smoothen(); //Left
    mValueTime[1].smoothen(); //Right
    
    //Mix
    mValueMix.getNextValue();
    
    //Lesson 8
    mValueAmount.getNextValue();
    
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
    
    //Lesson 5
    //mValueTest.setTargetValue(mParamTest->get());
    
    //Lesson 6 - exponential smoothing
    mValueTest.setTarget(mParamTest->get());
    
    //Delay: ms -> numSamples
    for(int i=0; i<2 ;++i)
    {
        const double timeSec = mParamTime[i]->get() * 0.001; // ms -> s
        
        //Lesson 5
        //mValueTime[i] = (float)(timeSec * mSampleRate);
        
        //Lesson 6 - delay with smoothing exp
        mValueTime[i].setTarget(timeSec * mSampleRate);
    }
    
    //Lesson 6 Mix
    const float mix = mParamMix->get() * 0.01f;// 단위 변경: 0~100% -> 0.00 ~ 1.00
    mValueMix.setTargetValue(mix);
    
    //Lesson 8 Amount
    const float amount = mParamAmount->get() * 0.01f;// 단위 변경: 0~100% -> 0.00 ~ 1.00
    mValueAmount.setTargetValue(amount);
    
}

//from target, set the current value. no smoothing needed
void FoxParameters::reset() noexcept
{
    //lesson 5 - linear
    // make current value as a target value!
    //because no need smoothing in reset 
    //Go directly to the target value! no need to smoothe
    //mValueTest.setCurrentAndTargetValue(mValueTest.getTargetValue());
    
    //lesson 6 - exponential
    mValueTest.setCurrent(mValueTest.getTarget());
    //Go directly to the target value! no need to smoothe!! as it's reset!
    mValueGain.setCurrentAndTargetValue(mValueGain.getTargetValue());
    
    //delay with exp
    mValueTime[0].setCurrent(mValueTime[0].getTarget());
    mValueTime[1].setCurrent(mValueTime[1].getTarget());
    
    //mix
    mValueMix.setCurrentAndTargetValue(mValueMix.getTargetValue());
    
    //Lesson 8 Feedback
    mValueAmount.setCurrentAndTargetValue(mValueAmount.getTargetValue());
}


float FoxParameters::getValueTest() const noexcept
{
    //Lesson 5
    //return mValueTest.getCurrentValue();
    
    //Lesson 6
    return (float)mValueTest.getCurrent();
}

float FoxParameters::getValueGain() const noexcept
{
    //return mValueGain;
    return mValueGain.getCurrentValue();
}

float FoxParameters::getValueTime(const int inChannel) const noexcept
{
    //Lesson 5 with linear smoothing
    //return mValueTime[inChannel];
    
    //lesson 6 exp delay
    return (float)mValueTime[inChannel].getCurrent();
}

float FoxParameters::getValueMix() const noexcept
{
    return mValueMix.getCurrentValue();
}

float FoxParameters::getValueAmount() const noexcept
{
    return mValueAmount.getCurrentValue();
}

void FoxParameters::setParamsByFactoryPreset(const FactoryPreset& inPreset) noexcept
{
    for(int i = 0 ; i<2 ; ++i)
    {
        *(mParamTime[i]) = inPreset.mTime[i];
    }
    
    (*mParamAmount) = inPreset.mFeedback;
    (*mParamMix) = inPreset.mMix;
    (*mParamGain) = inPreset.mGain;
}

juce::ValueTree FoxParameters::getStateCopied() const noexcept
{
    return mApvts.copyState();
}

bool FoxParameters::setParamsByValueTree(const juce::ValueTree& inState) noexcept
{
    //type 이 뭐야 : FoxParameters
    //Plugin Processor 에서 맨 처음 mApvts을 선언할때 valuteTreetype(이름)을 "FoxParameters" 로 저장을 했었지!
    if(inState.getType() == mApvts.state.getType()) //<FoxParameters> 가 맞는지 확인
    {
        //<FoxParameters>가 맞으면 파라미터 replace 가능
        mApvts.replaceState(inState);
        return true;
    }
    return false;
}
