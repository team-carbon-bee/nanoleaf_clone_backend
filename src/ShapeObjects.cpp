#include "ShapeObjects.h"

String shapeKindToString(const ShapeKind kind)
{
    switch (kind)
    {
        case triangle :
            return "triangle";
        default:
        case unknown :
            return "unknown";
    }
}

ShapeKind stringToShapeKind(const String & kind)
{
    if (kind == "triangle")
        return triangle;
    return unknown;
}