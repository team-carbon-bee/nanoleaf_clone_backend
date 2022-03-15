#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

#include "ShapeObjects.h"
#include "PixelHelper.h"
#include "Color.h"

class ConfigurationProvider
{
    public:
        typedef struct
        {
            //Hostname of the board
            String hostname;
            //Number of leds by triangle
            int ledPerTriangle;
            //maximum Brightness of leds (0.0-1.0)
            double maxBrightness;
            //whole animation speed (ms)
            int speed;
            //has main color has to be set randomly
            bool mainColorRandom;
            //Main color if not random
            Color mainColor;
            //Has Background color has to be set randomly
            bool backgroundColorRandom;
            //Background color if not random
            Color backgroundColor;
            //Duration of animation in ms
            int animationDuration;
            //Kind animation choice (static : one animation choosen, 
            //                                random : selected animation random
            //                                sequential : selected animation by order)
            String animationMethod;
            //List of the animation to use
            std::vector<uint8_t> animationList;
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