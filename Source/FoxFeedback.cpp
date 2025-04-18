/*
  ==============================================================================

    FoxFeedback.cpp
    Created: 18 Apr 2025 8:09:26pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "FoxFeedback.h"

FoxFeedback::FoxFeedback()
{
    
}
FoxFeedback::~FoxFeedback(){
    
}

void FoxFeedback::prepare() noexcept
{
    reset();
}
void FoxFeedback::reset() noexcept
{
    //처음엔 전단계가 없어서 프로세싱한 pop 할게 없으니까 0을 출력
    mSampleFeedback = 0.0f;
}

void FoxFeedback::pushSample(const float inSample, const float inAmount) noexcept
{
    mSampleFeedback = inSample * inAmount;
}

float FoxFeedback::popSample() const noexcept
{
    return mSampleFeedback;
}
