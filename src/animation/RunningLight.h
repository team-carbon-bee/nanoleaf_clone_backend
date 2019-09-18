#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "IAnimation.h"
#include "referenceSystem/Linear.h"
#include "PixelHelper.h"

namespace animation
{

class RunningLight : public IAnimation
{
    public:
        RunningLight(referenceSystem::Linear & referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~RunningLight()
        {
        }

        void setup()
        {
            _referenceSystem.clear();
            _referenceSystem.driveLeds(); 
            _currentPosition = 0;
        }

        void loop()
        {
            //Before the fade we know that all leds are off
            int startFade = _currentPosition - TrailLength;

            //we ensure that the last led is really off (could happen due to round values based on 255 / trailLength)
            if (startFade - 1 < 0)
                _referenceSystem.setPixel(_referenceSystem.ledCount() + startFade - 1, 0);
            else
                _referenceSystem.setPixel(startFade - 1, 0);

            //we erase the end of the strip (for negative values)
            for (int i = startFade; i < 0; ++i)
            {
                Color current = _referenceSystem.getPixel(_referenceSystem.ledCount() + i);
                current = PixelHelper::brightenPixel(current, BrightnessStep);
                _referenceSystem.setPixel(_referenceSystem.ledCount() + i, current);
            }

            for (int i = max(startFade, 0); i < _currentPosition; ++i)
            {
                Color current = _referenceSystem.getPixel(i);
                current = PixelHelper::brightenPixel(current, BrightnessStep);
                _referenceSystem.setPixel(i, current);
            }

            //we go to the next led
            _referenceSystem.setPixel(_currentPosition, RunningColor);
            
            _currentPosition++;
            if (_currentPosition == _referenceSystem.ledCount())
                _currentPosition = 0;

            _referenceSystem.driveLeds();
        }

    private:
        referenceSystem::Linear & _referenceSystem;
        int _currentPosition;
        static const int TrailLength = 4;
        static const int BrightnessStep = -1 * 255 / TrailLength;
        static const Color RunningColor = 0xFF0000;

};

}