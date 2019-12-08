#pragma once

#include <Arduino.h>

#include "PixelHelper.h"

namespace ledDriver {

class ILedDriver
{
    public:
        ILedDriver() {}
        virtual ~ILedDriver() {}
        virtual void setup()=0;
        virtual void clear()=0;
        virtual void fill(const Color c)=0;
        virtual void setPixel(int pixelNumber, Color color)=0;
        virtual void setPixels(const Color * arraySource, const int offsetSource, const int len, const int offsetDest)=0;
        virtual uint32_t getPixel(int pixelNumber)=0;
        virtual int numPixels() const=0;
        virtual void show()=0;
        virtual void setBrightness(const double value) = 0;
        virtual double getBrightness() = 0;

};

}