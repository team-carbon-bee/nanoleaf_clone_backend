#pragma once

#include <Arduino.h>

#include "ConfigurationProvider.h"
#include "ShapeObjects.h"

typedef uint32_t Color;

class PixelHelper
{
    public:
        virtual ~PixelHelper();

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

        static inline Color colorFromRgb(uint8_t r, uint8_t g, uint8_t b) 
        {
            return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
        }

        static inline Color colorFromRgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w) 
        {
            return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
        }

        static const Color brightenPixel(const Color & p, const int brightness);
        static const Color brightenRed(const Color & p, const int brightness);
        static const Color brightenGreen(const Color & p, const int brightness);
        static const Color brightenBlue(const Color & p, const int brightness);
        static const Color brightenWhite(const Color & p, const int brightness);
        
        static const Color getRandomFullColor();
        static const Color getRandomFullColorExcept(const Color notThisOne);

        static const int FullColorNumber = 6;
        static const Color FullColorList[]; 
    private:
        PixelHelper();
        

};
