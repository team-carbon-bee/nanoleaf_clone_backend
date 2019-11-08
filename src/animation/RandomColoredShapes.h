#pragma once

#include <Arduino.h>

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
        RandomColoredShapes()
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
            referenceSystem::ShapeRef.clear();
            referenceSystem::ShapeRef.driveLeds();
            
            Serial.printf("Setup animations : %d", referenceSystem::ShapeRef.shapeList().size());

            //we iterate all shapes using linkedlist (no need to localize them)
            if (referenceSystem::ShapeRef.shapeList().moveToStart())
            {
                do 
                {
                    //each shape will embed a Fade for this animation
                    Fade *f = new Fade();
                    referenceSystem::ShapeRef.getShape(referenceSystem::ShapeRef.shapeList().getCurrent())->animationObject(f);
                } while(referenceSystem::ShapeRef.shapeList().next());
            }

            Serial.println("Setup animations finished");
        }

        void loop()
        {
            //we iterate all shapes using linkedlist (no need to localize them)
            if (referenceSystem::ShapeRef.shapeList().moveToStart())
            {
                do 
                {
                    Shape * currentShape = referenceSystem::ShapeRef.shapeList().getCurrent();
                    Fade * currentFade = (Fade*)referenceSystem::ShapeRef.getShape(currentShape)->animationObject();
                    if ((not currentFade->isConfigured()) || (currentFade->isFinished()))
                    {
                        Color src = referenceSystem::ShapeRef.getShape(currentShape)->pixels()[0];
                        Color dst = PixelHelper::getRandomFullColorExcept(src);
                        int time = random(FadingSmallestDuration, FadingLongestDuration);
                        //We take the color of the first pixel of the random shape to set the source
                        
                        currentFade->configure(src, dst, time);
                    }
                    //after we apply each fade on each shape
                    Color c = currentFade->step();
                    referenceSystem::ShapeRef.getShape(currentShape)->fill(c);

                } while(referenceSystem::ShapeRef.shapeList().next());
                referenceSystem::ShapeRef.driveLeds();
            }
        }

    private:
        static const int FadingSmallestDuration = 5;
        static const int FadingLongestDuration = 20;
};

}