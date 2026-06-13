#pragma once
#include <Arduino.h>

struct SensorData {
  float dht_temp;
  float dht_humidity;
  float api_temp;
  int   weather_code;
  uint32_t timestamp_ms;
  bool  valid;
};
