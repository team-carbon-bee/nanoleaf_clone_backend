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
        void currentAnimation(animation::IAnimation * newAnimation);
        void enabled(const bool value);
        bool enabled() const;
        void previewAnimation(const uint8_t id);

    private:
        bool _enabled;
        animation::IAnimation * _currentAnimation;
        ledDriver::ILedDriver * _ledDriver;
        int _timeRemaining;
        bool _animationCanChange;
        ConfigurationProvider::EAnimationSelectionMethod _animationSelectionMethod;
        int _currentAnimationIndex;
        std::vector<uint8_t> _animationList;
        animation::FadingOff _fadingOffAnimation;

        void reloadConfiguration();
        animation::IAnimation * getAnimationById(const uint8_t & animationId);
        animation::IAnimation * getAnimationByMethod(const ConfigurationProvider::EAnimationSelectionMethod & animationMethod);

};

#if !defined(NO_GLOBAL_INSTANCES) 
extern Animator GlobalAnimator;
#endif