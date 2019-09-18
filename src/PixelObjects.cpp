#include "PixelObjects.h"

String pixelKindToString(const PixelKind kind)
{
    switch (kind)
    {
        case RGB : 
            return "RGB"; 
        case RBG : 
            return "RBG"; 
        case GRB : 
            return "GRB"; 
        case GBR : 
            return "GBR"; 
        case BRG : 
            return "BRG"; 
        case BGR : 
            return "BGR"; 
        case WRGB : 
            return "WRGB";
        case WRBG : 
            return "WRBG";
        case WGRB : 
            return "WGRB";
        case WGBR : 
            return "WGBR";
        case WBRG : 
            return "WBRG";
        case WBGR : 
            return "WBGR";
        case RWGB : 
            return "RWGB";
        case RWBG : 
            return "RWBG";
        case RGWB : 
            return "RGWB";
        case RGBW : 
            return "RGBW";
        case RBWG : 
            return "RBWG";
        case RBGW : 
            return "RBGW";
        case GWRB : 
            return "GWRB";
        case GWBR : 
            return "GWBR";
        case GRWB : 
            return "GRWB";
        case GRBW : 
            return "GRBW";
        case GBWR : 
            return "GBWR";
        case GBRW : 
            return "GBRW";
        case BWRG : 
            return "BWRG";
        case BWGR : 
            return "BWGR";
        case BRWG : 
            return "BRWG";
        case BRGW : 
            return "BRGW";
        case BGWR : 
            return "BGWR";
        case BGRW : 
            return "BGRW";
        default:
        case unknownPixelKind :
            return "unknownPixelKind";
    }
}

PixelKind stringToPixelKind(const String & kind)
{
    if (kind == "RGB")
        return RGB ;
    if (kind == "RBG")
        return RBG ;
    if (kind == "GRB")
        return GRB ;
    if (kind == "GBR")
        return GBR ;
    if (kind == "BRG")
        return BRG ;
    if (kind == "BGR")
        return BGR ;
    if (kind == "WRGB")
        return WRGB;
    if (kind == "WRBG")
        return WRBG;
    if (kind == "WGRB")
        return WGRB;
    if (kind == "WGBR")
        return WGBR;
    if (kind == "WBRG")
        return WBRG;
    if (kind == "WBGR")
        return WBGR;
    if (kind == "RWGB")
        return RWGB;
    if (kind == "RWBG")
        return RWBG;
    if (kind == "RGWB")
        return RGWB;
    if (kind == "RGBW")
        return RGBW;
    if (kind == "RBWG")
        return RBWG;
    if (kind == "RBGW")
        return RBGW;
    if (kind == "GWRB")
        return GWRB;
    if (kind == "GWBR")
        return GWBR;
    if (kind == "GRWB")
        return GRWB;
    if (kind == "GRBW")
        return GRBW;
    if (kind == "GBWR")
        return GBWR;
    if (kind == "GBRW")
        return GBRW;
    if (kind == "BWRG")
        return BWRG;
    if (kind == "BWGR")
        return BWGR;
    if (kind == "BRWG")
        return BRWG;
    if (kind == "BRGW")
        return BRGW;
    if (kind == "BGWR")
        return BGWR;
    if (kind == "BGRW")
        return BGRW;

    return unknownPixelKind;
}