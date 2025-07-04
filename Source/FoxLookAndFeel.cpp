/*
  ==============================================================================

    FoxLookAndFeel.cpp
    Created: 27 Jun 2025 9:08:46pm
    Author:  juyeon hong

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FoxLookAndFeel.h"
#include "FoxSize.h"

//==============================================================================
FoxLookAndFeel::FoxLookAndFeel()
: mFontOption(juce::Typeface::createSystemTypefaceFor(BinaryData::LatoMedium_ttf, BinaryData::LatoMedium_ttfSize))
{
    //배경색
    setColour(juce::ResizableWindow::backgroundColourId, FoxColors::Main::background);
    
    //group component
    //그룹 제목 글자
    setColour(juce::GroupComponent::textColourId, FoxColors::Panel::title);
    //그룹 박스 선
    setColour(juce::GroupComponent::outlineColourId, FoxColors::Panel::outline);
    
    //레이블
    setColour(juce::Label::textColourId, FoxColors::RotaryKnob::label);
    //버튼 색
    setColour(juce::TextButton::textColourOffId, FoxColors::Button::text);
    setColour(juce::TextButton::textColourOnId, FoxColors::Button::textToggled);
    setColour(juce::TextButton::buttonColourId, FoxColors::Button::background);
    setColour(juce::TextButton::buttonOnColourId, FoxColors::Button::backgroundToggled);
    
    //콤보박스 색
    setColour(juce::ComboBox::backgroundColourId, FoxColors::ComboBox::background);
    setColour(juce::ComboBox::textColourId, FoxColors::ComboBox::text);
    setColour(juce::ComboBox::outlineColourId, FoxColors::ComboBox::outline);
    setColour(juce::ComboBox::arrowColourId, FoxColors::ComboBox::arrow);
    
    //콤보 박스 열었을 때 뜨는 팝업메뉴 색
    setColour(juce::PopupMenu::backgroundColourId, FoxColors::PopupMenu::background);
    setColour(juce::PopupMenu::textColourId, FoxColors::PopupMenu::text);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, FoxColors::PopupMenu::backgroundHighlighted);
    setColour(juce::PopupMenu::highlightedTextColourId, FoxColors::PopupMenu::textHighlighted);
    
    //Slider - RotaryKnob 스타일 : 똥그란 스타일
    //textBoxTextColourId 슬라이더 밑 텍스트박스 섹
    setColour(juce::Slider::textBoxTextColourId, FoxColors::RotaryKnob::label);
    //슬라이더 엑시브한 영역의 색
    setColour(juce::Slider::rotarySliderFillColourId, FoxColors::RotaryKnob::trackActive);
    //슬라이더 밑 텍스트 박스안의 현재 위치를 알려주는 커서
    setColour(juce::CaretComponent::caretColourId, FoxColors::RotaryKnob::caret);
    
}

FoxLookAndFeel::~FoxLookAndFeel()
{
}


juce::Font FoxLookAndFeel::getLabelFont(juce::Label& inLabel)
{
    //label 그릴 때 어떤 폰트를 쓸건지 알려주는 함수. 이미 저장된 함수인데,
    //여기서 재정의
    
    //default - 원본 폰트 부르는 함수
    //return juce::LookAndFeel_V4::getLabelFont(inLabel);
    
    //폰트의 높이 (사이즈)를 정하는 함수
    //withMetricsKind 함수 : 옛날 형힉으로 변환해주는 함수 ( Font -> FontOption 형식으로) 
    return mFontOption.withMetricsKind(juce::TypefaceMetricsKind::legacy).withHeight(FoxSize::Font::Height);
    
}

juce::Font FoxLookAndFeel::getPopupMenuFont()
{
    //원래 디폴트 폰트
    //return juce::LookAndFeel_V4::getPopupMenuFont();
    return mFontOption.withMetricsKind(juce::TypefaceMetricsKind::legacy).withHeight(FoxSize::Font::Height);
}


//슬라이더 직접 디자인하기
void FoxLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                      float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                      juce::Slider& inSlider)
{
    //원본 스타일 호출
    //juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, inSlider);
    //--------------
    
    //맨땅에서 그리기를 직접 하는 경우
    //1. bound Local - 영역 지정을 먼저 한다. x,y = 가로세로 길이
    const juce::Rectangle<float> boundSlider = juce::Rectangle<int>(x,y,width,height).toFloat(); //사각형 그리기
    g.setColour(juce::Colours::red);
    g.drawRect(boundSlider);
    
    //2. bound square - 안에 초록색 정사각형 영역 그리기. 가로/세로를 똑같게 맞추기
    //가로,세로 중 짧은 애를 구하기
    const int length = width > height ? height : width;
    //길이를 정한 상태에서 사이즈를 85%로 줄인다.
    const float lengthSquare = (float)length * 0.85f;
    //새로운 정사각형 그리기 ( 센터를 유지하면서 크기만 0.85 만큼 줄이기 )
    const juce::Rectangle<float> boundSquare = boundSlider.withSizeKeepingCentre(lengthSquare, lengthSquare);
    g.setColour(juce::Colours::green);
    g.drawRect(boundSquare);
    
    //3. boundknob  안쪽에 파란 사각형 그리기, 더 작은 사이즈
    //전체 길이의 15% (0.15)만큼 줄인다. 상대적
    const juce::Rectangle<float> boundKnob = boundSquare.reduced(boundSquare.getWidth()*0.15f);
    g.setColour(juce::Colours::blue);
    g.drawRect(boundKnob);
    
    //4. boundknob 사이즈에 맞춰 원 그리기
    //4-1 dropShadow 그림자 그리기
    juce::Path path;
    path.addEllipse(boundKnob);
    juce::DropShadow dropShadow(FoxColors::RotaryKnob::dropShadow, 10, {0, 3});
    dropShadow.drawForPath(g, path);
    
    //원그리기
    g.setColour(FoxColors::RotaryKnob::outline);
    g.fillEllipse(boundKnob);
    
    //5. gradiant - 원 안에 그라데이션 효과 주기
    const juce::Rectangle<float> boundInner = boundKnob.reduced(boundKnob.getWidth()*0.05f);
    const juce::ColourGradient gradient(FoxColors::RotaryKnob::gradientTop, 0.0f, boundInner.getY(),
                                        FoxColors::RotaryKnob::gradientBottom, 0.0f, boundInner.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillEllipse(boundInner);
    
    //6. track background
    //선이 어디 안에 위치할지 사이즈/중심 지정
    //초록색 사각형의 반지름
    const float radiusSquare = boundSquare.getWidth() * 0.5f;
    //선의 두께
    const float thicknessTrack = radiusSquare * 0.1f;
    //초록색 사각형 안에 트랙이 들어오도록, 트랙의 반지름을 구하기
    const float radiusTrack = radiusSquare - (thicknessTrack * 0.5f);
    
    //선을 그릴 땐 juce::Path 를 사용
    //선 초기화
    path.clear();
    //중심 기준으로 선 긋기
    path.addCentredArc(boundSquare.getCentreX(), boundSquare.getCentreY(),
                       radiusTrack, radiusTrack, 0.0f, // 0.0f -> 몇도씩 회전시킬건지.
                       rotaryStartAngle, rotaryEndAngle, true); // 시작-끝 앵글 (FoxRotaryKnob.cpp 의 생성자에 degree 정의해놨음, true: 새로 그린다, false: 붓을 안떼고 이어서 그리는 모드
    //선의 성격 정하기. curved : 선이 꺾일때 뭉둥하게 꺾일지 아니면 각지게 그릴지, rounded : 선 양끝을 어떻게 그릴까(EndCapStyle) - 둥그렇게 그리기.
    const juce::PathStrokeType strokeType(thicknessTrack,
                                          juce::PathStrokeType::curved,
                                          juce::PathStrokeType::rounded);
    g.setColour(FoxColors::RotaryKnob::trackBackground);
    g.strokePath(path, strokeType);
    
    //눈금 = 다이어리 = 현재 위치에 따라 위치가 바뀜
    //근데 위치정보는 바로 못받고, sliderPosProportional (비율)로밖에 현재 위치를 알 수가 없어
    //시작 각도 - 끝 각도를 아니까 이 사이의 비율을 통해 x,y 를 구하기S$
    
    //7. 다이얼
    //juce 기준 현재값의 각도
    const float angleSliderPos = rotaryStartAngle + sliderPosProportional * ( rotaryEndAngle - rotaryStartAngle);
    
    //수학 기준 현재값의 각도
    const float theta = angleSliderPos - juce::MathConstants<float>::halfPi;
    
    //파란색 사각형의 반지름 구하기
    const float radiusDial = boundInner.getWidth()* 0.5f;
    
    //R (선: 시작점, 끝점) 구하기
    const float R1 = radiusDial * 0.5f;
    const float R2 = radiusDial * 0.85f;
    
    //식에 적용
    
    //시작점 - 반지름의 반 (0.5)
    const juce::Point<float> pos1(boundInner.getCentreX() + R1 * std::cos(theta),
                                  boundInner.getCentreX() + R1 * std::sin(theta));

    //끝점 - 반지름의 거의 끝 (0.85)
    const juce::Point<float> pos2(boundInner.getCentreX() + R2 * std::cos(theta),
                                  boundInner.getCentreX() + R2 * std::sin(theta));
    
    path.clear();
    path.startNewSubPath(pos1);
    path.lineTo(pos2);
    g.setColour(FoxColors::RotaryKnob::dial);
    g.strokePath(path, strokeType);
    
}
