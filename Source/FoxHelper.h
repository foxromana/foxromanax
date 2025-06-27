/*
  ==============================================================================

    FoxHelper.h
    Created: 18 Apr 2025 8:32:07pm
    Author:  juyeon hong

  ==============================================================================
*/

//고막 보호 함수. 소리 맥스 안찍게 만들기
#pragma once

#include <JuceHeader.h>

namespace FoxHelper
{
    //버튼 아이디 식별
    enum CmdIdButtons
    {
        Tempo, // 0
        Link //1 
    };

    inline void protectEars(juce::AudioBuffer<float>& inBuffer)
    {
        bool warning = true;
        //각 채널의
        for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
        {
            //샘플 데이터를 모두 확인
            const float* channelData = inBuffer.getReadPointer(channel);
            for (int i = 0; i < inBuffer.getNumSamples(); ++i)
            {
                bool shouldSilence = false;
                const float sample = channelData[i];
                if (std::isnan(sample)) //NaN = Not a Number. 숫자가 아닌 괴이한 값이 들어온 경우 - 애초에 오디오 샘플이 아님
                {
                    DBG("!!! SILENCING: nan detected in audio buffer !!!");
                    shouldSilence = true; //소리를 걍 꺼버려
                }
                else if (std::isinf(sample)) //Infinate -> x/0.000001 -> 무한대값 나오는 경우는 이것도 표현 불가능한 샘플
                {
                    DBG("!!! SILENCING: inf detected in audio buffer !!!");
                    shouldSilence = true; //소리를 걍 꺼버려
                }
                else if (sample < -2.0f || sample > 2.0f) // screaming feedback (> 6dB) 샘플값이 -2~2 사이인 경우. 허용범위 2배인 경우 = 6dB보다 큰 샘플이 발생한 경우 ( = Gain 값을 높여버림)
                {
                    DBG("!!! SILENCING: sample out of range (> 6dB) !!!");
                    shouldSilence = true; //소리를 걍 꺼버려
                }
                else if (sample < -1.0f || sample > 1.0f) //쪼끔 크긴한데.. 워닝으로 넘기기
                {
                    if (warning == true)
                    {
                        DBG("!!! WARNING: sample out of range (> 0dB) !!!");
                        warning = false; //소리를 끄지는 말고 워닝만 하기
                    }
                }
                
                if (shouldSilence)
                {
                    //소리를 걍 꺼버려
                    inBuffer.clear();
                    return;
                }
            }
        }
    }
}
