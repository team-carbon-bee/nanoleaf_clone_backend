#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <NeoPixelBus.h>

#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "ILedDriver.h"

namespace ledDriver {

class NeoPixelBusLedDriver : public ILedDriver
{
    public:
        NeoPixelBusLedDriver();
        virtual ~NeoPixelBusLedDriver();

        void setup();
        
        void clear();
        void fill(const Color c);

        void setPixel(int pixelNumber, Color color);
        void setPixels(const Color * arraySource, const int offsetSource, const int len, const int offsetDest);
        uint32_t getPixel(int pixelNumber);

        int numPixels() const;
        void show();

        void setBrightness(const double value);
        double getBrightness();
    private:
        NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> _strip;
        Color * _buffer;
        double _brightness;

        RgbColor colorToNeoPixelBusColor(const Color & c);
        Color neoPixelBusColorToColor(const RgbColor & c);
};

} //referenceSystem