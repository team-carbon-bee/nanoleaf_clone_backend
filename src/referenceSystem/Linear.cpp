#include "PixelHelper.h"

#include "Linear.h"

namespace referenceSystem {


Linear::Linear(ConfigurationProvider configuration, ShapeHelper & shapeHelper, PixelHelper & pixelHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper), _ledDriver(ledDriver)
{
}

Linear::~Linear()
{
}

void Linear::setup()
{
    //TODO : if (_configuration.parameters().ledModel == "rgb")
    _ledCount = _shapeHelper.ledCount();
    _pixels = (uint8_t *)malloc(sizeof(uint8_t) * _pixelHelper.pixelSize() * _ledCount);
    memset(_pixels, 0, _ledCount * _pixelHelper.pixelSize() * sizeof(uint8_t));
}

void Linear::driveLeds()
{
    //we simply iterate local pixels to ledDriver pixels
    memcpy(_ledDriver->getPixels(), _pixels, _ledCount * _pixelHelper.pixelSize() * sizeof(uint8_t));
    
    Serial.print("leds(");
    Serial.print(_ledCount);
    Serial.print(") : ");
    for (int i = 0; i < _ledCount; ++i)
    {
        Serial.printf("%06x ", getPixel(i));
    }
    Serial.println("");
    _ledDriver->show();
}

uint8_t * Linear::pixels()
{
    return _pixels;
}

int Linear::ledCount()
{
    return _ledCount;
}

void Linear::clear()
{
    memset(_pixels, 0, _ledCount * sizeof(uint8_t) * _pixelHelper.pixelSize());
}

int Linear::pixelSize() const
{
    return _pixelHelper.pixelSize();
}

void Linear::setPixel(int pixelNumber, uint32_t color)
{
    _pixelHelper.setPixel(_pixels, pixelNumber, color);
}

uint32_t Linear::getPixel(int pixelNumber)
{
    return _pixelHelper.getPixel(_pixels, pixelNumber);
}

} //referenceSystem