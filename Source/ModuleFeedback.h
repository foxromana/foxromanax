/*
  ==============================================================================

    ModuleFeedback.h
    Created: 18 Apr 2025 8:50:56pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include "FoxFeedback.h"

class ModuleFeedback
{
    public:
    ModuleFeedback();
    ~ModuleFeedback();
    
    //void prepare() noexcept;
    void prepare(const juce::dsp::ProcessSpec& inSpec) noexcept;
    void reset() noexcept;
    
    void ProcessPush(const float inSampleL, const float inSampleR,
                     const float inFeedbackAmount,
                     const float inLowCut, const float inHighCut) noexcept;
    
    void ProcessPop(float& outSampleL, float& outSampleR) const noexcept; 
    
    private:
    FoxFeedback mFeedbackL, mFeedbackR;
    
    //filter
    juce::dsp::StateVariableTPTFilter<float> mFilterLowCut;
    juce::dsp::StateVariableTPTFilter<float> mFilterHighCut;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleFeedback);
};
