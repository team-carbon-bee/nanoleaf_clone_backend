#pragma once

#include <Arduino.h>

typedef enum 
{
    //Extracted from AdaFruit_NeoPixel library
    // RGB NeoPixel permutations; white and red offsets are always same
    // Offset:         W        R        G        B
    RGB =  ((0<<6) | (0<<4) | (1<<2) | (2)), ///< Transmit as R,G,B
    RBG =  ((0<<6) | (0<<4) | (2<<2) | (1)), ///< Transmit as R,B,G
    GRB =  ((1<<6) | (1<<4) | (0<<2) | (2)), ///< Transmit as G,R,B
    GBR =  ((2<<6) | (2<<4) | (0<<2) | (1)), ///< Transmit as G,B,R
    BRG =  ((1<<6) | (1<<4) | (2<<2) | (0)), ///< Transmit as B,R,G
    BGR =  ((2<<6) | (2<<4) | (1<<2) | (0)), ///< Transmit as B,G,R

    // RGBW NeoPixel permutations; all 4 offsets are distinct
    // Offset:         W          R          G          B
    WRGB = ((0<<6) | (1<<4) | (2<<2) | (3)), ///< Transmit as W,R,G,B
    WRBG = ((0<<6) | (1<<4) | (3<<2) | (2)), ///< Transmit as W,R,B,G
    WGRB = ((0<<6) | (2<<4) | (1<<2) | (3)), ///< Transmit as W,G,R,B
    WGBR = ((0<<6) | (3<<4) | (1<<2) | (2)), ///< Transmit as W,G,B,R
    WBRG = ((0<<6) | (2<<4) | (3<<2) | (1)), ///< Transmit as W,B,R,G
    WBGR = ((0<<6) | (3<<4) | (2<<2) | (1)), ///< Transmit as W,B,G,R

    RWGB = ((1<<6) | (0<<4) | (2<<2) | (3)), ///< Transmit as R,W,G,B
    RWBG = ((1<<6) | (0<<4) | (3<<2) | (2)), ///< Transmit as R,W,B,G
    RGWB = ((2<<6) | (0<<4) | (1<<2) | (3)), ///< Transmit as R,G,W,B
    RGBW = ((3<<6) | (0<<4) | (1<<2) | (2)), ///< Transmit as R,G,B,W
    RBWG = ((2<<6) | (0<<4) | (3<<2) | (1)), ///< Transmit as R,B,W,G
    RBGW = ((3<<6) | (0<<4) | (2<<2) | (1)), ///< Transmit as R,B,G,W

    GWRB = ((1<<6) | (2<<4) | (0<<2) | (3)), ///< Transmit as G,W,R,B
    GWBR = ((1<<6) | (3<<4) | (0<<2) | (2)), ///< Transmit as G,W,B,R
    GRWB = ((2<<6) | (1<<4) | (0<<2) | (3)), ///< Transmit as G,R,W,B
    GRBW = ((3<<6) | (1<<4) | (0<<2) | (2)), ///< Transmit as G,R,B,W
    GBWR = ((2<<6) | (3<<4) | (0<<2) | (1)), ///< Transmit as G,B,W,R
    GBRW = ((3<<6) | (2<<4) | (0<<2) | (1)), ///< Transmit as G,B,R,W

    BWRG = ((1<<6) | (2<<4) | (3<<2) | (0)), ///< Transmit as B,W,R,G
    BWGR = ((1<<6) | (3<<4) | (2<<2) | (0)), ///< Transmit as B,W,G,R
    BRWG = ((2<<6) | (1<<4) | (3<<2) | (0)), ///< Transmit as B,R,W,G
    BRGW = ((3<<6) | (1<<4) | (2<<2) | (0)), ///< Transmit as B,R,G,W
    BGWR = ((2<<6) | (3<<4) | (1<<2) | (0)), ///< Transmit as B,G,W,R
    BGRW = ((3<<6) | (2<<4) | (1<<2) | (0)), ///< Transmit as B,G,R,W

    unknownPixelKind
} PixelKind;

String pixelKindToString(const PixelKind kind);
PixelKind stringToPixelKind(const String & kind);