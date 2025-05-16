/*
  ==============================================================================

    FoxPresetManager.h
    Created: 2 May 2025 9:27:27pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FoxParameters.h"

//팩토리 프리셋 관리자
//여러 팩토리 프리셋을 배열로 가지고 있기
class FoxPresetManager
{
    public:
        FoxPresetManager(FoxParameters& inParameters);
        ~FoxPresetManager();
    
        //Factory Preset
        juce::String getNameFactoryPreset(const int inIndex) const noexcept;
        int getNumFactoryPresets() const noexcept;
        int getFactoryPresetCurrent() const noexcept;
        void setFactoryPreset(const int inIndex) noexcept;
    
        const juce::File& getDirPreset() const noexcept;
    
    private:
        FoxParameters& mParameters;
        juce::Array<FactoryPreset> mArrayFactoryPresets; // preset data
        //preset index that user selects
        // -1 : not selected
        int mFactoryPresetCurrent;
        void resetFactoryPresets() noexcept;
  
        //preset 저장 경로
        const juce::File mDirPreset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxPresetManager)
    
};
