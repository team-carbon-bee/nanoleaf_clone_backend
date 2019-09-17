#include "PixelObjects.h"

String pixelKindToString(const PixelKind kind)
{
    switch (kind)
    {
        case BGR :
            return "bgr";
        case GRB :
            return "grb";
        case RGBW :
            return "rgbw";
        default:
        case unknownPixelKind :
            return "unknownPixelKind";
    }
}

PixelKind stringToPixelKind(const String & kind)
{
    if (kind == "bgr")
        return BGR;
    if (kind == "grb")
        return GRB;
    if (kind == "rgbw")
        return RGBW;
    return unknownPixelKind;
}