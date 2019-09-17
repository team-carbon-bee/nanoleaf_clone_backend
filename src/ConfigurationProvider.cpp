#include <FS.h>
#include "SPIFFS.h"
#include "ConfigurationProvider.h"

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
    /*File file = SPIFFS.open(ConfigurationFilename, "r");
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

    parseJson(configurationFileAsString);*/
}

void ConfigurationProvider::createDefaultConfiguration()
{
    _assembly = new Shape();
    _assembly->kind = triangle;
    _assembly->content = NULL;
    _assembly->connections = (Shape**)malloc(sizeof(Shape *) * 2);
    _assembly->connections[0] = NULL;
    _assembly->connections[1] = NULL;

    _parameters.ledPerTriangle = 21;
    _parameters.ledModel = GRB;
    _parameters.hostname = "nanoleaf_clone";
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
    parameters["ledModel"] = pixelKindToString(_parameters.ledModel);
    parameters["hostname"] = _parameters.hostname;

    doc["parameters"] = parameters;
    serializeJson(doc, file);
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
    _parameters.ledModel = stringToPixelKind(parameters["ledModel"].as<String>());
    _parameters.hostname = parameters["hostname"].as<String>();
}

Shape *ConfigurationProvider::createShapeFromJSon(JsonObject & jsonObject)
{
    if (jsonObject.isNull())
        return NULL;

    String shapeType = jsonObject["type"];
    if (shapeType == "triangle")
    {
        Shape * current = new Shape();
        current->kind = triangle;
        current->connections = (Shape**)malloc(sizeof(Shape *) * 2);
        JsonObject leftConnection = jsonObject["leftConnection"].as<JsonObject>();
        current->connections[0] = createShapeFromJSon(leftConnection);
        JsonObject rightConnection = jsonObject["rightConnection"].as<JsonObject>();
        current->connections[1] = createShapeFromJSon(rightConnection);
        current->content = NULL;
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
    
    switch (shape->kind)
    {
        case triangle:
            jsonShape["leftConnection"] = createJsonFromShape(shape->connections[0]);
            jsonShape["rightConnection"] = createJsonFromShape(shape->connections[1]);
        break;

        case unknown:
            //nothing to do
        break;
    }

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