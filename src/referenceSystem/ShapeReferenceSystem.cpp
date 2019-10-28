#include "PixelHelper.h"

#include "ShapeReferenceSystem.h"
#include "ShapeDetails.h"

namespace referenceSystem {


ShapeReferenceSystem::ShapeReferenceSystem(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, 
                                           ledDriver::ILedDriver * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _ledDriver(ledDriver), _assembly(NULL)
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
    createLinkedListFromShapes(_assembly);
}

void ShapeReferenceSystem::createShapeDetailObjects(Shape * shape)
{
    if (shape == NULL)
        return;
        //we create the object for the active shape and call for all of its childs
    ShapeDetails * details = new ShapeDetails(shape, _shapeHelper);
    details->setup();
    shape->content = details;
    for (int i = 0; i < _shapeHelper->numberOfConnections(shape); ++i)
        createShapeDetailObjects(shape->connections[i]);  
}

void ShapeReferenceSystem::createLinkedListFromShapes(Shape * shape)
{
    if (shape == NULL)
        return;
    
    _shapeList.Append(shape);
    
    for (int i = 0; i < _shapeHelper->numberOfConnections(shape); ++i)
        createLinkedListFromShapes(shape->connections[i]);  
}

void ShapeReferenceSystem::driveLeds()
{
    /*Serial.println("shape0");
    for (int i = 0; i < _shapeHelper->ledCountOfThisShape(_assembly); ++i)
    {
        Serial.printf("%06x ", getShape(_assembly)->getPixel(i));
    }
    Serial.println(".");
    Serial.println("shape1");
    for (int i = 0; i < _shapeHelper->ledCountOfThisShape(_assembly->connections[0]); ++i)
    {
        Serial.printf("%06x ", getShape(_assembly->connections[0])->getPixel(i));
    }
    Serial.println(".");*/

    //we disable brigthness during filling leds values
    //_ledDriver->setBrightness(255);
    prepareDriveLeds(_assembly);
    /*
    Serial.print("leds(");
    Serial.print(_ledDriver->numPixels());
    Serial.print(") : ");
    for (int i = 0; i < _ledDriver->numPixels(); ++i)
    {
        Serial.printf("%06x ", _ledDriver->getPixelColor(i));
    }
    Serial.println("");*/
    //we re-enable brightness
    //_ledDriver->setBrightness(_configuration->globalBrigthness());
    _ledDriver->show();
}

int ShapeReferenceSystem::prepareDriveLeds(Shape * node, const int offset)
{
    if (node == NULL)
        return offset;
    //we iterate each object taking a part of leds (numberOfLeds / (number of connections + 1) between each connection

    int numberOfLedToTakeBetweenConnections = _shapeHelper->ledCountOfThisShape(node) / (_shapeHelper->numberOfConnections(node) + 1);
    int newOffset = offset;
    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
    {
        _ledDriver->setPixels(getShape(node)->pixels(), i * numberOfLedToTakeBetweenConnections, 
                                   numberOfLedToTakeBetweenConnections, newOffset);
        
        newOffset += numberOfLedToTakeBetweenConnections;
        newOffset = prepareDriveLeds(node->connections[i], newOffset); 
    }
    //we copy the end of the shape after the last connection
    _ledDriver->setPixels(getShape(node)->pixels(), _shapeHelper->numberOfConnections(node) * numberOfLedToTakeBetweenConnections, 
                                   numberOfLedToTakeBetweenConnections, newOffset);
    newOffset += numberOfLedToTakeBetweenConnections;
    return newOffset;
}

Shape * ShapeReferenceSystem::assembly()
{
    return _assembly;
}

LinkedList<Shape *> & ShapeReferenceSystem::shapeList()
{
    return _shapeList;
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
    getShape(node)->clear();

    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
        clear(node->connections[i]);  
}

ShapeDetails * ShapeReferenceSystem::getShape(Shape * node)
{
    return (ShapeDetails *)node->content;
}

void ShapeReferenceSystem::clearAnimationObject()
{
    if (_shapeList.moveToStart())
    {
        do 
        {
            getShape(_shapeList.getCurrent())->clearAnimationObject();
        } while (_shapeList.next()); 
    }
}

void ShapeReferenceSystem::fill(const Color c)
{
    fill(_assembly, c);
}

void ShapeReferenceSystem::fill(Shape * node, const Color c)
{
    if (node == NULL)
        return;

    //we fill current
    getShape(node)->fill(c);
    for (int i = 0; i < _shapeHelper->numberOfConnections(node); ++i)
        fill(node->connections[i], c);  
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

Shape * ShapeReferenceSystem::getRandomShapeExcept(Shape * notThisOne)
{
    return getRandomShapeExcept(_assembly, notThisOne);
}

Shape * ShapeReferenceSystem::getRandomShapeExcept(Shape * node, Shape * notThisOne)
{
    if (_shapeHelper->shapeCount(node) == 0)
        return NULL;
    if (_shapeHelper->shapeCount(node) == 1)
        return node;
    Shape * current = notThisOne;
    while (current == notThisOne)
    {
        current = getRandomShape(node);
    }
    return current; 
}

} //referenceSystem