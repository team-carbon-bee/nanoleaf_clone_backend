#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"
#include "ConfigurationProvider.h"
#include "Color.h"

namespace referenceSystem {

class Border
{
    public:
        Border(ConfigurationProvider configuration);
        virtual ~Border();
    private:
        ConfigurationProvider & _configuration;
        Color * _pixels;
};

} //referenceSystem