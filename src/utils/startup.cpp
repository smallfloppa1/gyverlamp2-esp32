//
// Created by 12qwa on 12/4/2025.
//

#include "data.h"
#include "FastLED.h"
#include "config/config.h"

void start_leds()
{
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}
