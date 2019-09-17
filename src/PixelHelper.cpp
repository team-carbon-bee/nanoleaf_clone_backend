#include "PixelHelper.h"

//Local pixels are always in uint32_t format following structure : WWRRGGBB

PixelHelper::PixelHelper(ConfigurationProvider & configuration)
    : _configuration(configuration)
{
    switch (PixelKind())
    {
        //24 bit pixel size (3 bytes)
        default:
        case BGR:
            _pixelSize = 3;
            break;
        //32 bit pixel size (4 bytes)
        case RGBW:
            _pixelSize = 4;
            break;
    } 
}

PixelHelper::~PixelHelper()
{
}

PixelKind PixelHelper::pixelKind() const
{
    return _configuration.parameters().ledModel;
}

int PixelHelper::pixelSize() const
{
    return _pixelSize;
}

void PixelHelper::setPixel(uint8_t * memory, int pixelNumber, uint32_t color)
{
    switch (PixelKind())
    {
        case BGR:
            memory[pixelNumber * _pixelSize] = (color & 0x00FF0000) >> 16;
            memory[pixelNumber * _pixelSize + 1] = (color & 0x0000FF00) >> 8;
            memory[pixelNumber * _pixelSize + 2] = (color & 0x000000FF);
            break;
        case GRB:
            Serial.println("grb");
            memory[pixelNumber * _pixelSize] = (color & 0x000000FF);
            memory[pixelNumber * _pixelSize + 1] = (color & 0x00FF0000) >> 16;
            memory[pixelNumber * _pixelSize + 2] = (color & 0x0000FF00) >> 8;
            break;
        case RGBW:
            memory[pixelNumber * _pixelSize] = (color & 0xFF000000) >> 24;
            memory[pixelNumber * _pixelSize + 1] = (color & 0x0000FF);
            memory[pixelNumber * _pixelSize + 2] = (color & 0x0000FF00) >> 8;
            memory[pixelNumber * _pixelSize + 3] = (color & 0x00FF0000) >> 16;
            break;
    }
}

uint32_t PixelHelper::getPixel(uint8_t * memory, int pixelNumber)
{
    uint32_t result = 0;
    switch (PixelKind())
    {
        case BGR:
            result |= memory[pixelNumber * _pixelSize] << 16;
            result |= memory[pixelNumber * _pixelSize + 1] << 8;
            result |= memory[pixelNumber * _pixelSize + 2];
            break;
        case GRB:
            result |= memory[pixelNumber * _pixelSize];
            result |= memory[pixelNumber * _pixelSize + 1] << 16;
            result |= memory[pixelNumber * _pixelSize + 2] << 8;
            break;
        case RGBW:
            result |= memory[pixelNumber * _pixelSize] << 16; 
            result |= memory[pixelNumber * _pixelSize + 1] << 8;
            result |= memory[pixelNumber * _pixelSize + 2];
            result |= memory[pixelNumber * _pixelSize + 3] << 24;
            break;
    }
    return result;
}