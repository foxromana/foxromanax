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

FoxPresetComboBox::FoxPresetComboBox(FoxPresetManager& inPresetManager)
:mPresetManager(inPresetManager)
{
    //initialization (clear all and add item all again)
    //resetByFactoryPreset();
    resetByXmlPreset();
    
    //초기에 띄울 텍스트 (선택 안함 = 인텍스 0)
    //setText("Select", juce::NotificationType::dontSendNotification);
    //눌렀을 때 띄울 아이템 추가
    //아이템 이름 아이템 번호 (인덱스 1부터) 요청
    //addItem("Item1", 1);
    //addItem("Item2", 2);
    //addItem("Item3", 3);
    
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
        
        if(id == 0)
        {
            return; //nothing selected
        }
        
        //하드코딩한 프리셋을 등록할 때
        //combo box 는 아이템 인덱스가 1 부터, mPresetManager 에서는 인덱스가 0부터
        //mPresetManager.setFactoryPreset(id - 1);
        
        //파일 탐색기 열기 - 파일 선택
        mFileChooser = std::make_unique<juce::FileChooser>(id == IdComboBoxPreset::Save ? "Save" : "Load",
                                                           mPresetManager.getDirPreset(), //프리셋 메니저 기본 파일 경로
                                                           "*.xml", // 파일확장자는 xml 만 취급
                                                           true); // false: Juce에서 쓰는 파일 탐색기를 사용/ true: 맥 전용 파일 탐색기 사용
        
        //saveMode 정의가봐 2의 배수로 enum 정의 되어 있음
        //비트 변환으로 복수 선택 가능하게
        //canSelectFiles도 되면서 saveMode 도 선택하게 하기 etc..
        int flag = juce::FileBrowserComponent::canSelectFiles;
        flag |= (id == IdComboBoxPreset::Save) ? juce::FileBrowserComponent::saveMode : juce::FileBrowserComponent::openMode;
        
        //on close callback 함수
        //탐색기 결과를 처리
        auto onClose = [this, id](const juce::FileChooser& inFileChooser)
        {
            //입력을 하지 않거나 x로 꺼버리면 빈 파일이름이 저장됨
            const juce::File result = inFileChooser.getResult();
            const juce::String nameResult = result.getFileName();
            if(nameResult.isNotEmpty())
            {
                
                switch (id)
                {
                    //현재 프리셋 저장
                    case IdComboBoxPreset::Save:
                    {
                        //value tree를 xml로 변환하기
                        mPresetManager.saveXmlPreset(result.withFileExtension("xml"));
                        break;
                    }
                    //저장된 프리셋 불러오기
                    case IdComboBoxPreset::Load:
                    {
                        //xml 파일을 가져와서 value tree로 변환하고 현재 파라미터에 적용
                        mPresetManager.loadXmlPreset(result);
                        break;
                    }
                }
            }
            
            this->resetByXmlPreset();
            
        };
        
        mFileChooser->launchAsync(flag, onClose);
        
    };

}

FoxPresetComboBox::~FoxPresetComboBox()
{
}

void FoxPresetComboBox::resetByFactoryPreset() noexcept
{
    //리셋 하기 전에 한번 클리어
    clear(juce::NotificationType::dontSendNotification);
    
    //다시 채우기
    for(int i=0 ; i< mPresetManager.getNumFactoryPresets() ;++i )
    {
        //combo box : i =0 : not selected, item id = from 1~
        addItem(mPresetManager.getNameFactoryPreset(i), i+1);
    }
    
    //이미 선택한게 있으면 콤보박스에 선택 시켜놔야해
    const int indexCurrent = mPresetManager.getFactoryPresetCurrent();
    if(indexCurrent >=0)
    {
        const int idItem = indexCurrent +1 ;
        //콤보박스를 선택상태로 두되, 콜백함수 (onChange) 를 부르지 않겠다.
        setSelectedId(idItem,juce::NotificationType::dontSendNotification);
    }
}

void FoxPresetComboBox::resetByXmlPreset() noexcept
{
    clear(juce::NotificationType::dontSendNotification);
    addItem("Save", IdComboBoxPreset::Save);
    addItem("Load", IdComboBoxPreset::Load);
    setText(mPresetManager.getXmlPresetCurrent(), juce::NotificationType::dontSendNotification);
}
