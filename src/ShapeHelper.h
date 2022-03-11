#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "PixelHelper.h"

class ShapeHelper
{
    public:
        ShapeHelper();
        virtual ~ShapeHelper();

        int numberOfConnections(const Shape * node);
        int shapeCount();
        int shapeCount(const Shape * node);
        Shape * duplicateShape(Shape * node, Shape * parent = NULL);
        //Returns the total number of leds of the system
        int ledCount();
        //Returns the number of leds of the current node and all of its childs
        int ledCount(const Shape * node);
        //Return the number of leds of this shape only (based on its kind)
        int ledCountOfThisShape(const Shape * node);

    private:
};

#if !defined(NO_GLOBAL_INSTANCES)
extern ShapeHelper GlobalShapeHelper;
#endif