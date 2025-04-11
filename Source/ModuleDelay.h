/*
  ==============================================================================

    ModuleDelay.h
    Created: 14 Mar 2025 9:24:11pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once
#include "FoxDelay.h"

class ModuleDelay
{
    public:
        ModuleDelay();
        ~ModuleDelay();
    
        void prepare(const double inSampleRate) noexcept;
        void reset() noexcept;
        void process(const float inSampleL, const float inSampleR,
                     float& outSampleL, float& outSampleR,
                     const float inDelayL, const float inDelayR) noexcept;
        
    private:
        FoxDelay mDelayL, mDelayR;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleDelay)
};
