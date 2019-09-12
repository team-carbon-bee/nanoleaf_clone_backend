#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Assembly.h"

class ConfigurationProvider
{
    public:
        ConfigurationProvider();
        virtual ~ConfigurationProvider();

        void setup();

        void loadFromFlash();
        void saveToFlash();
        void load(const String & data);

        void createDefaultConfiguration();

    private:
        void parseJson(const String & data);
        Shape *createChilds(JsonObject & jsonObject);
        static const String ConfigurationFilename;
        Assembly _assembly;

};