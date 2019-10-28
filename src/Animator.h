#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "AnimationFactory.h"

class Animator
{
    public:
        Animator(ConfigurationProvider * configuration, AnimationFactory * animationFactory);
        virtual ~Animator();

        void setup();
        void loop();

    private:
        ConfigurationProvider * _configuration;
        AnimationFactory * _animationFactory;
        animation::IAnimation * _currentAnimation;

};