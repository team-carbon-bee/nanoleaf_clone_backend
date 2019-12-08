#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"
#include "PixelHelper.h"
#include "Color.h"

class ConfigurationProvider
{
    public:
        typedef struct
        {
            String hostname;
            int ledPerTriangle;
            double maxBrightness;
            int speed;
            bool mainColorRandom;
            Color mainColor;
            bool backgroundColorRandom;
            Color backgroundColor;
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
    private:
        void parseJson(const String & data);
        Shape *createShapeFromJSon(const JsonObject & jsonObject, Shape * parent = NULL);
        void createJsonFromShape(JsonObject & targetedObject, Shape * shape);

        static const String ConfigurationFilename;
        Shape * _assembly;
        Parameters _parameters;

        static const int DynamicJsonDocumentMaxSize = 3 * 1024;

};

#if !defined(NO_GLOBAL_INSTANCES)
extern ConfigurationProvider Configuration;
#endif