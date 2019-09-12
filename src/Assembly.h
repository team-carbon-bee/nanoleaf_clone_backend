#pragma once

#include <Arduino.h>

#include "ShapeObjects.h"

class Assembly
{
    public:
        Assembly() : _root(NULL)
        {}

        virtual ~Assembly()
        {}

        Shape * root()
        {
            return _root;
        }

        void root(Shape * value)
        {
            _root = value;
        }

    private:
        Shape * _root;

};