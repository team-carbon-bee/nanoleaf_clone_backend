#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "IAnimation.h"
#include "referenceSystem/Linear.h"

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
            //TODO : add clear method
            memset(_referenceSystem.pixels(), 0, _referenceSystem.ledCount() * sizeof(uint32_t));
            //we set the first pixel to on
            _referenceSystem.pixels()[0] = 0x0000FF;
        }

        void loop()
        {
            Serial.println("runningLight.loop()");
            //we save last pixel value
            uint32_t lastPx = _referenceSystem.pixels()[_referenceSystem.ledCount() - 1];
            //we shift all data by one
            memcpy((void*)(_referenceSystem.pixels() + (1 * sizeof(uint32_t))), _referenceSystem.pixels(), _referenceSystem.ledCount() - 1);
            //and restore the last pixel saved on first pixel
            _referenceSystem.pixels()[0] = lastPx;
            _referenceSystem.driveLeds();
        }

    private:
        referenceSystem::Linear & _referenceSystem;

};

}