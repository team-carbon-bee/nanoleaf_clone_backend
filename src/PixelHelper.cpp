#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

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

void PixelHelper::setPixel(uint8_t * memory, int n, Color color)
{
    uint8_t *p = &memory[n * pixelSize()]; 
    //if there is no white, the color will be overriden by red   
    p[_whiteOffset] = (uint8_t)(color >> 24);
    p[_redOffset] = (uint8_t)(color >> 16);
    p[_greenOffset] = (uint8_t)(color >>  8);
    p[_blueOffset] = (uint8_t)color;
}

Color PixelHelper::getPixel(uint8_t * memory, int n)
{
    uint8_t *p = &memory[n * pixelSize()];  
    if (pixelSize() == 3)
        return (p[_redOffset] << 16) | (p[_greenOffset] << 8) | (p[_blueOffset]);
    return (p[_redOffset] << 24) | (p[_redOffset] << 16) | (p[_greenOffset] << 8) | (p[_blueOffset]);
}

uint8_t PixelHelper::getRed(const Color & p)
{
    return (uint8_t)(p >> 16);
}

uint8_t PixelHelper::getGreen(const Color & p)
{
    return (uint8_t)(p >>  8);
}

uint8_t PixelHelper::getBlue(const Color & p)
{
    return (uint8_t)p;
}

uint8_t PixelHelper::getWhite(const Color & p)
{
    return (uint8_t)(p >> 24);
}

void PixelHelper::setRed(uint8_t value, Color & p)
{
    p = (p & 0xFF00FFFF) | (value << 16);
}

void PixelHelper::setGreen(uint8_t value, Color & p)
{
    p = (p & 0xFFFF00FF) | (value << 8);
}

void PixelHelper::setBlue(uint8_t value, Color & p)
{
    p = (p & 0xFFFFFF00) | value;
}

void PixelHelper::setWhite(uint8_t value, Color & p)
{
    p = (p & 0x00FFFFFF) | (value << 24);
}

const Color PixelHelper::brightenPixel(const Color & p, const int brightness)
{
    Color res = p;
    setWhite(max(min(255, getWhite(p) + brightness), 0), res);
    setRed(max(min(255, getRed(p) + brightness), 0), res);
    setGreen(max(min(255, getGreen(p) + brightness), 0), res);
    setBlue(max(min(255, getBlue(p) + brightness), 0), res);

    return res;
}