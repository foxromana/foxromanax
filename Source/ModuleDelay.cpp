/*
  ==============================================================================

    ModuleDelay.cpp
    Created: 14 Mar 2025 9:24:11pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "ModuleDelay.h"

ModuleDelay::ModuleDelay()
{
    
}

ModuleDelay::~ModuleDelay()
{
    
}

void ModuleDelay::prepare(const double inSampleRate) noexcept
{
    mDelayL.prepare(inSampleRate);
    mDelayR.prepare(inSampleRate);
}
void ModuleDelay::reset() noexcept
{
    mDelayL.reset();
    mDelayR.reset();
}
void ModuleDelay::process(const float inSampleL, const float inSampleR,
             float& outSampleL, float& outSampleR,
             const float inDelayL, const float inDelayR) noexcept
{
    outSampleL = mDelayL.process(inSampleL, inDelayL);
    outSampleR = mDelayR.process(inSampleR, inDelayR);
}
