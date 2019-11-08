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
        FullColoredFading()
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
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds();
            _srcColor = 0x000000;

        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                Color dst = PixelHelper::getRandomFullColorExcept(_srcColor);
                _fade.configure(_srcColor, dst, FadingDuration);
                _srcColor = dst;
            }

            Color c = _fade.step();
            referenceSystem::LinearRef.fill(c);
            referenceSystem::LinearRef.driveLeds();
        }

    private:
        Fade _fade;
        Color _srcColor;
        static const int FadingDuration = 50;

};

}