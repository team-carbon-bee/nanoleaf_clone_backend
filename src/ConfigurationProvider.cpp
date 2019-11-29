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
}

void ConfigurationProvider::loadFromFlash()
{
    //TODO : restore when wifi ok and validated
    createDefaultConfiguration();
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
    doc["assembly"] = createJsonFromShape(_assembly);

    JsonObject parameters;
    parameters["ledPerTriangle"] = _parameters.ledPerTriangle;
    parameters["hostname"] = _parameters.hostname;
    parameters["maxBrightness"] = _parameters.maxBrightness;

    doc["parameters"] = parameters;
    serializeJson(doc, file);
    file.close();
    Serial.println("Configuration fiele saved");
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
    JsonObject obj = doc["assembly"].as<JsonObject>();
    _assembly = createShapeFromJSon(obj);
    
    JsonObject parameters = doc["parameters"].as<JsonObject>();
    _parameters.ledPerTriangle = parameters["ledPerTriangle"].as<int>();
    _parameters.hostname = parameters["hostname"].as<String>();
    _parameters.maxBrightness = parameters["maxBrightness"].as<uint8_t>();
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
        JsonArray array = jsonObject["connections"].as<JsonArray>();
        for(int i = 0; i < array.size(); ++i) 
        {
            current->connections[i] = createShapeFromJSon(array[i].as<JsonObject>(), current);
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

JsonObject ConfigurationProvider::createJsonFromShape(Shape * shape)
{
    JsonObject obj;
    if (shape == NULL)
        return obj;
    JsonObject jsonShape;
    jsonShape["type"] = shapeKindToString(shape->kind);
    JsonArray array;

    switch (shape->kind)
    {
        case triangle:
            array.add(createJsonFromShape(shape->connections[0]));
            array.add(createJsonFromShape(shape->connections[1]));
        break;

        case unknown:
            //nothing to do
        break;
    }
    jsonShape["connections"] = array;
    obj["shape"] = jsonShape;
    return obj;
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