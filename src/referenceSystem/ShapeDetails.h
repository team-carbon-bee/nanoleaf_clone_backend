#pragma once

#include <Arduino.h>

#include "ShapeHelper.h"
#include "PixelHelper.h"

namespace referenceSystem
{

class ShapeDetails
{
    public:
        ShapeDetails(Shape * parent, ShapeHelper & shapeHelper, PixelHelper & pixelHelper) 
        : _parent(parent), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper)
        {
        }

        virtual ~ShapeDetails()
        {
        }

        void setup()
        {
            _ledCount = _shapeHelper.ledCountOfThisShape(_parent);
            _pixels = (uint8_t *)malloc(sizeof(uint8_t) * _pixelHelper.pixelSize() * _ledCount);
            memset(_pixels, 0, _ledCount * _pixelHelper.pixelSize() * sizeof(uint8_t));
        }

        uint8_t * pixels()
        {
            return _pixels;
        }

        int ledCount()
        {
            return _ledCount;
        }

        void clear()
        {
            memset(_pixels, 0, _ledCount * sizeof(uint8_t) * _pixelHelper.pixelSize());
        }

        void fill(const Color c)
        {
            for (int i = 0; i < _ledCount; ++i)
            {
                setPixel(i, c);
            }
        }

        int pixelSize() const
        {
            return _pixelHelper.pixelSize();
        }

        void setPixel(int pixelNumber, uint32_t color)
        {
            _pixelHelper.setPixel(_pixels, pixelNumber, color);
        }

        uint32_t getPixel(int pixelNumber)
        {
            return _pixelHelper.getPixel(_pixels, pixelNumber);
        }

    private:
        Shape * _parent;
        ShapeHelper & _shapeHelper;
        PixelHelper & _pixelHelper;
        uint8_t * _pixels;
        //for performance issues
        int _ledCount;
};

} // referenceSystem