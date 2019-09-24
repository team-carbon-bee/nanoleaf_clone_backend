#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "Fade.h"

namespace animation
{

class IndependantRandomFading : public IAnimation
{
    public:
        IndependantRandomFading(referenceSystem::LinearReferenceSystem & referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~IndependantRandomFading()
        {
        }

        std::string name()
        {
            return "Independant random fading";
        }

        void setup()
        {
            _referenceSystem.clear();
            _referenceSystem.driveLeds();

            _fades = (Fade*)malloc(sizeof(Fade) * _referenceSystem.ledCount());
            for (int i = 0; i < _referenceSystem.ledCount(); ++i)
                _fades[i] = Fade();
        }

        void loop()
        {
            for (int i = 0; i < _referenceSystem.ledCount(); ++i)
            {    
                if ((not _fades[i].isConfigured()) || (_fades[i].isFinished()))
                {
                    Color dst = esp_random();
                    //duration taken randomly between 50 and 114 ticks --> rounded to 64
                    int duration = (esp_random() & 0x3F) + 50;
                    _fades[i].configure(_fades[i].target(), dst, duration);
                }
                _referenceSystem.setPixel(i, _fades[i].step());
            }    

            _referenceSystem.driveLeds();
        }

    private:
        referenceSystem::LinearReferenceSystem & _referenceSystem;
        Fade * _fades;        

};

}