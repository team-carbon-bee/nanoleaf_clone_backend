#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"
#include "PixelHelper.h"

class ConfigurationProvider
{
    public:
        typedef struct
        {
            String hostname;
            int ledPerTriangle;
            uint8_t maxBrightness;
        } Parameters;
        
        ConfigurationProvider();
        virtual ~ConfigurationProvider();

        void setup();

        void loadFromFlash();
        void saveToFlash();
        void load(const String & data);

        void createDefaultConfiguration();

        const Parameters & parameters() const;
        Parameters & parameters();

        Shape * assembly();

        uint8_t globalBrigthness() const;
        void globalBrigthness(const uint8_t value);
    private:
        void parseJson(const String & data);
        Shape *createShapeFromJSon(JsonObject & jsonObject, Shape * parent = NULL);
        JsonObject createJsonFromShape(Shape * shape);


        static const String ConfigurationFilename;
        Shape * _assembly;
        Parameters _parameters;
        uint8_t _globalBrigthness;

        static const int DynamicJsonDocumentMaxSize = 3 * 1024;

};