#include <FS.h>
#include "SPIFFS.h"
#include "ConfigurationProvider.h"
#include "PixelHelper.h"
#include "tools/Logger.h"

const String ConfigurationProvider::ConfigurationFilename = "/configuration.json";

const String ConfigurationProvider::animationSelectionMethodToString(const ConfigurationProvider::EAnimationSelectionMethod &method)
{
    switch (method)
    {
    case kStatic:
        return "static";
    default:
    case kRandom:
        return "random";
    case kSequential:
        return "sequential";
    }
}

ConfigurationProvider::EAnimationSelectionMethod ConfigurationProvider::parseAnimationSelectionMethod(const String &method)
{
    if (method == "static")
        return kStatic;
    if (method == "random")
        return kRandom;
    if (method == "sequential")
        return kSequential;
    return kRandom;
}

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
        Log.println("Unable to begin SPIFFS");
    }
    else
    {
        Log.println("SPIFFS initialized.");
    }
}

void ConfigurationProvider::loadFromFlash()
{
    // TODO : restore when wifi ok and validated
    // createDefaultConfiguration();
    // saveToFlash();
    if (SPIFFS.exists(ConfigurationFilename))
    {
        File file = SPIFFS.open(ConfigurationFilename, "r");
        if (!file)
        {
            Log.println("Exception during opening system configuration, resetting to factory settings");
            createDefaultConfiguration();
            saveToFlash();
        }
        else
        {
            Log.println("Configuration file opened.");
        }
        String configurationFileAsString = "";
        while (file.available())
        {
            configurationFileAsString += (char)file.read();
        }
        Log.println("Configuration file read.");
        parseJson(configurationFileAsString);
    }
    else
    {
        Log.println("Configuration file doesn't exists, resetting to factory settings");
        createDefaultConfiguration();
        saveToFlash();
    }
}

void ConfigurationProvider::createDefaultConfiguration()
{
    Shape *rightSecond = new Shape();
    rightSecond->kind = triangle;
    rightSecond->content = NULL;
    rightSecond->connections = (Shape **)malloc(sizeof(Shape *) * 2);
    rightSecond->connections[0] = NULL;
    rightSecond->connections[1] = NULL;

    Shape *second = new Shape();
    second->kind = triangle;
    second->content = NULL;
    second->connections = (Shape **)malloc(sizeof(Shape *) * 2);
    second->connections[0] = NULL;
    second->connections[1] = NULL;

    Shape *rightThird = new Shape();
    rightThird->kind = triangle;
    rightThird->content = NULL;
    rightThird->connections = (Shape **)malloc(sizeof(Shape *) * 2);
    rightThird->connections[0] = NULL;
    rightThird->connections[1] = NULL;

    Shape *third = new Shape();
    third->kind = triangle;
    third->content = NULL;
    third->connections = (Shape **)malloc(sizeof(Shape *) * 2);
    third->connections[0] = NULL;
    third->connections[1] = NULL;

    _assembly = new Shape();
    _assembly->kind = triangle;
    _assembly->content = NULL;
    _assembly->connections = (Shape **)malloc(sizeof(Shape *) * 2);
    _assembly->connections[0] = second;
    _assembly->connections[1] = third;
    _assembly->parent = NULL;

    // we affect parent to second and third
    second->parent = _assembly;
    third->parent = _assembly;

    _parameters.ledPerTriangle = 21;
    _parameters.hostname = "nanoleaf_clone";
    _parameters.maxBrightness = 0.3;
    _parameters.speed = 50;
    _parameters.mainColorRandom = true;
    _parameters.mainColor = 0;
    _parameters.backgroundColorRandom = true;
    _parameters.backgroundColor = 0;
    _parameters.animationDuration = 20 * 1000;
    _parameters.animationMethod = kRandom;
    // TODO : make better
    _parameters.animationList = {10, 11, 12, 13, 14, 15, 16, 17, 18};
}

bool ConfigurationProvider::saveToFlash()
{
    // Delete existing file, otherwise the configuration is appended to the file
    if (SPIFFS.exists(ConfigurationFilename))
        SPIFFS.remove(ConfigurationFilename);

    // Open file for writing
    File file = SPIFFS.open(ConfigurationFilename, FILE_WRITE);
    if (!file)
    {
        Log.println(F("Failed to create file"));
        return false;
    }

    // we browse the tree to generate the json
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


    JsonObject animation = doc.createNestedObject("animation");
    animation["animationDuration"] = _parameters.animationDuration;
    animation["animationMethod"] = animationSelectionMethodToString(_parameters.animationMethod);

    JsonArray animationList = animation.createNestedArray("animationList");
    for (const auto &id : _parameters.animationList)
        animationList.add(id);

    serializeJson(doc, file);
    file.close();
    serializeJson(doc, Serial);
    Serial.println();
    Log.println("Configuration file saved");
    return true;
}

bool ConfigurationProvider::load(const String &data)
{
    return parseJson(data);
}

bool ConfigurationProvider::parseJson(const String &data)
{
    // deserializeJson
    DynamicJsonDocument doc(DynamicJsonDocumentMaxSize);
    DeserializationError error = deserializeJson(doc, data, DeserializationOption::NestingLimit(40));
    if (error)
    {
        Log.print("deserializeJson() failed: ");
        Log.println(error.c_str());
        return false;
    }

    // iterate and create structure
    JsonObject assembly = doc["assembly"];
    if (!assembly.isNull())
        _assembly = createShapeFromJSon(assembly["shape"]);

    JsonObject parameters = doc["parameters"];
    if (!parameters.isNull())
    {
        _parameters.ledPerTriangle = parameters["ledPerTriangle"] | 21;
        _parameters.hostname = parameters["hostname"] | "nanoleaf_clone";
        _parameters.maxBrightness = parameters["maxBrightness"] | 1.0;
        _parameters.speed = parameters["speed"] | 50;
        _parameters.mainColorRandom = parameters["mainColorRandom"] | true;
        _parameters.mainColor = parameters["mainColor"] | 0;
        _parameters.backgroundColorRandom = parameters["backgroundColorRandom"] | true;
        _parameters.backgroundColor = parameters["backgroundColor"] | 0;
    }

    JsonObject animation = doc["animation"];
    if (!animation.isNull())
    {
        _parameters.animationDuration = animation["animationDuration"] | 20 * 1000;
        _parameters.animationMethod = parseAnimationSelectionMethod(animation["animationMethod"] | "random");

        _parameters.animationList.clear();
        JsonArray jsonAnimation = animation["animationList"].as<JsonArray>();
        for (JsonVariant id : jsonAnimation)
        {
            _parameters.animationList.push_back(id.as<int>());
        }
    }

    return true;
}

Shape *ConfigurationProvider::createShapeFromJSon(const JsonObject &jsonObject, Shape *parent)
{
    if (jsonObject.isNull())
        return NULL;

    String shapeType = jsonObject["type"];
    if (shapeType == "triangle")
    {
        Shape *current = new Shape();
        current->kind = triangle;
        current->connections = (Shape **)malloc(sizeof(Shape *) * 2);
        JsonArray array = jsonObject["connections"];
        for (int i = 0; i < array.size(); ++i)
        {
            current->connections[i] = createShapeFromJSon(array[i]["shape"], current);
        }
        current->content = NULL;
        current->parent = parent;
        return current;
    }
    else
    {
        Log.print("unknown shape type: ");
        Log.println(shapeType);
        return NULL;
    }
}

void ConfigurationProvider::createJsonFromShape(JsonObject &targetedObject, Shape *shape)
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
                    // add "null" element to json
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
            // nothing to do
            break;
    }
}

const ConfigurationProvider::Parameters &ConfigurationProvider::parameters() const
{
    return _parameters;
}

ConfigurationProvider::Parameters &ConfigurationProvider::parameters()
{
    return _parameters;
}

Shape *ConfigurationProvider::assembly()
{
    return _assembly;
}

#if !defined(NO_GLOBAL_INSTANCES)
ConfigurationProvider Configuration;
#endif