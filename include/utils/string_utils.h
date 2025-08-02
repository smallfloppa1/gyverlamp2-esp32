#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    void bytesToHexString(const byte* bytes, size_t len, char* buffer);

    int parseIntFromString(const char* str);

    bool stringContains(const char* haystack, const char* needle);

#ifdef __cplusplus
}
#endif

#endif // STRING_UTILS_H