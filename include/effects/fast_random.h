#ifndef FAST_RANDOM_H
#define FAST_RANDOM_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    uint8_t fastrandom8();
    uint16_t fastrandom16();
    uint32_t fastrandom32();

    // Functions to seed the fast random generator
    void seedFastRandom(uint32_t seed);

#ifdef __cplusplus
}
#endif

#endif // FAST_RANDOM_H