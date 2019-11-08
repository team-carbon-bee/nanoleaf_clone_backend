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

        std::string name()
        {
            return "Full fading";
        }

        void setup()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds();
            _targetColor = 0x000000;
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