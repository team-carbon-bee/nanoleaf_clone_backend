#include <FS.h>
#include "SPIFFS.h"
#include "Assembly.h"
#include "ConfigurationProvider.h"

const String ConfigurationProvider::ConfigurationFilename = "/configuration.json";

ConfigurationProvider::ConfigurationProvider()
{
}

ConfigurationProvider::~ConfigurationProvider()
{
}

void ConfigurationProvider::setup()
{
    if (!SPIFFS.begin()) 
    {
        Serial.println("Unable to begin SPIFFS");
    }
}

void ConfigurationProvider::loadFromFlash()
{
    File file = SPIFFS.open(ConfigurationFilename, "r");
    if (!file)
    {
        Serial.println("Exception during opening system configuration, resetting to factory settings");
        createDefaultConfiguration();
        saveToFlash();
        return;
    }

    String configurationFileAsString = "";
    while (file.available())
    {
        configurationFileAsString +=(char)file.read();
    }

    parseJson(configurationFileAsString);
}

void ConfigurationProvider::createDefaultConfiguration()
{
    //TODO : create empty structure with only one shape
}

void ConfigurationProvider::saveToFlash()
{
    //TODO : save structure to flash
}

void ConfigurationProvider::load(const String & data)
{
    parseJson(data);
}

void ConfigurationProvider::parseJson(const String & data)
{
    //deserializeJson
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, data);
    if (error) 
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    //iterate and create structure
    JsonObject obj = doc["assembly"].as<JsonObject>();
    _assembly.root(createChilds(obj));
}

Shape *ConfigurationProvider::createChilds(JsonObject & jsonObject)
{
    String shapeType = jsonObject["type"];
    if (shapeType == "triangle")
    {
        Shape * current = new Shape();
        current->kind = triangle;
        current->connections = (Shape**)malloc(sizeof(Shape *) * 2);
        JsonObject leftConnection = jsonObject["leftConnection"].as<JsonObject>();
        current->connections[0] = createChilds(leftConnection);
        JsonObject rightConnection = jsonObject["rightConnection"].as<JsonObject>();
        current->connections[1] = createChilds(rightConnection);
        return current;
    }
    else
    {
        Serial.print("unknown shape type: ");
        Serial.println(shapeType);
        return NULL;
    }
}