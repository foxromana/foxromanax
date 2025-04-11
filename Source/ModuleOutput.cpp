/*
  ==============================================================================

    ModuleOutput.cpp
    Created: 14 Mar 2025 9:24:23pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "ModuleOutput.h"

ModuleOutput::ModuleOutput()
{}

ModuleOutput::~ModuleOutput()
{}

float ModuleOutput::process(const float inSampleDry, const float inSampleWet,
              const float inMix, const float inGain) const noexcept
{
 
    //Mix
    const float outMix = inSampleDry + (inSampleWet * inMix);
    
    //Gain
    return outMix * inGain;
}
