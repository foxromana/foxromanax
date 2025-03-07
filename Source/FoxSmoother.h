/*
  ==============================================================================

    FoxSmoother.h
    Created: 7 Mar 2025 9:29:57pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

// one pole filter
//target, current, coefficient 가 필요
//current = current * ( 1 - coeffiencent) + target * coefficient
//current = current + target * coefficient - current * coefficient
//current = current + (target - current ) * coefficient
//current += (target - current) * coefficient

