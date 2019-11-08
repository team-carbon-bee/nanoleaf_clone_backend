#include "ShapeHelper.h"

ShapeHelper::ShapeHelper()
{
}

ShapeHelper::~ShapeHelper()
{
}

void ShapeHelper::setup()
{
}

//return the number of child connections of a shape based on its kind
int ShapeHelper::numberOfConnections(const Shape * node)
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
    return shapeCount(Configuration.assembly());
}

int ShapeHelper::shapeCount(const Shape * node)
{
    if (node == NULL)
        return 0;
    int res = 1;
    for (int i = 0; i < numberOfConnections(node); ++i)
        res += shapeCount(node->connections[i]);
    return res;
}

Shape * ShapeHelper::duplicateShape(Shape * node, Shape * parent)
{
    if (node == NULL)
        return NULL;
    Shape * res = new Shape();
    res->kind = node->kind;
    int nbCnx = numberOfConnections(node);
    res->connections = (Shape**)malloc(sizeof(Shape *) * nbCnx);
    for (int i = 0; i < nbCnx; ++i)
        res->connections[i] = duplicateShape(node->connections[i], res);  
    res->content = NULL;
    return res;
}

int ShapeHelper::ledCount()
{
    return ledCount(Configuration.assembly());
}

int ShapeHelper::ledCount(const Shape * node)
{
    if (node == NULL)
        return 0;
    int count = ledCountOfThisShape(node);    
    
    for (int i = 0; i < numberOfConnections(node); ++i)
        count += ledCount(node->connections[i]);  
    
    return count;
}

int ShapeHelper::ledCountOfThisShape(const Shape * node)
{
    switch (node->kind)
    {
        case triangle :
            return Configuration.parameters().ledPerTriangle;
        default:
        case unknown :
            return 0;
    }
}

#if !defined(NO_GLOBAL_INSTANCES)
ShapeHelper GlobalShapeHelper;
#endif