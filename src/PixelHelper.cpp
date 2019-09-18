#include "PixelHelper.h"

//Local pixels are always in uint32_t format following structure : WWRRGGBB

PixelHelper::PixelHelper(ConfigurationProvider & configuration)
    : _configuration(configuration)
{
    //we extract offsets from pixelKind for each color
    _whiteOffset = (pixelKind() >> 6) & 0b11; 
    _redOffset = (pixelKind() >> 4) & 0b11;
    _greenOffset = (pixelKind() >> 2) & 0b11;
    _blueOffset =  pixelKind()       & 0b11;
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
    //There is 3 byte per pixel if white component has been defined with the same offset than the red one
    return (_whiteOffset == _redOffset?3:4);
}

void PixelHelper::setPixel(uint8_t * memory, int n, uint32_t color)
{
    uint8_t *p = &memory[n * pixelSize()]; 
    //if there is no white, the color will be overriden by red   
    p[_whiteOffset] = (uint8_t)(color >> 24);
    p[_redOffset] = (uint8_t)(color >> 16);
    p[_greenOffset] = (uint8_t)(color >>  8);
    p[_blueOffset] = (uint8_t)color;
}

uint32_t PixelHelper::getPixel(uint8_t * memory, int n)
{
    uint8_t *p = &memory[n * pixelSize()];  
    if (pixelSize() == 3)
        return (p[_redOffset] << 16) | (p[_greenOffset] << 8) | (p[_blueOffset]);
    return (p[_redOffset] << 24) | (p[_redOffset] << 16) | (p[_greenOffset] << 8) | (p[_blueOffset]);
}