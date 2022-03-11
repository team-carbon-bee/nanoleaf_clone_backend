#include "PixelHelper.h"
#include "Constants.h"

#include "NeoPixelBusLedDriver.h"

namespace ledDriver {

NeoPixelBusLedDriver::NeoPixelBusLedDriver()
    : _strip(GlobalShapeHelper.ledCount(), Constants::LedPin)
{
}

NeoPixelBusLedDriver::~NeoPixelBusLedDriver()
{
}

void NeoPixelBusLedDriver::setup()
{
    _buffer = (Color*)calloc(GlobalShapeHelper.ledCount(), sizeof(Color));
    _brightness = 1.0;
    _strip.Begin();
    _strip.Show();
}

inline RgbColor NeoPixelBusLedDriver::colorToNeoPixelBusColor(const Color & c)
{
    //we apply brightness
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
    fill(0);
}

void NeoPixelBusLedDriver::fill(const Color c)
{
    for(int i = 0; i < GlobalShapeHelper.ledCount(); ++i)
        ((Color *)_buffer)[i] = c;
}

void NeoPixelBusLedDriver::setPixel(int pixelNumber, Color color)
{
    _buffer[pixelNumber] = color;
}

void NeoPixelBusLedDriver::setPixels(const Color * arraySource, const int offsetSource, const int len, const int offsetDest)
{
    for (int i = 0; i < len; ++i)
    {
        setPixel(offsetDest + i, (*(arraySource + offsetSource + i)));
    }
}

uint32_t NeoPixelBusLedDriver::getPixel(int pixelNumber)
{
    return _buffer[pixelNumber];
}

int NeoPixelBusLedDriver::numPixels() const
{
    return GlobalShapeHelper.ledCount();
}

void NeoPixelBusLedDriver::show()
{
    //copy buffer applying the brightness to color
    for (int i = 0; i < GlobalShapeHelper.ledCount(); ++i)
    {
        _strip.SetPixelColor(i, colorToNeoPixelBusColor(PixelHelper::brightenPixel(_buffer[i], _brightness)));
    }
    //sync the strip
    _strip.Show();
}

void NeoPixelBusLedDriver::setBrightness(const double value)
{
    _brightness = value;
}

double NeoPixelBusLedDriver::getBrightness()
{
    return _brightness;
}

} //referenceSystem