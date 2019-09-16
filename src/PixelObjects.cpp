#include "PixelObjects.h"

String pixelKindToString(const PixelKind kind)
{
    switch (kind)
    {
        case BGR :
            return "bgr";
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
    if (kind == "rgbw")
        return RGBW;
    return unknownPixelKind;
}