#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "IAnimation.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "PixelHelper.h"
#include "Fade.h"

namespace animation
{

class FullColoredShapeFading : public IAnimation
{
    public:
        FullColoredShapeFading(referenceSystem::ShapeReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~FullColoredShapeFading()
        {
        }

        std::string name()
        {
            return "Full colored shape fading";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds();
            Serial.println("after driveleds.loop");
            _targetColor = 0x000000;
            _counter = 0;
        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                //TODO : _referenceSystem->getRandomShape();
                Color dst = _colorList[_counter];
                _fade.configure(_targetColor, dst, FadingDuration);
                _targetColor = dst;

                _counter++;
                if (_counter == 6)
                    _counter = 0;
            }
            
            Color c = _fade.step();
            _referenceSystem->getDetails(_referenceSystem->assembly()->connections[0])->fill(c);

            _referenceSystem->driveLeds();
        }

    private:
        referenceSystem::ShapeReferenceSystem * _referenceSystem;
        Fade _fade;
        Color _targetColor;
        int _counter;
        static const int FadingDuration = 100;
        const Color _colorList[6] = {0x0000FF, 0x00FFFF, 0x00FF00, 0XFFFF00, 0xFF0000, 0xFF00FF};

};

}