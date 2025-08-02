#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Function to scale a byte value by a byte scale factor (0-255)
    // Example: scaleFF(128, 64) -> 32
    byte scaleFF(byte val, byte scale);

    // Function similar to Arduino's map, but potentially optimized or with specific behavior
    // Example: mapFF(50, 0, 100, 0, 255) -> 127
    int mapFF(int val, int in_min, int in_max);

    // Add other generic utility functions here
    // Example: Constrain a value to a range
    template <typename T>
    T constrainFF(T val, T minVal, T maxVal) {
        return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
    }

#ifdef __cplusplus
}
#endif

#endif // UTILS_H