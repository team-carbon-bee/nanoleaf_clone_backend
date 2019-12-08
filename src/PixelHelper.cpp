#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#include "PixelHelper.h"

//Local pixels are always in uint32_t format following structure : WWRRGGBB
const Color PixelHelper::FullColorList[6] = {0x0000FF, 0x00FFFF, 0x00FF00, 0XFFFF00, 0xFF0000, 0xFF00FF};

PixelHelper::PixelHelper()
{
}

PixelHelper::~PixelHelper()
{
}

const Color PixelHelper::brightenPixel(const Color & p, const double brightness)
{
    Color res = p;
    setWhite(max(min(255, (int)((float)getWhite(p) * brightness)), 0), res);
    setRed(max(min(255, (int)((float)getRed(p) * brightness)), 0), res);
    setGreen(max(min(255, (int)((float)getGreen(p) * brightness)), 0), res);
    setBlue(max(min(255, (int)((float)getBlue(p) * brightness)), 0), res);

    return res;
}

const Color PixelHelper::brightenRed(const Color & p, const double brightness)
{
    Color res = p;
    setRed(max(min(255, (int)((float)getRed(p) * brightness)), 0), res);

    return res;
}

const Color PixelHelper::brightenGreen(const Color & p, const double brightness)
{
    Color res = p;
    setGreen(max(min(255, (int)((float)getGreen(p) * brightness)), 0), res);

    return res;
}

const Color PixelHelper::brightenBlue(const Color & p, const double brightness)
{
    Color res = p;
    setBlue(max(min(255, (int)((float)getBlue(p) * brightness)), 0), res);

    return res;
}

const Color PixelHelper::brightenWhite(const Color & p, const double brightness)
{
    Color res = p;
    setWhite(max(min(255, (int)((float)getWhite(p) * brightness)), 0), res);

    return res;
}

const Color PixelHelper::getRandomFullColor()
{
    int idx = random(PixelHelper::FullColorNumber);
    return PixelHelper::FullColorList[idx];
}

const Color PixelHelper::getRandomFullColorExcept(const Color notThisOne)
{
    Color current = notThisOne;
    while (current == notThisOne)
    {
        current = getRandomFullColor();
    }
    return current;
}