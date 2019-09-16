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
            
            //we set the first pixel to on
            _referenceSystem.setPixel(0, 0x0000FF00);
            _referenceSystem.driveLeds(); 
        }

        void loop()
        {
            //we save last pixel value
            uint32_t lastPx = _referenceSystem.getPixel(_referenceSystem.ledCount() - 1);
            //we shift all data by one
            for (int i = (_referenceSystem.ledCount() * _referenceSystem.pixelSize() - 1); i > 0; --i)
            {
                _referenceSystem.pixels()[i] = _referenceSystem.pixels()[i-_referenceSystem.pixelSize()];
            }
            //and restore the last pixel saved on first pixel
            _referenceSystem.setPixel(0, lastPx);
            _referenceSystem.driveLeds();
        }

    private:
        referenceSystem::Linear & _referenceSystem;

};

}