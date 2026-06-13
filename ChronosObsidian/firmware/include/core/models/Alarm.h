#pragma once
#include <Arduino.h>
#include "config/Config.h"

struct Alarme {
  uint8_t hora;
  uint8_t minuto;
  bool    ativo;
  uint8_t melodia;
  bool    jaTocou;
};

extern Alarme alarmes[NUM_ALARMES];
