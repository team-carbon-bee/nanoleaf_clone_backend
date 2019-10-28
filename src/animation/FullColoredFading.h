#pragma once

#include <Arduino.h>

#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class FullColoredFading : public IAnimation
{
    public:
        FullColoredFading(referenceSystem::LinearReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~FullColoredFading()
        {
        }

        std::string name()
        {
            return "Full colored fading";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds();
            _srcColor = 0x000000;

        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                Serial.printf("step\n");
                Color dst = PixelHelper::getRandomFullColorExcept(_srcColor);
                _fade.configure(_srcColor, dst, FadingDuration);
                _srcColor = dst;
            }

            Color c = _fade.step();
            _referenceSystem->fill(c);
            _referenceSystem->driveLeds();
        }

    private:
        referenceSystem::LinearReferenceSystem * _referenceSystem;
        Fade _fade;
        Color _srcColor;
        static const int FadingDuration = 50;

};

}