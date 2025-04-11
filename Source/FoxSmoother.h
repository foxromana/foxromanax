/*
  ==============================================================================

    FoxSmoother.h
    Created: 7 Mar 2025 9:29:57pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
// one pole filter
//target, current, coefficient 가 필요
//current = current * ( 1 - coeffiencent) + target * coefficient
//current = current + target * coefficient - current * coefficient
//current = current + (target - current ) * coefficient
//current += (target - current) * coefficient

class FoxSmoother
{
    public:
    FoxSmoother();
    ~FoxSmoother();
        
    void reset(const double inRateHz, const double inTimeSec) noexcept;
    void smoothen() noexcept;
    
    void setTarget(const double inValue) noexcept;
    void setCurrent(const double inValue) noexcept;
    
    double getCurrent() const noexcept;
    double getTarget() const noexcept;
    
    private:
        double mTarget;
        double mCurrent;
        double mCoefficient;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FoxSmoother)
};
