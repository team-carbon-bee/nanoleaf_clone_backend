#pragma once

#include <Arduino.h>

#include "ledDriver/ILedDriver.h"

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

};

#if !defined(NO_GLOBAL_INSTANCES) 
extern Animator GlobalAnimator;
#endif