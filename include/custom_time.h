#ifndef CUSTOM_TIME_H
#define CUSTOM_TIME_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    uint32_t getCustomTimeElapsed();
    void updateCustomTime();

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_TIME_H