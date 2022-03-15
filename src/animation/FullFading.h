#pragma once

#include <Arduino.h>

#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class FullFading : public IAnimation
{
    public:
        FullFading()
        {
        }

        virtual ~FullFading()
        {
        }

        String name() const
        {
            return "Full fading";
        }

        uint8_t id() const
        {
            return 13;
        }

        void setup()
        {
        }

        //Called each time before starting animation
        void initialize()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds();
            _targetColor = 0x000000;
        }

        //Called at the end of the animation
        virtual void deinitialize()
        {
        }
                
        //Determine if the animation can be ended by itself
        virtual bool canFinish() const
        {
            return false;
        }

        //Check if the animation has finished if it can false otherwise
        virtual bool isFinished() const
        {
            return false;
        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                Color dst = esp_random();
                _fade.configure(_targetColor, dst, FadingDuration);
                _targetColor = dst;
            }
            
            Color c = _fade.step();

            referenceSystem::LinearRef.fill(c);
            referenceSystem::LinearRef.driveLeds();
        }

    private:
        Fade _fade;
        Color _targetColor;
        static const int FadingDuration = 100;


};

}