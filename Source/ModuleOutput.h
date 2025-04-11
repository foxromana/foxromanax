/*
  ==============================================================================

    ModuleOutput.h
    Created: 14 Mar 2025 9:24:23pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ModuleOutput
{
    public:
    ModuleOutput();
    ~ModuleOutput();
    
    float process(const float inSampleDry, const float inSampleWet,
                  const float inMix, const float inGain) const noexcept;
    
    private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleOutput)
};
