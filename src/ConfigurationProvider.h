#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"

class ConfigurationProvider
{
    public:
        typedef struct
        {
            String hostname;
            int ledPerTriangle;
            String ledModel;

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

    private:
        void parseJson(const String & data);
        Shape *createShapeFromJSon(JsonObject & jsonObject);
        JsonObject createJsonFromShape(Shape * shape);


        static const String ConfigurationFilename;
        Shape * _assembly;
        Parameters _parameters;

        static const int DynamicJsonDocumentMaxSize = 3 * 1024;

};