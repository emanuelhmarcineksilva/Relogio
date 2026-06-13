#include "ChronosObsidian.h"

const Nota melodia_aurora[] PROGMEM = {
  {N_C4,400},{N_SIL,50},{N_E4,400},{N_SIL,50},{N_G4,400},{N_SIL,50},
  {N_C5,500},{N_SIL,80},{N_E5,500},{N_SIL,80},{N_G5,600},{N_SIL,100},
  {N_C6,800},{N_SIL,300},
  {N_G5,300},{N_SIL,50},{N_E5,300},{N_SIL,50},{N_C5,400},{N_SIL,200},
  {0,0}
};
const Nota melodia_sino[] PROGMEM = {
  {N_E5,500},{N_SIL,50},{N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_G4,1000},{N_SIL,200},
  {N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_E5,500},{N_SIL,50},{N_C5,1000},{N_SIL,300},
  {0,0}
};
const Nota melodia_despertar[] PROGMEM = {
  {N_A4,600},{N_SIL,100},{N_C5,600},{N_SIL,100},{N_E5,800},{N_SIL,200},
  {N_D5,600},{N_SIL,100},{N_F5,600},{N_SIL,100},{N_A5,800},{N_SIL,300},
  {N_E5,400},{N_SIL,80},{N_C5,400},{N_SIL,80},{N_A4,600},{N_SIL,400},
  {0,0}
};
const Nota melodia_furelise[] PROGMEM = {
  {N_E5,250},{N_Ds5,250},{N_E5,250},{N_Ds5,250},{N_E5,250},
  {N_B4,250},{N_D5,250},{N_C5,250},{N_A4,500},{N_SIL,100},
  {N_C4,250},{N_E4,250},{N_A4,250},{N_B4,500},{N_SIL,100},
  {N_E4,250},{N_G4,250},{N_B4,250},{N_C5,500},{N_SIL,200},
  {0,0}
};
const Nota melodia_estrela[] PROGMEM = {
  {N_C4,300},{N_C4,300},{N_G4,300},{N_G4,300},{N_A4,300},{N_A4,300},{N_G4,500},{N_SIL,100},
  {N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,300},{N_D4,300},{N_C4,500},{N_SIL,200},
  {N_G4,300},{N_G4,300},{N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,500},{N_SIL,200},
  {0,0}
};
const Nota* const MELODIAS[] = {
  melodia_aurora, melodia_sino, melodia_despertar, melodia_furelise, melodia_estrela
};
const char* MELODIA_NOMES[] = {"Aurora", "Sino", "Despertar", "Fur Elise", "Estrela"};
const char* MELODIA_CURTO[] = {"Aur", "Sin", "Des", "Fur", "Est"};
