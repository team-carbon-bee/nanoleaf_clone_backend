#include "PixelHelper.h"
#include "Constants.h"

#include "NeoPixelBusLedDriver.h"

namespace ledDriver {

NeoPixelBusLedDriver::NeoPixelBusLedDriver(ConfigurationProvider * configuration, ShapeHelper * shapeHelper)
    : _configuration(configuration), _shapeHelper(shapeHelper), _strip(_shapeHelper->ledCount(), Constants::LedPin)
{
}

NeoPixelBusLedDriver::~NeoPixelBusLedDriver()
{
}

void NeoPixelBusLedDriver::setup()
{
    _strip.Begin();
    _strip.Show();
}

inline RgbColor NeoPixelBusLedDriver::colorToNeoPixelBusColor(const Color & c)
{
    //our Color can be seen as Html color in NeoPixelBus format
    return HtmlColor(c);
}

inline Color NeoPixelBusLedDriver::neoPixelBusColorToColor(const RgbColor & c)
{
    //our Color can be seen as Html color in NeoPixelBus format
    return (Color)HtmlColor(c).Color;
}

void NeoPixelBusLedDriver::clear()
{
    _strip.ClearTo(RgbColor(0));
}

void NeoPixelBusLedDriver::fill(const Color c)
{
    _strip.ClearTo(colorToNeoPixelBusColor(c));
}

void NeoPixelBusLedDriver::setPixel(int pixelNumber, Color color)
{
    _strip.SetPixelColor(pixelNumber, colorToNeoPixelBusColor(color));
}

void NeoPixelBusLedDriver::setPixels(const Color * arraySource, const int offsetSource, const int len, const int offsetDest)
{
    for (int i = 0; i < len; ++i)
    {
        _strip.SetPixelColor(offsetDest + i, colorToNeoPixelBusColor(*(arraySource + offsetSource + i)));
    }
}

uint32_t NeoPixelBusLedDriver::getPixel(int pixelNumber)
{
    return neoPixelBusColorToColor(_strip.GetPixelColor(pixelNumber));
}

int NeoPixelBusLedDriver::numPixels() const
{
    return _strip.PixelCount();
}

void NeoPixelBusLedDriver::show()
{
    _strip.Show();
}

} //referenceSystem