#pragma once
#include <Arduino.h>

enum BootState {
  BOOT_SERIAL_INIT,
  BOOT_DISPLAY,
  BOOT_LCD,
  BOOT_SPIFFS,
  BOOT_RTC,
  BOOT_I2S,
  BOOT_BOTOES,
  BOOT_WIFI_START,
  BOOT_WIFI_WAIT,
  BOOT_WIFI_RESULT,
  BOOT_NTP,
  BOOT_SERVER,
  BOOT_DHT,
  BOOT_COMPLETO
};

extern BootState bootState;
extern bool bootFinalizado;
extern bool oledOkBoot;
extern unsigned long bootStepTime;
extern unsigned long bootWifiT0;
