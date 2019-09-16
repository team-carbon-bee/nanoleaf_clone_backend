#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "IReferenceSystem.h"

#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ConfigurationProvider.h"

namespace referenceSystem {

class Linear : public IReferenceSystem
{
    public:
        Linear(ConfigurationProvider configuration, ShapeHelper & shapeHelper, PixelHelper & pixelHelper, Adafruit_NeoPixel * ledDriver);
        virtual ~Linear();

        void setup();
        void driveLeds();
        uint8_t * pixels();
        //cached value for performance reasons
        int ledCount();
        void clear();

        int pixelSize() const;
        void setPixel(int pixelNumber, uint32_t color);
        uint32_t getPixel(int pixelNumber);
        
    private:
        ConfigurationProvider & _configuration;
        ShapeHelper & _shapeHelper;
        PixelHelper & _pixelHelper;
        Adafruit_NeoPixel * _ledDriver;
        uint8_t * _pixels;
        int _ledCount;
};

} //referenceSystem