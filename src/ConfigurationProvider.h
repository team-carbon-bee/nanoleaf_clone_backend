#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShapeObjects.h"
#include "PixelObjects.h"

class ConfigurationProvider
{
    public:
        typedef struct
        {
            String hostname;
            int ledPerTriangle;
            PixelKind ledModel;
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
        Shape *createShapeFromJSon(JsonObject & jsonObject, Shape * parent = NULL);
        JsonObject createJsonFromShape(Shape * shape);


        static const String ConfigurationFilename;
        Shape * _assembly;
        Parameters _parameters;

        static const int DynamicJsonDocumentMaxSize = 3 * 1024;

};