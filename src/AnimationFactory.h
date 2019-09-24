#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "LinkedList.h"
#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ConfigurationProvider.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "animation/IAnimation.h"

class AnimationFactory
{
    public:
        AnimationFactory(ConfigurationProvider & configuration, ShapeHelper & shapeHelper, 
                         PixelHelper & pixelHelper, Adafruit_NeoPixel * ledDriver);
        virtual ~AnimationFactory();

        void setup();

        LinkedList<animation::IAnimation *> & animations();
    private:
        ConfigurationProvider & _configuration;
        ShapeHelper & _shapeHelper;
        PixelHelper & _pixelHelper;
        Adafruit_NeoPixel * _ledDriver;
        referenceSystem::LinearReferenceSystem _linearReferenceSystem;
        referenceSystem::ShapeReferenceSystem _shapeReferenceSystem;
        LinkedList<animation::IAnimation*> _animations;

};