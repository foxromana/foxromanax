/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FoxHelper.h"

//==============================================================================
FoxRomanaXAudioProcessor::FoxRomanaXAudioProcessor()
     : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
//member variables objects initialization - calling consturctor of member object
mApvts(*this, nullptr, "FoxParameters", FoxParameters::initParameterLayout()),
// parameters
mParameters(mApvts),
//preset manager
mPresetManager(mParameters)
{
    DBG("HJY: contructor");
}

FoxRomanaXAudioProcessor::~FoxRomanaXAudioProcessor()
{
    DBG("HJY: destructor");
}

//==============================================================================
const juce::String FoxRomanaXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FoxRomanaXAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FoxRomanaXAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FoxRomanaXAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FoxRomanaXAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FoxRomanaXAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FoxRomanaXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FoxRomanaXAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FoxRomanaXAudioProcessor::getProgramName (int index)
{
    return {};
}

void FoxRomanaXAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

//when plugin is from the track of the DAW, this function is up only once!
void FoxRomanaXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //pass sample rate to mParameters
    // Use this method as the place to do any pre-playback
    mParameters.prepare(sampleRate);
    
    //Until Lesson 5-6
    //mDelayL.prepare(sampleRate);
    //mDelayR.prepare(sampleRate);
    
    //Lesson 6
    mModuleDelay.prepare(sampleRate);
    
    //Lesson 9
    //mFeedbackL.prepare();
    //mFeedbackR.prepare();
    
    //lesson 9-2 feedback module을 쓴 경우
    mModuleFeedback.prepare();
}

void FoxRomanaXAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    DBG("HJY: releaseresources");
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FoxRomanaXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    //is this plugin mono or stereo? fix it to stereo.
    //when DAW is open, DAW checks all the plugins and companies, and which channel system is supported**!!
    //this functions returns the plugin's info : "this plugin supports stereo and mono, or stereo only etc...
    //so All of the plugin calls this function!!! (that's why it's slow
    
    return (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() &&
            layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo());
}
#endif

void FoxRomanaXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //HJY: actual work here!!
    
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //stereo : left 0, right 1
    //input buffer. const
    //range: -1.0 ~ 1.0
    const float* bufferInputL = buffer.getReadPointer(0);
    const float* bufferInputR = buffer.getReadPointer(1);
    
    //output buffer
    float* bufferOutputL = buffer.getWritePointer(0);
    float* bufferOutputR = buffer.getWritePointer(1);
       
    //DAW로부터 BPM 정보 가져오기
    double bpm = 120.0 ;
    updateByPositionInfo(bpm);
    
    //class 3
    //Normalize db-> ratio update first
    //chage the final target value of for smoothing
    mParameters.update(bpm);
    
    //crossfading
    mModuleDelay.preProcess(mParameters.getValueTime(0), mParameters.getValueTime(1));
    
    for(int i= 0; i<buffer.getNumSamples(); i++)
    {
        //class 1
        //Volume 50% down! -6 dB
        //bufferOutputL[i] = bufferInputL[i] * 0.5f; //-6dB
        //bufferOutputR[i] = bufferInputR[i] * 0.5f; //-6dB
                
        //class 3
        //move one step. ( number of sample rates = step!)
        mParameters.smoothen();
                
        //only Gain up
        //bufferOutputL[i] = bufferInputL[i] * mParameters.getValueGain(); // get gain in ratio
        //bufferOutputR[i] = bufferInputR[i] * mParameters.getValueGain(); //-6dB


        //class 4
        //Delay first
        //const float inputL = bufferInputL[i];
        //const float inputR = bufferInputR[i];
        //const float outDelayL = mDelayL.process(inputL, mParameters.getValueTime(0));
        //const float outDelayR = mDelayR.process(inputR, mParameters.getValueTime(1));
        //after delay, Gain up
        //bufferOutputL[i] = outDelayL * mParameters.getValueGain(); // get gain in ratio
        //bufferOutputR[i] = outDelayR * mParameters.getValueGain(); //

        //class 6
        //dry = input
        const float dryL = bufferInputL[i];
        const float dryR = bufferInputR[i];
        
        
        //wet = output
        //const float wetL = mDelayL.process(dryL, mParameters.getValueTime(0));
        //const float wetR = mDelayR.process(dryR, mParameters.getValueTime(1));
        
        //class 6 - L/R same exponential 신호만 확인할 때 사용 (음 x)
        //bufferOutputL[i] = mParameters.getValueTest();
        //bufferOutputR[i] = mParameters.getValueTest();

      
        //class 9 -----------
        //dry = input + pop sample
        //const float feedbackL = mFeedbackL.popSample();
        //const float feedbackR = mFeedbackR.popSample();
        
        //lesson 9-2 feedback module을 쓴 경우
        float feedbackL = 0.0f;
        float feedbackR = 0.0f;
        mModuleFeedback.ProcessPop(feedbackL, feedbackR);
        
        //--------class 9 //
        
        
        //class 6 - module output. mix only
        float wetL = 0.0f;
        float wetR = 0.0f;
        // 이때는 feedback (wet)이 없고 dry 로 한번 딜레이로 나옴
        //mModuleDelay.process(dryL, dryR,
        //                     wetL, wetR,
        //                     mParameters.getValueTime(0),
        //                     mParameters.getValueTime(1));
  
        //class 6
        //const float mixL = dryL + (wetL * mParameters.getValueMix());
        //const float mixR = dryR + (wetR * mParameters.getValueMix());

    
        //class 9 : dry 에 전단계에서 발생한 feedback 결과도 더하기 = 메아리 생기게
        mModuleDelay.process(dryL + feedbackL, dryR + feedbackR,
                             wetL, wetR,
                             mParameters.getValueTime(0),
                             mParameters.getValueTime(1));
        
        
        //class 7 - Output module mix * gain
        const float outL = mModuleOutput.process(dryL, wetL,
                                           mParameters.getValueMix(),
                                           mParameters.getValueGain());

        const float outR = mModuleOutput.process(dryR, wetR,
                                           mParameters.getValueMix(),  
                                           mParameters.getValueGain());

            
        //class 6
        //bufferOutputL[i] = mixL * mParameters.getValueGain();
        //bufferOutputR[i] = mixR * mParameters.getValueGain();
        
        //class 7
        bufferOutputL[i] = outL;
        bufferOutputR[i] = outR;
        
        //class 9 : feedback push!
        //mFeedbackL.pushSample(wetL, mParameters.getValueAmount());
        //mFeedbackR.pushSample(wetR, mParameters.getValueAmount());
        
        //lesson 9-2 feedback module을 쓴 경우
        mModuleFeedback.ProcessPush(wetL, wetR, mParameters.getValueAmount());
    }
    
    //최종 validation 단계
    //debug mode에서만 사용. Edit Scheme 메뉴에서 release 로 바꾸고 빌드하면 이 코드는 실행하지 않음
    //마지막 딜레이 아웃풋이 허용가능한 dB 안인지 확인하고 만약 값이 너무 크거나 숫자가 아니면 소리를 꺼버려서 귀를 보호하기
    #if JUCE_DEBUG
    FoxHelper::protectEars(buffer);
    #endif
}

//==============================================================================
bool FoxRomanaXAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FoxRomanaXAudioProcessor::createEditor()
{
    return new FoxRomanaXAudioProcessorEditor (*this);
}

//==============================================================================
void FoxRomanaXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //DAW: 나 이제 프로젝트 끌꺼니까 니네들(플러그인들) 현재 정보 저장해라.
    //plugin tree -> xml -> memory
    // - parameter tree
    // - preset tree
    // 트리 벨류 이름 = 띄어쓰기가 있으면 안됨 (foxromanaX_plugin 로) 
    juce::ValueTree statePlugin("foxromanaX_plugin"); //JucePlugin_Name = 플로그인 이름 자동으로 불러와주는 매크로
    
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //DAW 입장에서 프로젝트 저장할 때 모든 플로그인들의 상태들을 get, 플로그인에게 정보 요청. 나 이제 프로젝트 끌꺼니까 니네 현재 정보 저장해라.
    //Parameter 정보 바구니에 현재 정보 집어넣기 -> 차일드 노드로 집어넣기. 자식 트리 1
    const juce::ValueTree stateParameter = mApvts.copyState();
    if(stateParameter.isValid())
    {
        statePlugin.appendChild(stateParameter, nullptr);
    }
    
    //preset 정보 차일드로 집어넣기. 자식 트리 2.
    const juce::ValueTree statePreset = mPresetManager.getState();
    if(statePreset.isValid())
    {
        statePlugin.appendChild(statePreset, nullptr);
    }
    
    //xml 포맷으로 바꾸기
    std::unique_ptr<juce::XmlElement> xmlState = statePlugin.createXml();
    if(xmlState == nullptr)
    {
        return;
    }
    
    //binary 로 만들어서 DAW(로직파일) 메모리 공간에 저장
    copyXmlToBinary(*xmlState, destData);
    
    //한번 저장 잘되는지 데스크탑에 파일 만들어 확인해보기
    //const juce::File stateFile(juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("pluginState.xml"));
    //xmlState->writeTo(stateFile);
    //stateFile.create();
    
}

void FoxRomanaXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    //DAW 입장에서 set. DAW에서 프로젝트를 열때, 이전 프로젝트에서 저장되었던 플로그인의 상태 정보 가져오기
    //DAW: 나 이제 프로젝트 열거니까 니네들(플러그인들) 현재 정보 꺼내라.


    std::unique_ptr<juce::XmlElement> xmlPlugin = getXmlFromBinary(data, sizeInBytes);
    if(xmlPlugin == nullptr)
    {
        return; //저장된 정보 없으면 그냥 고
    }
    
    const juce::ValueTree statePlugin = juce::ValueTree::fromXml(*xmlPlugin);
    if(statePlugin.isValid() == false || statePlugin.getType().toString() != "foxromanaX_plugin" )
    {
        return;
    }
    
    //preset data
    const juce::ValueTree statePreset = statePlugin.getChildWithName(mPresetManager.getIdState());
    if(statePreset.isValid())
    {
        mPresetManager.setByState(statePreset);
    }
    
    //parameter data
    const juce::ValueTree stateParameter = statePlugin.getChildWithName(mApvts.state.getType());
    if(stateParameter.isValid())
    {
        mApvts.replaceState(stateParameter);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FoxRomanaXAudioProcessor();
}

//재생을 멈췄다가 처음부터 다시 시작될 때 호출되는 함수
void FoxRomanaXAudioProcessor::reset()
{
    mParameters.reset();
    
    //till lesson 5-6
    //Replay시 delay 버퍼를 초기화 해야함!
    //mDelayL.reset();
    //mDelayR.reset();
    
    //lesson 6
    mModuleDelay.reset();
    
    //lesson 9
    //mFeedbackL.reset();
    //mFeedbackR.reset();
    //lesson 9-2 feedback module을 쓴 경우
    mModuleFeedback.reset();
}

juce::AudioProcessorValueTreeState& FoxRomanaXAudioProcessor::getApvts() noexcept
{
    return mApvts; 
}

FoxPresetManager& FoxRomanaXAudioProcessor::getPresetManager() noexcept
{
    return mPresetManager; 
}


void FoxRomanaXAudioProcessor::updateByPositionInfo(double& outBpm) noexcept
{
    //audio proccessor -> audio play head -> positionInfo -> BPM 가져오기
    juce::AudioPlayHead* playHead = getPlayHead();
    if(playHead == nullptr)
    {
        return;
    }
    
    //playhead 통해 position 가져오기
    //std::optional (있을수도 없을수도 있는 내용) 리턴 타입은 auto - 타입을 포인터로 지정해줌
    auto positionInfo = playHead->getPosition();
    if(!positionInfo)
    {
        //getPosition 결과가 null 이다.
        return;
    }
    //getPosition 결과가 유효하다면 BPM을 가져온다. 근데 BPM도 optional 타입이라 auto로 가져와야한다.
    auto bpmFromDAW = positionInfo->getBpm();
    if(bpmFromDAW)
    {
        //이 함수의 최종 결과는 DAW가 알려준 BPM 수
        outBpm = *bpmFromDAW;
    }
    
}
