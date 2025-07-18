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

void ModuleFeedback::prepare(const juce::dsp::ProcessSpec& inSpec) noexcept
{
    mFeedbackL.prepare();
    mFeedbackR.prepare();
    
    //dsp - lowcut or highcut 중 어떤 모드를 할건지 선택하기
    mFilterLowCut.setType(juce::dsp::StateVariableTPTFilterType::highpass); //lowcut
    mFilterLowCut.prepare(inSpec);
    
    mFilterHighCut.setType(juce::dsp::StateVariableTPTFilterType::lowpass); //highcut
    mFilterHighCut.prepare(inSpec);
    
    reset();
}
void ModuleFeedback::reset() noexcept
{
    mFeedbackL.reset();
    mFeedbackR.reset();
    mFilterLowCut.reset();
}

void ModuleFeedback::ProcessPush(const float inSampleL, const float inSampleR,
                                 const float inFeedbackAmount,
                                 const float inLowCut, const float inHighCut) noexcept
{
    // low cut 통과 -> high cut 통과 -> 아웃풋 출력
    //low cut filter
    mFilterLowCut.setCutoffFrequency(inLowCut);
    const float outLowCutL = mFilterLowCut.processSample(0, inSampleL);
    const float outLowCutR = mFilterLowCut.processSample(1, inSampleR);
    
    //high cut filter 적용
    mFilterHighCut.setCutoffFrequency(inHighCut);
    const float outHighCutL = mFilterHighCut.processSample(0, outLowCutL);
    const float outHighCutR = mFilterHighCut.processSample(1, outLowCutR);
    
    //주파수 세팅된걸 아웃풋에 집어넣어
    //push
    mFeedbackL.pushSample(outHighCutL, inFeedbackAmount);
    mFeedbackR.pushSample(outHighCutR, inFeedbackAmount);
}

void ModuleFeedback::ProcessPop(float& outSampleL, float& outSampleR) const noexcept
{
    outSampleL = mFeedbackL.popSample();
    outSampleR = mFeedbackR.popSample(); 
}
