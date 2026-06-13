#pragma once
#include <Arduino.h>

struct ButtonEvent {
  int button_id;
  unsigned long timestamp_ms;
  uint8_t debounce_state;
};
