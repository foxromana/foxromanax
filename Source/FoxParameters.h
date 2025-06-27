/*
  ==============================================================================

    FoxParameters.h
    Created: 21 Feb 2025 8:34:17pm
    Author:  Fox Romana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FoxSmoother.h"

//class 10
struct FactoryPreset
{
    juce::String mName;
    float mTime[2]; //Delay time
    float mFeedback;
    float mMix;
    float mGain; 
};

namespace FoxParamIDs
{
    const juce::ParameterID Test("Test", 1);

    namespace Control
    {
        const juce::ParameterID Tempo("Tempo", 1);
        const juce::ParameterID Link("Link", 1);
    }

    namespace Output
    {
        // Gain : parameter name, 1: version ( logic pro requires version which is not 0 ! )
        const juce::ParameterID Gain("Gain", 1);
    
        //Lesson 6 - mix for delay
        const juce::ParameterID Mix("Mix", 1);
    }
    namespace Delay
    {
        const juce::ParameterID Time[2] {{"TimeL", 1}, {"TimeR", 1}};
        //박자를 선택할 수 있는 파라미터
        const juce::ParameterID Note[2] {{"NoteL", 1}, {"NoteR", 1}};
    }

    //Class 8
    namespace Feedback
    {
        const juce::ParameterID Amount("Amount", 1);
    }
}

//리스너 상속 : stereo link 체크박스 클릭시 엑션 위해
class FoxParameters : private juce::AudioProcessorValueTreeState::Listener,
                      private juce::Timer
{
    public:
        FoxParameters(juce::AudioProcessorValueTreeState& inApvts);
        ~FoxParameters() override;
        
        //for smoothe changing
        void prepare(const double inSampleRate) noexcept;
        void smoothen() noexcept;
        void update(const double inBpm = 120.0) noexcept;//normalize dB -> ratio ( -1 ~ 1 ) initialize Target value
        void reset() noexcept; //Initialize Current value
        
        //Gain
        float getValueGain() const noexcept; // no except : no exeption handling. because this function should be fast fast fast.
       
        //Test
        float getValueTest() const noexcept;
        
        static constexpr float kTimeMin = 5.0f;
        static constexpr float kTimeMax = 2000.0f;
        float getValueTime(const int inChannel) const noexcept;
        
        static juce::AudioProcessorValueTreeState::ParameterLayout initParameterLayout();
        
        //Lesson 6 - mix
        float getValueMix() const noexcept;
        
        //class 8 - feedback
        float getValueAmount() const noexcept;
    
        //class 11
        void setParamsByFactoryPreset(const FactoryPreset& inPreset) noexcept;
    
        //class 12
        juce::ValueTree getStateCopied() const noexcept;
        bool setParamsByValueTree(const juce::ValueTree& inState) noexcept;
    private:
        juce::AudioProcessorValueTreeState& mApvts;
        
        // this should be double to store .xxxxxxxxxxx many numbers
        double mSampleRate;
        
        //Gain
        juce::AudioParameterFloat* mParamGain; // for avoiding casting / mapping all the time. do this job beforehand.
        
        //smoothed Gain
        juce::LinearSmoothedValue<float> mValueGain;
        
        //float mValueGain; // ratio
            
        //Delay
        juce::AudioParameterFloat* mParamTime[2];
        
        //Lesson 5 - Delay - but zipper noise
        //float mValueTime[2];
        //Lesson 6 - Delay with exponential smoothing
        FoxSmoother mValueTime[2];
        
        //for testing Lesson 5 - linear smoothing
        juce::AudioParameterFloat* mParamTest;
        //juce::LinearSmoothedValue<float> mValueTest;

        
        //Lesson 6 - Exponential smoothing
        FoxSmoother mValueTest;
        
        //Lesson 6 - mix
        juce::AudioParameterFloat* mParamMix;
        juce::LinearSmoothedValue<float> mValueMix;
        
        //class 8 - feedback
        juce::AudioParameterFloat* mParamAmount;
        juce::LinearSmoothedValue<float> mValueAmount;
    
        //Control
        juce::AudioParameterBool* mParamTempo; //boolean
        juce::AudioParameterChoice* mParamNote[2]; //stereo라서 2개
    
        //인풋 BPM 기준으로 내가 고른 박자 노트 기준 딜레이 시간을 구하는 함수 
        double getTimeByNote(const double inBpm, const int inNote) const noexcept;
        
        juce::AudioParameterBool* mParamLink;
        void parameterChanged(const juce::String& inParamId, float inValue) override;
    
        //std::atomic처리 -> 누군가 write 할때 쓰지 않도록 경고 처리해주는 기능
        std::atomic<int> mChannelMaster;  //링크 기능 사용 시 마스터 (기준) 체널. 변화를 준 놈.( 나머지는 slave) 디폴트는 L
        std::atomic<bool> mFlagLinking; // 슬레이브 마스터 링크 중인지 아닌지 판별하는 플래그. 파라미터체인지 함수 안에서 강제로 파라미터 바꾸는 경우는 false 로 되어야 함
    void timerCallback() override;
    
    //safety
    //if there is no user's constructor, system create temporary copied constructor inside.
    //this function doesn't allow creating the copied constructor.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxParameters);
};
