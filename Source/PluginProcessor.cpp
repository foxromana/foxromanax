/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FoxRomanaXAudioProcessor::FoxRomanaXAudioProcessor()
     : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
//member variables objects initialization - calling consturctor of member object
mApvts(*this, nullptr, "FoxParameters", FoxParameters::initParameterLayout()),
// parameters
mParameters(mApvts)
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
       
    //class 3
    //Normalize db-> ratio update first
    //chage the final target value of for smoothing
    mParameters.update();
    
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

        
        //class 6 - module output. mix only
        float wetL = 0.0f;
        float wetR = 0.0f;
        mModuleDelay.process(dryL, dryR,
                             wetL, wetR,
                             mParameters.getValueTime(0),
                             mParameters.getValueTime(1));
  
        //class 6
        //const float mixL = dryL + (wetL * mParameters.getValueMix());
        //const float mixR = dryR + (wetR * mParameters.getValueMix());

        
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
    }
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FoxRomanaXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FoxRomanaXAudioProcessor();
}

void FoxRomanaXAudioProcessor::reset()
{
    mParameters.reset();
    
    //till lesson 5-6
    //Replay시 delay 버퍼를 초기화 해야함!
    //mDelayL.reset();
    //mDelayR.reset();
    
    //lesson 6
    mModuleDelay.reset();
}

juce::AudioProcessorValueTreeState& FoxRomanaXAudioProcessor::getApvts() noexcept
{
    return mApvts; 
}
