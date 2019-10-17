#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "IAnimation.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class FullColoredShapeFading : public IAnimation
{
    public:
        FullColoredShapeFading(referenceSystem::ShapeReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem), _currentShape(NULL)
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
        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                //We manage to get randomly a new shape (not the last one)
                _currentShape = _referenceSystem->getRandomShapeExcept(_currentShape);
                Color src = _referenceSystem->getDetails(_currentShape)->getPixel(0);
                Color dst = PixelHelper::getRandomFullColorExcept(src);
                
                //We take the color of the first pixel of the random shape to set the source
                _fade.configure(src, dst, FadingDuration);
            }
            
            Color c = _fade.step();
            _referenceSystem->getDetails(_currentShape)->fill(c);

            _referenceSystem->driveLeds();
        }

    private:
        referenceSystem::ShapeReferenceSystem * _referenceSystem;
        Fade _fade;
        Shape * _currentShape;
        static const int FadingDuration = 10;
};

}