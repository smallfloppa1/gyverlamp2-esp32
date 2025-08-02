#ifndef DATA_H
#define DATA_H

#include <FastLED.h>

#include "config/config.h"
#include "utils/timer_millis.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Global Structures ---

// Configuration settings, often stored in EEPROM or NVS
struct Config {
  byte length;        // Length of the LED strip/matrix along the primary axis
  byte width;         // Width of the LED strip/matrix (1 for strip, >1 for matrix)
  byte deviceType;    // 0: strip, 1: horizontal matrix, 2: vertical matrix (example)
  bool state;         // Overall power state (on/off)
  bool role;          // True for master, False for slave (in UDP setup)
  byte adcMode;       // Mode for Analog-to-Digital Converter: GL_ADC_BRI (brightness sensor)
  byte minLight;      // Min raw ADC value from light sensor
  byte maxLight;      // Max raw ADC value from light sensor
  byte minBright;     // Minimum brightness level (0-255)
  byte maxBright;     // Maximum brightness level (0-255)
  byte bright;        // Current overall brightness
  byte matrix;        // Matrix type for FastLED (e.g., NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ZIGZAG)
  // Add other global config parameters here (e.g., WiFi state, effect defaults)
};

// Current preset settings for the active effect
struct CurrentPreset {
  byte effect;        // Current active effect ID
  byte palette;       // Current active palette ID
  byte speed;         // Speed of the effect (0-255)
  byte color;         // Base color for effects (0-255 hue)
  bool fromCenter;    // True if effect starts from center, False if from one end
  byte bright;        // Brightness specific to this preset
  bool fadeBright;    // True if brightness should fade based on preset value
  byte advMode;       // Advanced mode (e.g., GL_ADV_CLOCK for clock overlay)
};

// Data structure for storing custom palettes (if used beyond FastLED built-ins)
struct PaletteData {
    byte size;        // Number of colors in the custom palette
    byte strip[16 * 3]; // Raw RGB data for up to 16 colors (16 * 3 bytes)
};

// Configuration for a "dawn" simulation effect
struct DawnConfig {
    byte bright;      // Target brightness for dawn
    uint16_t post;    // Post-dawn duration in minutes
};

// Structure to hold current time information
struct NowTime {
    uint32_t weekMs;  // Milliseconds since the start of the week (or other suitable time base)
};

// --- Global Instances (Declared as extern, defined once in main.cpp) ---
extern Config cfg;            // Global configuration object
extern CurrentPreset CUR_PRES; // Global current preset object
extern CRGB leds[NUM_LEDS];   // Array of CRGB structs for LED data (defined in config.h)
extern CRGBPalette16 paletteArr[MAX_PALETTES]; // Array of FastLED palettes
extern PaletteData pal;       // Global custom palette data
extern DawnConfig dawn;       // Global dawn configuration
extern NowTime now;           // Global current time object

// --- Global Timers (Declared as extern, defined once in main.cpp) ---
extern TimerMillis effectTimer;    // Timer for effect updates
extern TimerMillis dawnTimer;   // Timer for dawn effect duration
extern TimerMillis postDawn;  // Timer for post-dawn bright period
extern TimerMillis turnoffTimer; // Timer for turn-off animation
extern TimerMillis blinkTimer;  // Timer for blink animation

// --- Global Variables for Master/Slave UDP Communication ---
extern uint32_t updateTimer;      // Timestamp of last UDP send
extern uint32_t gotAdcTimer;   // Timestamp of last received UDP data (from master)
extern int udpLength;        // Length received via UDP
extern byte udpScale;        // Scale received via UDP
extern byte udpBright;       // Brightness received via UDP

// --- Global Variables for Debug/Feedback Indicators ---
extern int btnClicks;        // Count of button clicks for debug/feedback
extern int brTicks;          // Brightness ticks for debug/feedback

// --- Enums for Configuration Modes ---
enum AdcMode {
  GL_ADC_BRI = 0, // ADC used for brightness sensor only
};

// Removed: SoundReactMode enum (no sound analysis)

enum AdvancedMode {
  GL_ADV_CLOCK = 0, // Display clock overlay
  // Add other advanced modes here
};

// --- Common Macros for Loops ---
// These are often defined in a common utilities file or directly where used,
// but can be here for global availability.
#ifndef FOR_i
#define FOR_i(from, to) for (int i = from; i < to; i++)
#endif
#ifndef FOR_j
#define FOR_j(from, to) for (int j = from; j < to; j++)
#endif
#ifndef FOR_k
#define FOR_k(from, to) for (int k = from; k < to; k++)
#endif

// --- Forward Declarations for Utility Functions & Classes ---
// These functions might have their own headers later, but declared here for now.
byte scaleFF(byte val, byte scale); // Scales a byte value by a scale factor
int mapFF(int val, int in_min, int in_max); // Maps a value from one range to another

// Effects related functions (their full headers might be included by effects.cpp)
void drawClock(int x, byte sat, CHSV color); // Draws a clock on the display
void fireRoutine(byte speed); // Old fire effect (2D)
void fire2020(byte scale, int length); // Modern fire effect

// Network related functions
void sendUDP(byte type, int len, byte scl, byte bri); // Sends UDP packet

// Power management
void setPower(bool state); // Sets the overall power state of the LEDs

// PhotSensor class declaration
class PhotSensor {
public:
    int getFil(); // Gets filtered photoresistor reading
};
extern PhotSensor phot; // Global instance of the photo sensor

#ifdef __cplusplus
}
#endif

#endif // DATA_H