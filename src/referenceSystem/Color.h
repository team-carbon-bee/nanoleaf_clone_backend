#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"
#include "ConfigurationProvider.h"

namespace referenceSystem {

class Color
{
    public:
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
            :   _r(r), _g(g), _b(b), _w(w)
        {}

        Color(uint8_t r, uint8_t g, uint8_t b)
            :   _r(r), _g(g), _b(b), _w(0)
        {}

        virtual ~Color();

        uint8_t r() const
        {
            return _r;
        }

        void r(uint8_t value)
        {
            _r = value;
        }

        uint8_t g() const
        {
            return _g;
        }

        void g(uint8_t value)
        {
            _g = value;
        }

        uint8_t b() const
        {
            return _b;
        }

        void b(uint8_t value)
        {
            _b = value;
        }
        
        uint8_t w() const
        {
            return _w;
        }

        void w(uint8_t value)
        {
            _w = value;
        }
    private:
        uint8_t _r, _g, _b, _w;
};

} //referenceSystem