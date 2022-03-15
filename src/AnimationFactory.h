#pragma once

#include <Arduino.h>

#include "ledDriver/ILedDriver.h"
#include "LinkedList.h"
#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "animation/IAnimation.h"

class AnimationFactory
{
    public:
        AnimationFactory();
        virtual ~AnimationFactory();

        void setup(ledDriver::ILedDriver * ledDriver);
        LinkedList<animation::IAnimation *> & animations();
        void clearAnimationObject();
        
    private:
        ledDriver::ILedDriver * _ledDriver;
        LinkedList<animation::IAnimation*> _animations;

};

#if !defined(NO_GLOBAL_INSTANCES) 
extern AnimationFactory GlobalAnimationFactory;
#endif