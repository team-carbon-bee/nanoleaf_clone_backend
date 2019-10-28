#pragma once

#include <Arduino.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/DividedCounter.h"

namespace animation
{

class PaintingLight : public IAnimation
{
    public:
        PaintingLight(referenceSystem::LinearReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~PaintingLight()
        {
        }

        std::string name()
        {
            return "Painting lights";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds(); 
            
            _foreColor = PixelHelper::getRandomFullColor();
            _backgroundColor = PixelHelper::getRandomFullColorExcept(_foreColor);
            _curPos = 0;
            _divider.configure(random(1, 3));
        }

        void loop()
        {
            if (_divider.step())
            {
                //we draw the painted part
                for (int i = 0;  i < _curPos; ++i)
                    _referenceSystem->setPixel(i, _foreColor);
                //we draw the background
                for (int i = _curPos; i < _referenceSystem->ledCount(); ++i)
                    _referenceSystem->setPixel(i, _backgroundColor);

                _curPos++;
                if (_curPos >= _referenceSystem->ledCount())
                {
                    _curPos = 0;
                    _backgroundColor = _foreColor;
                    _foreColor = PixelHelper::getRandomFullColorExcept(_backgroundColor);
                }

                _referenceSystem->driveLeds();
            }
        }

    private:

        referenceSystem::LinearReferenceSystem * _referenceSystem;
        Color _foreColor;
        Color _backgroundColor;
        int _curPos;
        DividedCounter _divider;
};

}