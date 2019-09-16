#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "ShapeObjects.h"

class PixelHelper
{
    public:
        PixelHelper(ConfigurationProvider & configuration);
        virtual ~PixelHelper();

        PixelKind pixelKind() const;

        int pixelSize() const;
        void setPixel(uint8_t * memory, int pixelNumber, uint32_t color);
        uint32_t getPixel(uint8_t * memory, int pixelNumber);
    private:
        ConfigurationProvider & _configuration;
        int _pixelSize;
};
