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
mDirPreset(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile(JucePlugin_Name)),
mIdState("Preset")
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

const juce::String& FoxPresetManager::getXmlPresetCurrent() const noexcept
{
    return mXmlPresetCurrent;
}

void FoxPresetManager::setXmlPresetCurrent(const juce::String& inNamePreset) noexcept
{
    mXmlPresetCurrent = inNamePreset;
}

void FoxPresetManager::saveXmlPreset(const juce::File& inFileXml) noexcept
{
    //빈파일 정보 가져와서 채우기
    
    //인풋 파일의 파일 작성하는 함수
    //현재의 파라미터 정보 (mParameters 벨류 트리)를 모두 긁어와 파일 안에 xml 형식으로 저장
    const juce::ValueTree stateCopied = mParameters.getStateCopied();
    
    //value tree 를 xml 형식으로 변환
    std::unique_ptr<juce::XmlElement> xmlContents = stateCopied.createXml();
    if(xmlContents==nullptr)
    {
        return;
    }
    
    //파일에 이 xml 내용을 작성
    if( xmlContents->writeTo(inFileXml) == false)
    {
        return;
    }
    
    //파일을 로컬 위치에 생성
    if(inFileXml.create().failed())
    {
        return;
    }
    
    //확장자 뺀 이름을 현재 프리셋 이름으로 저장
    setXmlPresetCurrent(inFileXml.getFileNameWithoutExtension());
    
}

void FoxPresetManager::loadXmlPreset(const juce::File& inFileXml) noexcept
{
    //채워져있는 파일 가져와서 읽고 value tree로 변환
    std::unique_ptr<juce::XmlElement> xmlfile = juce::XmlDocument::parse(inFileXml);
    if(xmlfile == nullptr)
    {
        return;
    }
    
    //xml -> value tree
    const juce::ValueTree presetState = juce::ValueTree::fromXml(*xmlfile);
    if(presetState.isValid() == false)
    {
        return;
    }
    
    //현재 파라미터에 적용해라
    if(mParameters.setParamsByValueTree(presetState) == true)
    {
        setXmlPresetCurrent(inFileXml.getFileNameWithoutExtension());
    }
}


const juce::Identifier& FoxPresetManager::getIdState() const noexcept
{
    return mIdState;
}

juce::ValueTree FoxPresetManager::getState() const noexcept
{
    juce::ValueTree state(getIdState());
    state.setProperty("name", getXmlPresetCurrent(), nullptr);
    
    return state;
}
