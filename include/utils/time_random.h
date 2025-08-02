#ifndef TIME_RANDOM_H
#define TIME_RANDOM_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    void seedRandomWithTime();

    long getRandomTimeSeeded(long howBig);
    long getRandomTimeSeeded(long howSmall, long howBig);

#ifdef __cplusplus
}
#endif

#endif // TIME_RANDOM_H