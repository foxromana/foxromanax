/*
  ==============================================================================

    FoxSmoother.cpp
    Created: 7 Mar 2025 9:29:57pm
    Author:  juyeon hong

  ==============================================================================
*/

#include "FoxSmoother.h"


FoxSmoother::FoxSmoother()
:mTarget(0.0), mCurrent(0.0), mCoefficient(0.0)
{
    
}
FoxSmoother::~FoxSmoother()
{
    
}
    
void FoxSmoother::reset(const double inRateHz, const double inTimeSec) noexcept
{
    //전기전자회로 - Capacitor : 오일러 법칙 e^x = exp(x)
    // x =inRateHz * inTimeSec
    //휴대폰 베터리 찰 때 0~80% 지점까지는 빨리 찬다
    mCoefficient = 1.0 - std::exp(-1.0/(inRateHz * inTimeSec));
}
void FoxSmoother::smoothen() noexcept
{
    //        exponential transition
    //        /-------------|
    //       ㅣ              ㄴ---------->
    //--------
    //current += (target - current) * coefficient
    mCurrent += (mTarget - mCurrent) * mCoefficient;
    
}

void FoxSmoother::setTarget(const double inValue) noexcept
{
    mTarget= inValue;
}

void FoxSmoother::setCurrent(const double inValue) noexcept
{
    mCurrent = inValue;
}

double FoxSmoother::getCurrent() const noexcept
{
    return mCurrent;
}
double FoxSmoother::getTarget() const noexcept
{
    return mTarget;
}
