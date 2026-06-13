#pragma once
#include <Arduino.h>

struct Nota {
  uint16_t freq;
  uint16_t durMs;
};

extern const Nota melodia_aurora[] PROGMEM;
extern const Nota melodia_sino[] PROGMEM;
extern const Nota melodia_despertar[] PROGMEM;
extern const Nota melodia_furelise[] PROGMEM;
extern const Nota melodia_estrela[] PROGMEM;
extern const Nota* const MELODIAS[];
extern const char* MELODIA_NOMES[];
extern const char* MELODIA_CURTO[];
