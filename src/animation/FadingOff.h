#pragma once

#include <Arduino.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/DividedCounter.h"

namespace animation
{

class FadingOff : public IAnimation
{
    private:
        bool _finished;
        Color _backgroundColor;

    public:
        FadingOff() : _finished(false)
        {
        }

        virtual ~FadingOff()
        {
        }

        String name() const
        {
            return "Fading off";
        }

        void setup()
        {
            _finished = false;
        }

        //Called each time before starting animation
        void initialize()
        {
            _backgroundColor = 0;
        }

        //Called at the end of the animation
        virtual void deinitialize()
        {
        }
                
        //Determine if the animation can be ended by itself
        virtual bool canFinish() const
        {
            return true;
        }

        //Check if the animation has finished if it can false otherwise
        virtual bool isFinished() const
        {
            return _finished;
        }

        void loop()
        {
            _finished = true; //true if remains for all leds
            for (int i = 0; i < referenceSystem::LinearRef.ledCount(); ++i)
            {
                Color c = referenceSystem::LinearRef.getPixel(i);
                if (c != 0)
                {
                    _finished = false;
                    referenceSystem::LinearRef.setPixel(i, PixelHelper::brightenPixel(c, .5));
                }
            }
            referenceSystem::LinearRef.driveLeds();
        }
};

}