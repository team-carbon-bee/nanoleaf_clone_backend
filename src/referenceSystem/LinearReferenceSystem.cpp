#include "PixelHelper.h"

#include "LinearReferenceSystem.h"

namespace referenceSystem {


LinearReferenceSystem::LinearReferenceSystem()
{
}

LinearReferenceSystem::~LinearReferenceSystem()
{
}

void LinearReferenceSystem::setup(ledDriver::ILedDriver * ledDriver)
{
    _ledDriver = ledDriver;
}

void LinearReferenceSystem::driveLeds()
{
    _ledDriver->show();
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

Color LinearReferenceSystem::getPixel(int pixelNumber)
{
    return _ledDriver->getPixel(pixelNumber);
}

#if !defined(NO_GLOBAL_INSTANCES)
LinearReferenceSystem LinearRef;
#endif

} //referenceSystem