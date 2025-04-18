/*
  ==============================================================================

    ModuleFeedback.cpp
    Created: 18 Apr 2025 8:50:56pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "ModuleFeedback.h"

ModuleFeedback::ModuleFeedback()
{}
ModuleFeedback::~ModuleFeedback()
{}

void ModuleFeedback::prepare() noexcept
{
    mFeedbackL.prepare();
    mFeedbackR.prepare();
    
    reset();
}
void ModuleFeedback::reset() noexcept
{
    mFeedbackL.reset();
    mFeedbackR.reset();
}

void ModuleFeedback::ProcessPush(const float inSampleL, const float inSampleR,
                 const float inFeedbackAmount) noexcept
{
    //push
    mFeedbackL.pushSample(inSampleL, inFeedbackAmount);
    mFeedbackR.pushSample(inSampleR, inFeedbackAmount);
}

void ModuleFeedback::ProcessPop(float& outSampleL, float& outSampleR) const noexcept
{
    outSampleL = mFeedbackL.popSample();
    outSampleR = mFeedbackR.popSample(); 
}
