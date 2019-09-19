#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "ShapeObjects.h"

class PixelHelper
{
    public:
        PixelHelper(ConfigurationProvider & configuration);
        virtual ~PixelHelper();

        void setup();

        PixelKind pixelKind() const;

        int pixelSize() const;
        void setPixel(uint8_t * memory, int n, Color color);
        Color getPixel(uint8_t * memory, int n);

        static inline uint8_t getRed(const Color & p)
        {
            return (uint8_t)(p >> 16);
        }

        static inline uint8_t getGreen(const Color & p)
        {
            return (uint8_t)(p >>  8);
        }

        static inline uint8_t getBlue(const Color & p)
        {
            return (uint8_t)p;
        }

        static inline uint8_t getWhite(const Color & p)
        {
            return (uint8_t)(p >> 24);
        }

        static inline void setRed(uint8_t value, Color & p)
        {
            p = (p & 0xFF00FFFF) | (value << 16);
        }

        static inline void setGreen(uint8_t value, Color & p)
        {
            p = (p & 0xFFFF00FF) | (value << 8);
        }

        static inline void setBlue(uint8_t value, Color & p)
        {
            p = (p & 0xFFFFFF00) | value;
        }

        static inline void setWhite(uint8_t value, Color & p)
        {
            p = (p & 0x00FFFFFF) | (value << 24);
        }

        static const Color brightenPixel(const Color & p, const int brightness);
        static const Color brightenRed(const Color & p, const int brightness);
        static const Color brightenGreen(const Color & p, const int brightness);
        static const Color brightenBlue(const Color & p, const int brightness);
        static const Color brightenWhite(const Color & p, const int brightness);
        
    private:
        ConfigurationProvider & _configuration;
        int _redOffset;
        int _greenOffset;
        int _blueOffset;
        int _whiteOffset;
};
