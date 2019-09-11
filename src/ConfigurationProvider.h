#pragma once

#include <Arduino.h>

class ConfigurationProvider
{
    public:
        ConfigurationProvider();
        virtual ~ConfigurationProvider();

        void setup();

        void loadFromFlash();
        void load();

    private:
        void parseJson();

};