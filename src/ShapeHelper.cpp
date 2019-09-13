#include "ShapeHelper.h"

ShapeHelper::ShapeHelper(ConfigurationProvider & configuration)
    : _configuration(configuration)
{
}

ShapeHelper::~ShapeHelper()
{
}

//return the number of child connections of a shape based on its kind
int ShapeHelper::numberOfCconnections(const Shape * node)
{
    if (node == NULL)
        return 0;
    switch (node->kind)
    {
        case triangle :
            return 2;
        default:
        case unknown :
            return 0;
    }
}

int ShapeHelper::shapeCount()
{
    return shapeCount(_configuration.assembly());
}

int ShapeHelper::shapeCount(const Shape * node)
{
    if (node == NULL)
        return 0;
    int res = 1;
    for (int i = 0; i < numberOfCconnections(node); ++i)
        res += shapeCount(node->connections[i]);
    return res;
}

int ShapeHelper::ledCount()
{
    return ledCount(_configuration.assembly());
}

int ShapeHelper::ledCount(const Shape * node)
{
    //TODO : manage other shape than triangle for numberOfLed
    return  _configuration.parameters().ledPerTriangle 
            * shapeCount(node);
}