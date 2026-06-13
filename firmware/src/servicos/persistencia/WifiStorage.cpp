#include "ChronosObsidian.h"

void salvarWifiConfig() {
  File f = SPIFFS.open("/wifi_config.txt", FILE_WRITE);
  if (!f) { logError("Falha ao salvar WiFi config!\n"); return; }
  // Formato simples: primeira linha = SSID, segunda = senha
  f.println(wifiSSID);
  f.println(wifiPassword);
  f.close();
  logInfo("WiFi config salva: %s\n", wifiSSID);
}

// Carrega SSID e senha da flash. Retorna true se encontrou.
bool carregarWifiConfig() {
  if (!SPIFFS.exists("/wifi_config.txt")) return false;
  File f = SPIFFS.open("/wifi_config.txt", FILE_READ);
  if (!f) return false;
  // Lê o SSID (primeira linha)
  String ssid = f.readStringUntil('\n');
  ssid.trim();  // Remove espacos e \r\n extras
  // Lê a senha (segunda linha)
  String pass = f.readStringUntil('\n');
  pass.trim();
  f.close();
  if (ssid.length() > 0) {
    // Copia para os arrays globais
    // strncpy = copia texto com limite de tamanho (evita estouro de memoria)
    strncpy(wifiSSID, ssid.c_str(), sizeof(wifiSSID) - 1);
    strncpy(wifiPassword, pass.c_str(), sizeof(wifiPassword) - 1);
    logInfo("WiFi config carregada: %s\n", wifiSSID);
    return true;
  }
  return false;
}
