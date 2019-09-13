#include "Linear.h"

namespace referenceSystem {


Linear::Linear(ConfigurationProvider configuration, ShapeHelper & shapeHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _ledDriver(ledDriver)
{
}

Linear::~Linear()
{
}

void Linear::setup()
{
    _ledCount = _shapeHelper.ledCount();
    _pixels = (uint32_t *)malloc(sizeof(uint32_t) * _ledCount);
    memset(_pixels, 0, _ledCount * sizeof(uint32_t));
}

void Linear::driveLeds()
{
    //we simply iterate local pixels to ledDriver pixels
    memcpy(_ledDriver->getPixels(), _pixels, _ledCount);
    String dbg = "";
    for (int i = 0; i < _ledCount; ++i)
        dbg += String(_pixels[i], 16) + " ";
    Serial.print("leds(");
    Serial.print(_ledCount);
    Serial.print(") : ");
    Serial.println(dbg);
    _ledDriver->show();
}

uint32_t * Linear::pixels()
{
    return _pixels;
}

int Linear::ledCount()
{
    return _ledCount;
}

} //referenceSystem