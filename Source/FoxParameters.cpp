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
: mApvts(inApvts),
mChannelMaster(0), //기본 마스터는 0
mFlagLinking(false)
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
        castParameter(mApvts,FoxParamIDs::Delay::Note[i], mParamNote[i]); //BPM 기준일때
    }
    
    //Lesson 6 Mix
    castParameter(mApvts, FoxParamIDs::Output::Mix, mParamMix);
    
    //class 8 feedback
    castParameter(mApvts, FoxParamIDs::Feedback::Amount, mParamAmount);
    
    //class 13 control
    //tempo 기능 - GUI 링크
    castParameter(mApvts, FoxParamIDs::Control::Tempo, mParamTempo);
    //좌우 Link 기능 - GUI 링크
    castParameter(mApvts, FoxParamIDs::Control::Link, mParamLink);
    
    //리스너 등록
    mApvts.addParameterListener(FoxParamIDs::Control::Link.getParamID(), this);
    
}

FoxParameters::~FoxParameters()
{
    //리스너 삭제
    mApvts.removeParameterListener(FoxParamIDs::Control::Link.getParamID(), this);
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
    const juce::StringArray notes = {
        "1/16 triplet",
        "1/32 dotted",
        "1/16",
        "1/8 triplet",
        "1/16 dotted",
        "1/8",
        "1/4 triplet",
        "1/8 dotted",
        "1/4",
        "1/2 triplet",
        "1/2 dotted",
        "1/2",
        "1/1 triplet",
        "1/2 dotted"
    };
    
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
        
        //notes 박자 기준
        const juce::String nameNote = (i ==0)?"Delay Note L" : "Delay Note R";
        layout.add(std::make_unique<juce::AudioParameterChoice>(FoxParamIDs::Delay::Note[i],
                                                                nameNote,
                                                                notes,
                                                                notes.indexOf("1/4")));
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
    
    //Control - 초기값이 false
    layout.add(std::make_unique<juce::AudioParameterBool>(FoxParamIDs::Control::Tempo,"Tempo",false));
    layout.add(std::make_unique<juce::AudioParameterBool>(FoxParamIDs::Control::Link,"Stereo Link",false));
        
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
void FoxParameters::update(const double inBpm) noexcept
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
    
    //Control - tempo 싱크 기능을 쓰는지 안쓰는지 확인 후 BPM, note 기준
    const bool tempo = mParamTempo->get();
    
    //Delay: ms -> numSamples
    for(int i=0; i<2 ;++i)
    {
        
        //tempo 설정시 BPM/note 기준으로 딜레이 시간 구하기, tempo가 아닌경우 딜레이 시간을 직접 인력함.
        const double timeSec = tempo ?
        getTimeByNote(inBpm, mParamNote[i]->getIndex()) : mParamTime[i]->get() * 0.001;
        
        //딜레이시간 직접 입력하는 경우.(옛날 방식)
        //const double timeSec = mParamTime[i]->get() * 0.001; // ms -> s
        
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

double FoxParameters::getTimeByNote(const double inBpm, const int inNote) const noexcept
{
    static constexpr double scalars[] = { 0.25 * 2.0 / 3.0,  // SixteenthTriplet,
                                          0.125 * 1.5,       // ThirtySecondDotted,
                                          0.25,              // Sixteenth,
                                          0.5 * 2.0 / 3.0,   // EighthTriplet
                                          0.25 * 1.5,        // SixteenthDotted,
                                          0.5,               // Eighth,
                                          1.0 * 2.0 / 3.0,   // QuarterTriplet,
                                          0.5 * 1.5,         // EighthDotted,
                                          1.0,               // Quarter,
                                          2.0 * 2.0 / 3.0,   // HalfTriplet,
                                          1.0 * 1.5,         // QuarterDotted,
                                          2.0,               // Half,
                                          4.0 * 2.0 / 3.0,   // Triplet,
                                          2.0 * 1.5 };       // HalfDotted,
    
    // bpm = beat per minute : 1분에 몇 비트인가
    const double minutePerBeat = 1.0 / inBpm; // 1 비트에 몇분인가
    const double secondPerBeat = minutePerBeat * 60.0; // 1비트(1박)에 몇 초인가 = quater
 
    /*
    enum Notes
    {
        SixteenthTriplet,
        ThirtySecondDotted, // 1/32 dotted
        Sixteenth, // 1/16
        EighthTriplet,
        SixteenthDotted,
        Eighth, // 1/8
        QuarterTriplet,
        EighthDotted,
        Quarter, // 1/4
        HalfTriplet,
        QuarterDotted,
        Half, // 1/2
        Triplet, // 1/1 triplet
        HalfDotted
    };
    
    //시간 구하기
    //juce::StringArray notes 의 인덱스를 enum 화 해서 매핑
    switch (inNote) {
        case Notes::Sixteenth: return secondPerBeat * 0.25;
        case Notes::Eighth: return secondPerBeat * 0.5;
        case Notes::Quarter: return secondPerBeat;
        case Notes::Half: return secondPerBeat * 2.0;
            
        //Triplet : 2박 3연음
        case Notes::SixteenthTriplet: return secondPerBeat * 0.25 * 2.0 / 3.0;
        case Notes::EighthTriplet: return secondPerBeat * 0.5 * 2.0 / 3.0;;
        case Notes::QuarterTriplet: return secondPerBeat * 2.0 / 3.0;
        case Notes::HalfTriplet: return secondPerBeat * 2.0 * 2.0 / 3.0;
        case Notes::Triplet: return secondPerBeat * 4.0 * 2.0 / 3.0;
            
        //dotted : 점음표. 부점 QuaterDotted : 1/4 dotted = secondPerBeat * 1.5
        case Notes::ThirtySecondDotted: return secondPerBeat * 0.125 * 1.5;
        case Notes::SixteenthDotted: return secondPerBeat * 0.25 * 1.5;
        case Notes::EighthDotted: return secondPerBeat * 0.5 * 1.5;
        case Notes::QuarterDotted: return secondPerBeat * 1.5;
        case Notes::HalfDotted: return secondPerBeat * 2.0 * 1.5;


        default: return secondPerBeat;
    }
     */
    return secondPerBeat * scalars[inNote];
}

//이벤트 콜백 함수
//어떤 파라미터 ID 에 해당하는 변수의 값에 변화가 있으면 이 함수가 호출되었는지, 파라미터의 현재값이 inValue 일때 호출
//이 함수는 재귀처럼 불리는게 아니라 다른 스레드 처럼 호출된대...
void FoxParameters::parameterChanged(const juce::String& inParamId, float inValue)
{
    if(mFlagLinking.load() == true)
    {
        //반복구문 때문에 이 함수가 호출된 경우
        return; // 함수 종료. 아무것도 안하고.
    }
    
    const juce::String& paramIdLink = FoxParamIDs::Control::Link.getParamID();
    const juce::String& paramIdTimeL = FoxParamIDs::Delay::Time[0].getParamID();
    const juce::String& paramIdTimeR = FoxParamIDs::Delay::Time[1].getParamID();
    const juce::String& paramIdNoteL = FoxParamIDs::Delay::Note[0].getParamID();
    const juce::String& paramIdNoteR = FoxParamIDs::Delay::Note[1].getParamID();
    
    //mChannelMaster = (inParamId == paramIdTimeR || inParamId == paramIdNoteR) ? 1 : 0;
    const int masterCurrent = mChannelMaster.load();
    const int masterNew = (inParamId == paramIdTimeR || inParamId == paramIdNoteR) ? 1 : 0;
    if( masterCurrent != masterNew)
    {
        mChannelMaster.store(masterNew);
    }
    
    if(inParamId == paramIdLink)
    {
        if (inValue == 1.0f)
        {
            // stereo link 가 On 인 상태
            //sync 가 On 되면 탬포랑 딜레이 타임 변화에 대한 리스너도 on 해야 한다.
            //이제부터는 time 과 tempo 중에 변화가 생기면 (= 마스터 채널이 될 놈) parameterChanged() 가 호출됨
            mApvts.addParameterListener(paramIdTimeL, this);
            mApvts.addParameterListener(paramIdTimeR, this);
            mApvts.addParameterListener(paramIdNoteL, this);
            mApvts.addParameterListener(paramIdNoteR, this);
            //timerCallback 함수가 30 Hz 마다 실행된다
            startTimerHz(30); //30 HZ
        }
        else
        {
            // stereo link 가 Off 인 상태
            stopTimer();
            //sync 가 안되어 있으므로 이제 리스너들 필요없음
            mApvts.removeParameterListener(paramIdTimeL, this);
            mApvts.removeParameterListener(paramIdTimeR, this);
            mApvts.removeParameterListener(paramIdNoteL, this);
            mApvts.removeParameterListener(paramIdNoteR, this);
            
            //링크 off 일 때는 L/R 싱크를 실행할 필요가 없으로 아래 코드로 이동하지 않도록 아예 함수를 종료(return)
            return;
        }
    }
}

void FoxParameters::timerCallback()
{
    const int master = mChannelMaster.load(); //atomic 저장된 값 가져오는 방법
    
    //mFlagLinking = true;
    mFlagLinking.store(true); //atomic 이라서 바로 true 로 안하고.. 크리티컬 섹션 기능
    
    //L/R 값 싱크하기
    for(int i=0 ; i<2; i++)
    {
        if(i != mChannelMaster) // slave 인 경우에만
        {
            //슬레이브의 파라미터 Time 값을 마스터의 파라미터 값으로 변경
            //값에 변경이 일어났으니 parameterChanged() 함수가 또 호출될거야.
            *(mParamTime[i]) = mParamTime[master]->get(); //float
            //슬레이브의 파라미터 Tempo 값을 마스터의 파라미터 값으로 변경
            //값에 변경이 일어났으니 parameterChanged() 함수가 또 호출될거야.
            *(mParamNote[i]) = mParamNote[master]->getIndex(); //tempo 타입 배열 중 선택된 index
        }
    }
    
    //mFlagLinking = false;
    mFlagLinking.store(false);
}

