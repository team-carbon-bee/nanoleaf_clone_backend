#pragma once

#include <Arduino.h>

#include "ledDriver/ILedDriver.h"
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
        AnimationFactory(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, 
                         ledDriver::ILedDriver * ledDriver);
        virtual ~AnimationFactory();

        void setup();

        LinkedList<animation::IAnimation *> & animations();
        void clearAnimationObject();
        
    private:
        ConfigurationProvider * _configuration;
        ShapeHelper * _shapeHelper;
        ledDriver::ILedDriver * _ledDriver;
        referenceSystem::LinearReferenceSystem * _linearReferenceSystem;
        referenceSystem::ShapeReferenceSystem * _shapeReferenceSystem;
        LinkedList<animation::IAnimation*> _animations;

};