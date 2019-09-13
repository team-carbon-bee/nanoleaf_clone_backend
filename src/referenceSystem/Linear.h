#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "ConfigurationProvider.h"

namespace referenceSystem {

class Linear
{
    public:
        Linear(ConfigurationProvider configuration, ShapeHelper & shapeHelper, Adafruit_NeoPixel * ledDriver);
        virtual ~Linear();

        void setup();
        void driveLeds();
        uint32_t * pixels();
        //cached value for performance reasons
        int ledCount();
    private:
        ConfigurationProvider & _configuration;
        ShapeHelper & _shapeHelper;
        Adafruit_NeoPixel * _ledDriver;
        uint32_t * _pixels;
        int _ledCount;
};

} //referenceSystem