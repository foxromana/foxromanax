/*
  ==============================================================================

    FoxDelay.h
    Created: 7 Mar 2025 8:47:17pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FoxDelay
{
    public:
        FoxDelay();
        ~FoxDelay();
    
    void prepare(const double inSampleRate) noexcept;
    void reset() noexcept;
    float process(const float inSample, const float inDelay) noexcept;
    
    private:
    //similar to STL::vector
    juce::Array<float> mBuffer;
    int mSizeBuffer;
    
    int mIndexWriting; //where to store current data 
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxDelay)
};
