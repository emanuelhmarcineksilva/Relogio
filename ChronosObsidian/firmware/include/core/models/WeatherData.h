#pragma once
#include <Arduino.h>

struct WeatherData {
  int weather_code;
  float temp_current;
  float temp_min;
  float temp_max;
  bool valid;
  unsigned long last_fetch;
};

extern WeatherData climaAtual;
