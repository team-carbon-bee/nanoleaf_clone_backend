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
    _ledCount = _shapeHelper->ledCount();
    _pixels = (uint8_t *)malloc(sizeof(uint8_t) * _pixelHelper->pixelSize() * _ledCount);
    memset(_pixels, 0, _ledCount * _pixelHelper->pixelSize() * sizeof(uint8_t));
}

void LinearReferenceSystem::driveLeds()
{
    //we disable brigthness during filling leds values
    _ledDriver->setBrightness(255);

    //we simply iterate local pixels to ledDriver pixels
    memcpy(_ledDriver->getPixels(), _pixels, _ledCount * _pixelHelper->pixelSize() * sizeof(uint8_t));
    
    //Serial.printf("led 29 - 31 %06x(%06x) %06x(%06x) %06x(%06x)\n", _ledDriver->getPixelColor(29), _ledDriver->getPixelColor(30), _ledDriver->getPixelColor(31));
    
    //Serial.printf("led 50 %06x(%06x)\n", _ledDriver->getPixelColor(50), getPixel(50));
    //Serial.printf("led 62 %06x(%06x)\n", _ledDriver->getPixelColor(62), getPixel(62));

    /*Serial.print("leds(");
    Serial.print(_ledCount);
    Serial.print(") : ");
    for (int i = 0; i < _ledCount; ++i)
    {
        Serial.printf("%06x ", getPixel(i));
    }
    Serial.println("");*/
    //we re-enable brightness
    _ledDriver->setBrightness(_configuration->globalBrigthness());
    //Serial.printf("led 29 %06x(%06x)\n", _ledDriver->getPixelColor(29), getPixel(29));
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