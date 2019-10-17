#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <LinkedList.h>

#include "IAnimation.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class RandomColoredShapes : public IAnimation
{
    public:
        RandomColoredShapes(referenceSystem::ShapeReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~RandomColoredShapes()
        {
        }

        std::string name()
        {
            return "Random colored shapes";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds();
            
            Serial.printf("Setup animations : %d", _referenceSystem->shapeList().size());

            //we iterate all shapes using linkedlist (no need to localize them)
            if (_referenceSystem->shapeList().moveToStart())
            {
                do 
                {
                    //each shape will embed a Fade for this animation
                    //Serial.println("cretaing");
                    Fade *f = new Fade();
                    //Serial.println("Adding");
                    _referenceSystem->getDetails(_referenceSystem->shapeList().getCurrent())->animationObject(f);
                    //Serial.println("Added");
                } while(_referenceSystem->shapeList().next());
            }

            Serial.println("Setup animations finished");
        }

        void loop()
        {
            //we iterate all shapes using linkedlist (no need to localize them)
            if (_referenceSystem->shapeList().moveToStart())
            {
                do 
                {
                    Shape * currentShape = _referenceSystem->shapeList().getCurrent();
                    Fade * currentFade = (Fade*)_referenceSystem->getDetails(currentShape)->animationObject();
                    if ((not currentFade->isConfigured()) || (currentFade->isFinished()))
                    {
                        Color src = _referenceSystem->getDetails(currentShape)->getPixel(0);
                        Color dst = PixelHelper::getRandomFullColorExcept(src);
                        int time = random(FadingSmallestDuration, FadingLongestDuration);
                        //We take the color of the first pixel of the random shape to set the source
                        
                        currentFade->configure(src, dst, time);
                    }
                    //after we apply each fade on each shape
                    Color c = currentFade->step();
                    _referenceSystem->getDetails(currentShape)->fill(c);

                } while(_referenceSystem->shapeList().next());
                _referenceSystem->driveLeds();
            }
        }

    private:
        referenceSystem::ShapeReferenceSystem * _referenceSystem;
        static const int FadingSmallestDuration = 5;
        static const int FadingLongestDuration = 20;
};

}