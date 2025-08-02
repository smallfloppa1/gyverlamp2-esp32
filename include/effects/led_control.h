#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <FastLED.h>
#include "data.h"    // For cfg, NUM_LEDS, MAX_LEDS (if NUM_LEDS isn't directly defined)
#include "config/config.h" // For NUM_LEDS

#ifdef __cplusplus
extern "C" {
#endif

    void setPix(int x, int y, CRGB color);

    CRGB getPix(int x, int y);

    // Function to set an LED by its linear index (for strips or linear mapping)
    void setLED(int index, CRGB color);

    void fillStrip(int from, int to, CRGB color);

    void fillRow(int y, CRGB color);

    // Similar to setPix but handles overlapping/blending colors
    void setPixOverlap(int x, int y, CRGB color);

#ifdef __cplusplus
}
#endif

#endif // LED_CONTROL_H