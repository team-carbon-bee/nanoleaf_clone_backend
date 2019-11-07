#include "PixelHelper.h"

#include "LinearReferenceSystem.h"

namespace referenceSystem {


LinearReferenceSystem::LinearReferenceSystem(ConfigurationProvider * configuration, 
                                             ledDriver::ILedDriver * ledDriver)
    : _configuration(configuration), _ledDriver(ledDriver)
{
}

LinearReferenceSystem::~LinearReferenceSystem()
{
}

void LinearReferenceSystem::setup()
{
}

void LinearReferenceSystem::driveLeds()
{
    //Serial.printf("led 29 - 31 %06x(%06x) %06x(%06x) %06x(%06x)\n", _ledDriver->getPixelColor(29), _ledDriver->getPixelColor(30), _ledDriver->getPixelColor(31));

    //we re-enable brightness
    //TODO : implement brightness
    //_ledDriver->setBrightness(_configuration->globalBrigthness());
    
    _ledDriver->show();

    /*Serial.print("leds(");
    Serial.print(_ledDriver->numPixels());
    Serial.print(") : ");
    for (int i = 0; i < _ledDriver->numPixels(); ++i)
    {
        Serial.printf("%06x ", _ledDriver->getPixel(i));
    }
    Serial.println("");*/
    // for (int n = 0; n < 21 * 3; ++n)
    //     //_ledDriver->setPixelColor(n, Adafruit_NeoPixel::Color(i, 0, i));
    //     strip.SetPixelColor(n, RgbColor(0, i, 0));
    // if (inc)
    //     i += 20;
    // else
    //     i -= 20;

    // if (i > 255)
    // {
    //     i = 255;
    //     inc = false;
    // }
    // else if (i < 0)
    // {
    //     i = 0;
    //     inc = true;
    // }
    
    // strip.Show();

}

int LinearReferenceSystem::ledCount()
{
    return _ledDriver->numPixels();
}

void LinearReferenceSystem::clear()
{
    _ledDriver->clear();
}

void LinearReferenceSystem::fill(const Color c)
{
    _ledDriver->fill(c);
}

void LinearReferenceSystem::setPixel(int pixelNumber, Color color)
{
    _ledDriver->setPixel(pixelNumber, color);
}

uint32_t LinearReferenceSystem::getPixel(int pixelNumber)
{
    return _ledDriver->getPixel(pixelNumber);
}

} //referenceSystem