#pragma once

#include <Arduino.h>

#include "ledDriver/ILedDriver.h"
#include "animation/FadingOff.h"

class Animator
{
    public:
        Animator();
        virtual ~Animator();

        void setup();
        void handle();

    private:
        animation::IAnimation * _currentAnimation;
        ledDriver::ILedDriver * _ledDriver;
        int _timeRemaining;
        bool _animationCanChange;
        int _currentAnimationIndex;
        animation::FadingOff _fadingOffAnimation;

        animation::IAnimation * getAnimationById(const uint8_t & animationId);
        animation::IAnimation * getAnimationByMethod(const ConfigurationProvider::EAnimationSelectionMethod & animationMethod);

};

#if !defined(NO_GLOBAL_INSTANCES) 
extern Animator GlobalAnimator;
#endif