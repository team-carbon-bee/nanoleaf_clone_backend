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

        static inline uint8_t getRed(const Color & p);
        static inline uint8_t getGreen(const Color & p);
        static inline uint8_t getBlue(const Color & p);
        static inline uint8_t getWhite(const Color & p);

        static inline void setRed(uint8_t value, Color & p);
        static inline void setGreen(uint8_t value, Color & p);
        static inline void setBlue(uint8_t value, Color & p);
        static inline void setWhite(uint8_t value, Color & p);

        static const Color brightenPixel(const Color & p, const int brightness);
        
    private:
        ConfigurationProvider & _configuration;
        int _redOffset;
        int _greenOffset;
        int _blueOffset;
        int _whiteOffset;
};
