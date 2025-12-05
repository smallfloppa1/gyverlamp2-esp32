#include <Arduino.h>
#include <FastLED.h>

#include "config/config.h"
#include "data.h"
#include "core/button.h"

Config config;
CurrentPreset CURR_PRESET;
CRGB leds[NUM_LEDS];
CRGBPalette16 paletteArr[MAX_PALETTES];
PaletteData palette;
DawnConfig dawn;
NowTime now;
Button button(BUTTON_PIN);

TimerMillis effectsTimer(50);
TimerMillis dawnTimer;
TimerMillis postDawn;
TimerMillis turnoffTimer;
TimerMillis blinkTimer;

uint32_t udpTimer = 0;                 // Timestamp of last UDP send by master
uint32_t gotAdcTimer = 0;              // Timestamp of last received UDP data by slave
int udpLength = 0;                   // Length parameter received via UDP
byte udpScale = 0;                   // Scale parameter received via UDP
byte udpBright = 0;                  // Brightness parameter received via UDP

int btnClicks = 0;                   // Counter for button clicks
int brTicks = 0;                     // Counter for brightness adjustments



void setup()
{
    delay(INIT_DELAY_MILLIS);
    Serial.begin(BAUD_RATE);



}

void loop()
{

}