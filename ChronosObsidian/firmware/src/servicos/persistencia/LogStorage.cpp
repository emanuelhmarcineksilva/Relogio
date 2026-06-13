#include "ChronosObsidian.h"

void salvarLogFlash() {
  File f = SPIFFS.open("/log_backup.txt", FILE_WRITE);
  if (!f) return;
  DateTime agora = rtc.now();
  f.printf("=== BACKUP %02d:%02d ===\n", agora.hour(), agora.minute());
  for (int i = 0; i < NUM_ALARMES; i++) {
    f.printf("A%d:%02d:%02d,%s\n", i+1, alarmes[i].hora, alarmes[i].minuto, alarmes[i].ativo?"ON":"OFF");
  }
  f.printf("T:%.1f|H:%lu|U:%lu\n", temperatura, (unsigned long)ESP.getFreeHeap(), millis()/1000UL);
  f.close();
  logPrintln("[FLASH] Backup salvo!");
}

// Carrega log da flash
String carregarLogFlash() {
  if (!SPIFFS.exists("/log_backup.txt")) return "Sem backup.";
  File f = SPIFFS.open("/log_backup.txt", FILE_READ);
  if (!f) return "Erro ao ler.";
  String content = f.readString();
  f.close();
  return content;
}
