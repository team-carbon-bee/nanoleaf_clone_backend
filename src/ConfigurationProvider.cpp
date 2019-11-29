#include <FS.h>
#include "SPIFFS.h"
#include "ConfigurationProvider.h"
#include "PixelHelper.h"

const String ConfigurationProvider::ConfigurationFilename = "/configuration.json";

ConfigurationProvider::ConfigurationProvider()
    : _assembly(NULL)
{
}

ConfigurationProvider::~ConfigurationProvider()
{
}

void ConfigurationProvider::setup()
{
    if (!SPIFFS.begin(true)) 
    {
        Serial.println("Unable to begin SPIFFS");
    }
    else
    {
        Serial.println("SPIFFS initialized.");
    }
}

void ConfigurationProvider::loadFromFlash()
{
    //TODO : restore when wifi ok and validated
    createDefaultConfiguration();
    if(SPIFFS.exists(ConfigurationFilename))
    {
        File file = SPIFFS.open(ConfigurationFilename, "r");
        if (!file)
        {
            Serial.println("Exception during opening system configuration, resetting to factory settings");
            createDefaultConfiguration();
            saveToFlash();
        }
        else
        {
            Serial.println("Configuration file opened.");
        }
        String configurationFileAsString = "";
        while (file.available())
        {
            configurationFileAsString +=(char)file.read();
        }

        parseJson(configurationFileAsString);
    }
    else
    {
        Serial.println("Configuration file doesn't exists, resetting to factory settings");
        createDefaultConfiguration();
        saveToFlash();
    }
}

void ConfigurationProvider::createDefaultConfiguration()
{
    Shape *rightSecond = new Shape();
    rightSecond->kind = triangle;
    rightSecond->content = NULL;
    rightSecond->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    rightSecond->connections[0] = NULL;
    rightSecond->connections[1] = NULL;
    
    Shape *second = new Shape();
    second->kind = triangle;
    second->content = NULL;
    second->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    second->connections[0] = NULL;
    second->connections[1] = NULL;

    Shape *rightThird = new Shape();
    rightThird->kind = triangle;
    rightThird->content = NULL;
    rightThird->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    rightThird->connections[0] = NULL;
    rightThird->connections[1] = NULL;

    Shape *third = new Shape();
    third->kind = triangle;
    third->content = NULL;
    third->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    third->connections[0] = NULL;
    third->connections[1] = NULL;

    _assembly = new Shape();
    _assembly->kind = triangle;
    _assembly->content = NULL;
    _assembly->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    _assembly->connections[0] = second;
    _assembly->connections[1] = third;
    _assembly->parent = NULL;

    //we affect parent to second and third
    second->parent = _assembly;
    third->parent = _assembly;

    _parameters.ledPerTriangle = 21;
    _parameters.hostname = "nanoleaf_clone";
    _parameters.maxBrightness = 255;
    _parameters.speed = 50;
    _parameters.mainColorRandom = true;
    _parameters.mainColor = 0;
    _parameters.backgroundColorRandom = true;
    _parameters.backgroundColor = 0;

    _globalBrigthness = 30;
}

void ConfigurationProvider::saveToFlash()
{
    // Delete existing file, otherwise the configuration is appended to the file
    if (SPIFFS.exists(ConfigurationFilename))
        SPIFFS.remove(ConfigurationFilename);

    // Open file for writing
    File file = SPIFFS.open(ConfigurationFilename, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }
    
    //we browse the tree to generate the json
    DynamicJsonDocument doc(DynamicJsonDocumentMaxSize);
    JsonObject assemblyNode = doc.createNestedObject("assembly");
    createJsonFromShape(assemblyNode, _assembly);

    JsonObject parameters = doc.createNestedObject("parameters");
    parameters["ledPerTriangle"] = _parameters.ledPerTriangle;
    parameters["hostname"] = _parameters.hostname;
    parameters["maxBrightness"] = _parameters.maxBrightness;
    parameters["speed"] = _parameters.speed;
    parameters["mainColorRandom"] = _parameters.mainColorRandom;
    parameters["mainColor"] = _parameters.mainColor;
    parameters["backgroundColorRandom"] = _parameters.backgroundColorRandom;
    parameters["backgroundColor"] = _parameters.backgroundColor;
    
    serializeJson(doc, Serial);
    serializeJson(doc, file);
    file.close();
    Serial.println("Configuration file saved");
}

void ConfigurationProvider::load(const String & data)
{
    parseJson(data);
}

void ConfigurationProvider::parseJson(const String & data)
{
    //deserializeJson
    DynamicJsonDocument doc(DynamicJsonDocumentMaxSize);
    Serial.print("parseJson : ");
    Serial.println(data);
    DeserializationError error = deserializeJson(doc, data);
    if (error) 
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    //iterate and create structure
    JsonObject obj = doc["assembly"]["shape"];
    _assembly = createShapeFromJSon(obj);
    
    JsonObject parameters = doc["parameters"];
    _parameters.ledPerTriangle = parameters["ledPerTriangle"] | 21;
    _parameters.hostname = parameters["hostname"] | "nanoleaf_clone";
    _parameters.maxBrightness = parameters["maxBrightness"] | 255;
    _parameters.speed = parameters["speed"] | 50;
    _parameters.mainColorRandom = parameters["mainColorRandom"] | true;
    _parameters.mainColor = parameters["mainColor"] | 0;
    _parameters.backgroundColorRandom = parameters["backgroundColorRandom"] | true;
    _parameters.backgroundColor = parameters["backgroundColor"] | 0;
}

Shape *ConfigurationProvider::createShapeFromJSon(const JsonObject & jsonObject, Shape * parent)
{
    if (jsonObject.isNull())
        return NULL;

    String shapeType = jsonObject["type"];
    if (shapeType == "triangle")
    {
        Shape * current = new Shape();
        current->kind = triangle;
        current->connections = (Shape**)malloc(sizeof(Shape *) * 2);
        JsonArray array = jsonObject["connections"];
        for(int i = 0; i < array.size(); ++i) 
        {
            current->connections[i] = createShapeFromJSon(array[i]["shape"], current);
        }
        current->content = NULL;
        current->parent = parent;
        return current;
    }
    else
    {
        Serial.print("unknown shape type: ");
        Serial.println(shapeType);
        return NULL;
    }
}

void ConfigurationProvider::createJsonFromShape(JsonObject & targetedObject, Shape * shape)
{
    if (shape == NULL)
        return;
    JsonObject jsonShape = targetedObject.createNestedObject("shape");
    jsonShape["type"] = shapeKindToString(shape->kind);
    JsonArray array = jsonShape.createNestedArray("connections");

    switch (shape->kind)
    {
        case triangle:
        {
            for (int i = 0; i < 2; ++i)
            {
                if (shape->connections[0] == NULL)
                {
                    //add "null" element to json
                    array.add(JsonObject());
                }
                else
                {
                    JsonObject branch = array.createNestedObject();
                    createJsonFromShape(branch, shape->connections[0]);
                }
            }
            break;
        }

        case unknown:
            //nothing to do
        break;
    }
}

const ConfigurationProvider::Parameters & ConfigurationProvider::parameters() const
{
    return _parameters;
}

ConfigurationProvider::Parameters & ConfigurationProvider::parameters()
{
    return _parameters;
}

Shape * ConfigurationProvider::assembly()
{
    return _assembly;
}

uint8_t ConfigurationProvider::globalBrigthness() const
{
    return _globalBrigthness;
}

void ConfigurationProvider::globalBrigthness(const uint8_t value)
{
    _globalBrigthness = value;
}

#if !defined(NO_GLOBAL_INSTANCES)
ConfigurationProvider Configuration;
#endif