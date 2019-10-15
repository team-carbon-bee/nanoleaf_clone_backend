#include "PixelHelper.h"

#include "ShapeReferenceSystem.h"
#include "ShapeDetails.h"

namespace referenceSystem {


ShapeReferenceSystem::ShapeReferenceSystem(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, PixelHelper * pixelHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper), _ledDriver(ledDriver), _assembly(NULL)
{
}

ShapeReferenceSystem::~ShapeReferenceSystem()
{
}

void ShapeReferenceSystem::setup()
{
    _shapeCount = _shapeHelper->shapeCount();
    _assembly = _shapeHelper->duplicateShape(_configuration->assembly());
    
    //we iterate the duplicate tree to create content objects that will embed values of each leds of the shape
    createShapeDetailObjects(_assembly);
}

void ShapeReferenceSystem::createShapeDetailObjects(Shape * shape)
{
    if (shape == NULL)
        return;
        //we create the object for the active shape and call for all of its childs
    ShapeDetails * details = new ShapeDetails(shape, _shapeHelper, _pixelHelper);
    details->setup();
    shape->content = details;
    for (int i = 0; i < _shapeHelper->numberOfConnections(shape); ++i)
        createShapeDetailObjects(shape->connections[i]);  
}

void ShapeReferenceSystem::driveLeds()
{
    /*Serial.println("shape0");
    for (int i = 0; i < _shapeHelper->ledCountOfThisShape(_assembly); ++i)
    {
        Serial.printf("%06x ", getDetails(_assembly)->getPixel(i));
    }
    Serial.println(".");
    Serial.println("shape1");
    for (int i = 0; i < _shapeHelper->ledCountOfThisShape(_assembly->connections[0]); ++i)
    {
        Serial.printf("%06x ", getDetails(_assembly->connections[0])->getPixel(i));
    }
    Serial.println(".");*/
    prepareDriveLeds(_assembly);
    
    Serial.print("leds(");
    Serial.print(_ledDriver->numPixels());
    Serial.print(") : ");
    for (int i = 0; i < _ledDriver->numPixels(); ++i)
    {
        Serial.printf("%06x ", _ledDriver->getPixelColor(i));
    }
    Serial.println("");
    _ledDriver->show();
}

int ShapeReferenceSystem::prepareDriveLeds(Shape * node, const int offset)
{
    if (node == NULL)
        return offset;
    //we iterate each object taking a part of leds (numberOfLeds / (number of connections + 1) between each connection
    //memcpy(_ledDriver->getPixels(), _pixels, _ledCount * _pixelHelper.pixelSize() * sizeof(uint8_t));
    int numberOfLedToTakeBetweenConnections = _shapeHelper->ledCountOfThisShape(node) / (_shapeHelper->numberOfConnections(node) + 1);
    int newOffset = offset;
    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
    {
        memcpy(&(_ledDriver->getPixels()[newOffset * _pixelHelper->pixelSize()]), 
               &(getDetails(node)->pixels()[i * numberOfLedToTakeBetweenConnections * _pixelHelper->pixelSize()]), 
               numberOfLedToTakeBetweenConnections * _pixelHelper->pixelSize() * sizeof(uint8_t));
        newOffset += numberOfLedToTakeBetweenConnections;

        newOffset = prepareDriveLeds(node->connections[i], newOffset); 
    }
    //we copy the end of the shape after the last connection
    memcpy(&(_ledDriver->getPixels()[newOffset * _pixelHelper->pixelSize()]), 
               &(getDetails(node)->pixels()[_shapeHelper->numberOfConnections(node) * numberOfLedToTakeBetweenConnections * _pixelHelper->pixelSize()]), 
               numberOfLedToTakeBetweenConnections * _pixelHelper->pixelSize() * sizeof(uint8_t));

    newOffset += numberOfLedToTakeBetweenConnections;
    return newOffset;
}

Shape * ShapeReferenceSystem::assembly()
{
    return _assembly;
}

int ShapeReferenceSystem::shapeCount()
{
    return _shapeCount;
}

void ShapeReferenceSystem::clear()
{
    clear(_assembly);
}

void ShapeReferenceSystem::clear(Shape * node)
{
    if (node == NULL)
        return;

    //we clear current
    getDetails(node)->clear();

    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
        clear(node->connections[i]);  
}

ShapeDetails * ShapeReferenceSystem::getDetails(Shape * node)
{
    return (ShapeDetails *)node->content;
}

void ShapeReferenceSystem::fill(const Color c)
{
    fill(c, _assembly);
}

void ShapeReferenceSystem::fill(const Color c, Shape * node)
{
    if (node == NULL)
        return;

    //we fill current
    getDetails(node)->fill(c);

    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
        fill(c, node->connections[i]);  
}

int ShapeReferenceSystem::pixelSize() const
{
    return _pixelHelper->pixelSize();
}

Shape * ShapeReferenceSystem::getRandomShape()
{
    return getRandomShape(_assembly);
}

//Return random child shape of node 
Shape * ShapeReferenceSystem::getRandomShape(Shape * node)
{
    int rnd = random(_shapeHelper->shapeCount(node));
    if (rnd == 0)
    {
        //One chance to take the current
        return node;
    }

    int nbConn = _shapeHelper->numberOfConnections(node);
    int cumuledNbNodes = 1; //the current node 
    for (int i = 0; i < nbConn; ++i)
    {
        if (node->connections[i] != NULL)
        {
            //we compute the probability to take this connection
            cumuledNbNodes += _shapeHelper->shapeCount(node->connections[i]);
            if (rnd < cumuledNbNodes)
            {
                //we have to take one from this connection
                return getRandomShape(node->connections[i]);
            }
        }
    }
    //we are not supposed to go here
    Serial.println("Invalid random shape");
    return node;
}

} //referenceSystem