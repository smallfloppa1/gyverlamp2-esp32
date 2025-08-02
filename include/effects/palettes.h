#ifndef PALETTES_H
#define PALETTES_H

#include <FastLED.h> // For CRGBPalette16
#include "data.h"    // For paletteArr, pal, CUR_PRES

#ifdef __cplusplus
extern "C" {
#endif

    void updatePalette();

    byte scalePalette(byte val);

    void loadCustomPalette(const PaletteData* data, CRGBPalette16& targetPalette);

#ifdef __cplusplus
}
#endif

#endif // PALETTES_H