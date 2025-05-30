/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FoxParameters.h"
#include "ModuleDelay.h"
#include "ModuleOutput.h"
#include "FoxFeedback.h"
#include "ModuleFeedback.h"
#include "FoxPresetManager.h"
//==============================================================================
/**
 GUI 화면 뜨기 전에 plugin processor 은 먼저 실행
*/
class FoxRomanaXAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FoxRomanaXAudioProcessor();
    ~FoxRomanaXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //DAW 기준. 프로젝트에서 사용하고 있는 플로그인들한테 저장공간을 좀 주면서 "나 이제 나갈꺼니까 니 정보 (state)좀 이 바구니에 저장해" 함
    //DAW가 시작될때, "나 프로젝트 시작했다,플로그인들아 니네 바구니에서 마지막으로 저장했던거 꺼내라"
    //플러그인은 각자의 바구니에 정보를 저장/열기 가능
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //called when DAW stops and replay
    void reset() override;
    
    juce::AudioProcessorValueTreeState& getApvts() noexcept;
    
    //Preset manager obj address 
    FoxPresetManager& getPresetManager() noexcept;
    
private:
    
    //parameter storage that JUCE is recommending!
    //parameter is so important, need to create a class!
    juce::AudioProcessorValueTreeState mApvts;
    
    //Fox Romana Parameter layout
    FoxParameters mParameters;
    
    //Until Lesson 5-6
    //FoxDelay mDelayL, mDelayR;
    
    //Lesson 6
    ModuleDelay mModuleDelay;
    
    //Lesson 7
    ModuleOutput mModuleOutput; 
    
    //Lesson 9
    //FoxFeedback mFeedbackL, mFeedbackR;
    
    //lesson 9 feedback을 모듈화 한 경우
    ModuleFeedback mModuleFeedback;
    
    //lesson preset !
    FoxPresetManager mPresetManager;
    
    //audio playhead (하얀 세로줄 재생바)기준으로 정보 받아서 BPM 기준으로 딜레이 박자 업데이트 하기
    void updateByPositionInfo(double& outBpm) noexcept;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxRomanaXAudioProcessor)
};
