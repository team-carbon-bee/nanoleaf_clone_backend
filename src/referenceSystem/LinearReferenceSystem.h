#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "IReferenceSystem.h"

#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ConfigurationProvider.h"
#include "ledDriver/ILedDriver.h"

namespace referenceSystem {

class LinearReferenceSystem : public IReferenceSystem
{
    public:
        LinearReferenceSystem(ConfigurationProvider * configuration, ledDriver::ILedDriver * ledDriver);
        virtual ~LinearReferenceSystem();

        void setup();
        void driveLeds();
        //cached value for performance reasons
        int ledCount();
        void clear();
        void fill(const Color c);

        void setPixel(int pixelNumber, Color color);
        uint32_t getPixel(int pixelNumber);
        
    private:
        ConfigurationProvider * _configuration;
        ledDriver::ILedDriver * _ledDriver;
};

} //referenceSystem