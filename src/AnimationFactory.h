#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "LinkedList.h"
#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "ConfigurationProvider.h"
#include "referenceSystem/Linear.h"
#include "animation/IAnimation.h"

class AnimationFactory
{
    public:
        AnimationFactory(ConfigurationProvider & configuration, ShapeHelper & shapeHelper, 
                         Adafruit_NeoPixel * ledDriver);
        virtual ~AnimationFactory();

        void setup();

        LinkedList<animation::IAnimation*> & animations();
    private:
        ConfigurationProvider & _configuration;
        ShapeHelper & _shapeHelper;
        Adafruit_NeoPixel * _ledDriver;
        referenceSystem::Linear _linearReferenceSystem;
        LinkedList<animation::IAnimation*> _animations;

};