#include "ChronosObsidian.h"

void salvarConfigParams() {
  File f = SPIFFS.open("/config_params.txt", FILE_WRITE);
  if (!f) return;
  f.printf("%lu\n%lu\n%lu\n%s\n", cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs, cfgClimaPlaceId);
  f.close();
  logInfo("Parametros salvos\n");
}

// Carrega os parametros configuraveis da flash
void carregarConfigParams() {
  if (!SPIFFS.exists("/config_params.txt")) return;
  File f = SPIFFS.open("/config_params.txt", FILE_READ);
  if (!f) return;
  cfgDhtIntervaloMs    = f.readStringUntil('\n').toInt();
  cfgClimaIntervaloMs  = f.readStringUntil('\n').toInt();
  cfgBackupIntervaloMs = f.readStringUntil('\n').toInt();
  String placeId = f.readStringUntil('\n');
  placeId.trim();
  if (placeId.length() > 0 && placeId.length() < sizeof(cfgClimaPlaceId)) {
    strncpy(cfgClimaPlaceId, placeId.c_str(), sizeof(cfgClimaPlaceId) - 1);
    cfgClimaPlaceId[sizeof(cfgClimaPlaceId) - 1] = '\0';
  }
  // Protecao: minimos razoaveis para nao travar o sistema
  if (cfgDhtIntervaloMs < 2000) cfgDhtIntervaloMs = 2000;
  if (cfgClimaIntervaloMs < 60000) cfgClimaIntervaloMs = 60000;
  if (cfgBackupIntervaloMs < 300000) cfgBackupIntervaloMs = 300000;
  f.close();
  logInfo("Config carregada: DHT=%lums Clima=%lums Backup=%lums Cidade=%s\n",
    cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs, cfgClimaPlaceId);
}
