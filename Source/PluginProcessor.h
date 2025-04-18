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
//==============================================================================
/**
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

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //called when DAW stops and replay
    void reset() override;
    
    juce::AudioProcessorValueTreeState& getApvts() noexcept;

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
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxRomanaXAudioProcessor)
};
