/*
  ==============================================================================

    FoxPresetManager.cpp
    Created: 2 May 2025 9:27:27pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "FoxPresetManager.h"

FoxPresetManager::FoxPresetManager(FoxParameters& inParameters)
:
mParameters(inParameters),
mFactoryPresetCurrent(-1),
//모든 운영체제 파일시스템이 다르니까,Juce에서 운영체제 상관없이 문서 경로 찾아주는 함수 만들어줌.
//윈도우-C드라이브/사용자/내문서, 맥-사용자/documents
//내문서 안에 플러그인 이름으로 하위폴더가 있어
//그 중 JucePlugin_Name (프로젝트 이름 : Fox Romana X ) 의 폴더가 프리셋 폴더야
mDirPreset(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile(JucePlugin_Name))
{
    resetFactoryPresets();
}
FoxPresetManager::~FoxPresetManager()
{
    
}

juce::String FoxPresetManager::getNameFactoryPreset(const int inIndex) const noexcept
{
    return mArrayFactoryPresets[inIndex].mName;
}
int FoxPresetManager::getNumFactoryPresets() const noexcept
{
    return mArrayFactoryPresets.size();
}

int FoxPresetManager::getFactoryPresetCurrent() const noexcept
{
    return mFactoryPresetCurrent;
}
void FoxPresetManager::setFactoryPreset(const int inIndex) noexcept
{
    mFactoryPresetCurrent = inIndex;
    mParameters.setParamsByFactoryPreset(mArrayFactoryPresets[mFactoryPresetCurrent]);
}

const juce::File& FoxPresetManager::getDirPreset() const noexcept
{
    return mDirPreset;
}

void FoxPresetManager::resetFactoryPresets() noexcept
{
    /**
     struct FactoryPreset
     {
         juce::String mName;
         float mTime[2]; //Delay time
         float mFeedback;
         float mMix;
         float mGain;
     };
     **/
    
    mArrayFactoryPresets.add({"Preset1", {111.0f, 111.0f}, 50.0f, 35.0f, -1.0f });
    mArrayFactoryPresets.add({"Preset2", {222.0f, 222.0f}, 50.0f, 35.0f, -1.0f });
    mArrayFactoryPresets.add({"Preset3", {333.0f, 333.0f}, 50.0f, 35.0f, -1.0f });
}
