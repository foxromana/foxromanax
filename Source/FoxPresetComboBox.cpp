/*
  ==============================================================================

    FoxPresetComboBox.cpp
    Created: 2 May 2025 8:55:52pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FoxPresetComboBox.h"

//==============================================================================

FoxPresetComboBox::FoxPresetComboBox()
{
    //초기에 띄울 텍스트 (선택 안함 = 인텍스 0)
    setText("Select", juce::NotificationType::dontSendNotification);
    
    //눌렀을 때 띄울 아이템 추가
    //아이템 이름 아이템 번호 (인덱스 1부터) 요청
    addItem("Item1", 1);
    addItem("Item2", 2);
    addItem("Item3", 3);
    
    // std::function<void()> onChange;
    // onChange 에 내 함수를 다이렉트로 저장하면 콜백함수화 됨
    // 이런 걸 function object 라고 함
    // 람다 함수를 만들어서 등록해야 한단다

    //람다 함수 기초
    // [](){}() = 4개의 괄호로 만들어져있다
    // [] : 캡쳐 -> 매개변수 자리에 들어올 수 없는 외부 데이터
    // () : 매개변수
    // {} : 함수 내용
    // () : 매개변수를 직접 넣어서 바로 호출해버리기.
    // 람다 함수를 저장하는 방법 :
    // auto myLambda = [](int num)
    // { 함수 내용 };
    // auto : 자료형을 모를 때 저장할 자료의 자료형으로 저장
    //콜백 함수
    onChange = [this]()
    {
        //선택한 아이디 찾기
        const int id = this->getSelectedId();
        const juce::String text = this->getText();
        DBG(id <<":" <<text);
    };

}

FoxPresetComboBox::~FoxPresetComboBox()
{
}
