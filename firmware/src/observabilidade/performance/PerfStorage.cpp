#include "ChronosObsidian.h"

uint32_t crc32_simple(uint32_t *data, size_t len) {
  uint32_t crc = 0xFFFFFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 32; j++) {
      if (crc & 0x80000000) {
        crc = (crc << 1) ^ 0x04C11DB7;
      } else {
        crc = crc << 1;
      }
    }
  }
  return crc ^ 0xFFFFFFFF;
}

// Salva histórico de performance na flash (binário) com CRC (FASE 2.4)
void salvarPerfFlash() {
  File f = SPIFFS.open("/perf_hist.bin", FILE_WRITE);
  if (!f) { logPrintln("[PERF] Erro ao salvar!"); return; }
  
  // Calculate CRC before saving
  perfHistCRC = crc32_simple((uint32_t*)perfHist, PERF_FUNCOES * PERF_HIST_SIZE);
  
  // Write data
  f.write((uint8_t*)perfHist, sizeof(perfHist));
  f.write((uint8_t*)&perfHistPos, sizeof(perfHistPos));
  uint8_t fb = perfHistFull ? 1 : 0;
  f.write(&fb, 1);
  f.write((uint8_t*)&perfHistCRC, sizeof(perfHistCRC));  // CRC at end
  
  f.close();
  logPrintf("[PERF] Historico salvo na flash! (CRC: 0x%08X, %d samples)\n", 
    perfHistCRC, PERF_HIST_SIZE);
}

// Carrega histórico de performance da flash com CRC validation (FASE 2.4)
void carregarPerfFlash() {
  if (!SPIFFS.exists("/perf_hist.bin")) {
    logPrintln("[PERF] Arquivo /perf_hist.bin nao encontrado, iniciando novo");
    return;
  }
  
  File f = SPIFFS.open("/perf_hist.bin", FILE_READ);
  if (!f) return;
  
  // Expected size: perfHist + perfHistPos + perfHistFull + CRC
  size_t expected = sizeof(perfHist) + sizeof(perfHistPos) + 1 + sizeof(perfHistCRC);
  
  if (f.size() == expected) {
    // Read all data
    f.read((uint8_t*)perfHist, sizeof(perfHist));
    f.read((uint8_t*)&perfHistPos, sizeof(perfHistPos));
    uint8_t fb = 0;
    f.read(&fb, 1);
    perfHistFull = (fb == 1);
    
    uint32_t savedCRC = 0;
    f.read((uint8_t*)&savedCRC, sizeof(savedCRC));
    
    // Validate CRC
    uint32_t calculatedCRC = crc32_simple((uint32_t*)perfHist, PERF_FUNCOES * PERF_HIST_SIZE);
    
    if (calculatedCRC == savedCRC) {
      // CRC matches: data is valid
      if (perfHistPos < 0 || perfHistPos >= PERF_HIST_SIZE) perfHistPos = 0;
      logPrintf("[PERF] Historico restaurado da flash (CRC valido: 0x%08X, 24h data)\n", 
        calculatedCRC);
    } else {
      // CRC mismatch: data corrupted
      logWarn("[PERF] CRC invalido! Esperava 0x%08X, obteve 0x%08X - iniciando novo\n",
        calculatedCRC, savedCRC);
      // Reset data
      memset(perfHist, 0, sizeof(perfHist));
      perfHistPos = 0;
      perfHistFull = false;
    }
  } else {
    logWarn("[PERF] Tamanho invalido: %d bytes (esperado %d)\n", f.size(), expected);
  }
  
  f.close();
}
