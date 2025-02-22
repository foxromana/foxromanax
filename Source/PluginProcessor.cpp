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
mApvts(*this, nullptr, "CParameters", CParameters::initParameterLayout()),
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
void FoxRomanaXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    DBG("HJY: preparetoplay"<< sampleRate <<", " <<samplesPerBlock<< ".");
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }*/
    
    //stereo : left 0, right 1
    
    //input buffer. const
    //range: -1.0 ~ 1.0
    const float* bufferInputL = buffer.getReadPointer(0);
    const float* bufferInputR = buffer.getReadPointer(1);
    
    //output buffer
    float* bufferOutputL = buffer.getWritePointer(0);
    float* bufferOutputR = buffer.getWritePointer(1);
        
    for(int i= 0; i<buffer.getNumSamples(); i++)
    {
        //Volume 50% down! -6 dB
        //bufferOutputL[i] = bufferInputL[i] * 0.5f; //-6dB
        //bufferOutputR[i] = bufferInputR[i] * 0.5f; //-6dB
        
        //left only.
        //bufferOutputL[i] = bufferInputL[i] + bufferInputR[i];
        //bufferOutputR[i] = 0.0f;
        
        //parameter
        bufferOutputL[i] = bufferInputL[i] * mParameters.getValueGain(); //-6dB
        bufferOutputR[i] = bufferInputR[i] * mParameters.getValueGain(); //-6dB
        
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
