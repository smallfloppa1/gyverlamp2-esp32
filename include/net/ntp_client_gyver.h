#ifndef NTP_CLIENT_GYVER_H
#define NTP_CLIENT_GYVER_H

#include <Arduino.h>
#include <time.h>    // For time_t, tm struct
#include <WiFiUdp.h> // If using WiFi UDP for NTP

#ifdef __cplusplus
extern "C" {
#endif

    void ntp_setup();

    void ntp_update();

    time_t getNtpTime();

#ifdef __cplusplus
}
#endif

#endif // NTP_CLIENT_GYVER_H