#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "ShapeObjects.h"

class ShapeHelper
{
    public:
        ShapeHelper(ConfigurationProvider & configuration);
        virtual ~ShapeHelper();

        int numberOfCconnections(const Shape * node);
        int shapeCount();
        int shapeCount(const Shape * node);
        int ledCount();
        int ledCount(const Shape * node);

    private:
        ConfigurationProvider & _configuration;
};