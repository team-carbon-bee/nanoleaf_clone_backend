#pragma once

#include <Arduino.h>

typedef enum 
{
    BGR,
    GRB,
    RGBW,
    unknownPixelKind
} PixelKind;

String pixelKindToString(const PixelKind kind);
PixelKind stringToPixelKind(const String & kind);