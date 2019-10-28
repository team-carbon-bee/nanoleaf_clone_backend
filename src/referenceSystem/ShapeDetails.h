#pragma once

#include <Arduino.h>

#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "Constants.h"

namespace referenceSystem
{

class ShapeDetails
{
    public:
        ShapeDetails(Shape * parent, ShapeHelper * shapeHelper) 
        : _parent(parent), _shapeHelper(shapeHelper), _animationObject(NULL), 
          _pixels(NULL), _ledCount(0)
        {
        }

        virtual ~ShapeDetails()
        {
        }

        void setup()
        {
            _ledCount = _shapeHelper->ledCountOfThisShape(_parent);
            _pixels = (Color *)malloc(sizeof(Color) * _ledCount);
            memset(_pixels, 0, _ledCount * sizeof(Color));
        }

        Color * pixels()
        {
            return _pixels;
        }

        int ledCount()
        {
            return _ledCount;
        }

        void clear()
        {
            fill(0);
        }

        void fill(const Color c)
        {
            for (int i = 0; i < _ledCount; ++i)
                _pixels[i] = c;
        }

        void animationObject(void * object)
        {
            _animationObject = object;
        }

        void * animationObject() const
        {
            return _animationObject;
        }

        void clearAnimationObject()
        {
            if (_animationObject != NULL)
            {
                free(_animationObject);
                _animationObject = NULL;
            }
        }

    private:
        Shape * _parent;
        ShapeHelper * _shapeHelper;
        void * _animationObject;
        Color * _pixels;
        //for performance issues
        int _ledCount;
};

} // referenceSystem