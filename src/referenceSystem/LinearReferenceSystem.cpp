#include "PixelHelper.h"

#include "LinearReferenceSystem.h"

namespace referenceSystem {


LinearReferenceSystem::LinearReferenceSystem(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, 
    PixelHelper * pixelHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper), _ledDriver(ledDriver)
{
}

LinearReferenceSystem::~LinearReferenceSystem()
{
}

void LinearReferenceSystem::setup()
{
    Serial.printf("LinearReferenceSystem kind = %d\n", _configuration->assembly()->kind);
    _ledCount = _shapeHelper->ledCount();
    _pixels = (uint8_t *)malloc(sizeof(uint8_t) * _pixelHelper->pixelSize() * _ledCount);
    memset(_pixels, 0, _ledCount * _pixelHelper->pixelSize() * sizeof(uint8_t));
}

void LinearReferenceSystem::driveLeds()
{
    //we simply iterate local pixels to ledDriver pixels
    memcpy(_ledDriver->getPixels(), _pixels, _ledCount * _pixelHelper->pixelSize() * sizeof(uint8_t));

    /*Serial.print("leds(");
    Serial.print(_ledCount);
    Serial.print(") : ");
    for (int i = 0; i < _ledCount; ++i)
    {
        Serial.printf("%06x ", getPixel(i));
    }
    Serial.println("");*/
    _ledDriver->show();
}

uint8_t * LinearReferenceSystem::pixels()
{
    return _pixels;
}

int LinearReferenceSystem::ledCount()
{
    return _ledCount;
}

void LinearReferenceSystem::clear()
{
    memset(_pixels, 0, _ledCount * sizeof(uint8_t) * _pixelHelper->pixelSize());
}

void LinearReferenceSystem::fill(const Color c)
{
    for (int i = 0; i < _ledCount; ++i)
    {
        setPixel(i, c);
    }
}

int LinearReferenceSystem::pixelSize() const
{
    return _pixelHelper->pixelSize();
}

void LinearReferenceSystem::setPixel(int pixelNumber, uint32_t color)
{
    _pixelHelper->setPixel(_pixels, pixelNumber, color);
}

uint32_t LinearReferenceSystem::getPixel(int pixelNumber)
{
    return _pixelHelper->getPixel(_pixels, pixelNumber);
}

} //referenceSystem