#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "utils/timer_millis.h" // For debounce/click timers

#ifdef __cplusplus
extern "C" {
#endif

    // Enum for button states or events
    enum ButtonEvent {
        BTN_NONE,
        BTN_CLICK,
        BTN_LONG_PRESS,
        BTN_DOUBLE_CLICK
    };

    class Button {
    public:
        Button(byte pin, uint32_t debounceDelayMs = 50);
        void tick(); // Call this frequently in loop()
        ButtonEvent getEvent();
        bool isPressed();

    private:
        byte _pin;
        uint32_t _debounceDelay;
        uint32_t _lastDebounceTime;
        bool _lastButtonState;
        bool _currentButtonState;
        ButtonEvent _currentEvent;

        // Add timers for long press, double click etc.
        TimerMillis _pressTimer;
        TimerMillis _clickTimer;
        int _clickCount;
    };

#ifdef __cplusplus
}
#endif

#endif // BUTTON_H