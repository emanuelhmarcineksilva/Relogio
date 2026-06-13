#include "ChronosObsidian.h"

// ============================================================================
// TAREFA: TaskPersistencia (Core desindexado, baixa prioridade)
// ============================================================================
// Responsabilidade: Persistência de dados em flash (SPIFFS)
// - Backup periódico de logs
// - Salvamento de histórico de performance (a cada 24h ou intervalo config)
// - Gerenciamento de arquivos e limpeza de cache
// - Restauração de dados corrompidos
//
// Comunicação: Via variáveis globais (ultimoBackupFlash, logBuffer, perfHist)
// Executada com baixa prioridade para não interferir no boot ou operação real-time
// ============================================================================

void TaskPersistencia(void *pvParameters) {
  logPrintln("[TASK] TaskPersistencia iniciada (BAIXA PRIORIDADE)");
  
  unsigned long ultimoBackupLog = millis();
  unsigned long ultimoBackupPerf = millis();
  
  while (1) {
    uint64_t tPerf0 = esp_timer_get_time();
    
    // ==================== BACKUP DE LOGS (A cada 30 minutos) ====================
    if (millis() - ultimoBackupLog >= 1800000) {  // 1800000ms = 30min
      ultimoBackupLog = millis();
      
      logPrintln("[TASK-PERSIST] Iniciando backup de logs...");
      salvarLogFlash();
      logInfo("[TASK-PERSIST] Logs salvos em /log_backup.txt\n");
    }
    
    // ==================== BACKUP DE PERFORMANCE (A cada 2 horas) ====================
    if (millis() - ultimoBackupPerf >= 7200000) {  // 7200000ms = 2h
      ultimoBackupPerf = millis();
      
      logPrintln("[TASK-PERSIST] Iniciando backup de performance...");
      salvarPerfFlash();  // Inclui CRC para validação
      logInfo("[TASK-PERSIST] Performance salva em /perf_backup.bin\n");
    }
    
    // ==================== VERIFICAÇÃO DE INTEGRIDADE FLASH (A cada 6 horas) ====================
    static unsigned long ultimoCheckIntegridade = 0;
    if (millis() - ultimoCheckIntegridade >= 21600000) {  // 21600000ms = 6h
      ultimoCheckIntegridade = millis();
      
      logPrintln("[TASK-PERSIST] Verificação de integridade da flash...");
      validarIntegridadeFlash();
      logInfo("[TASK-PERSIST] Verificação concluída\n");
    }
    
    // ==================== COLETA DE MÉTRICAS DA TAREFA ====================
    uint32_t tempoPerf = (uint32_t)(esp_timer_get_time() - tPerf0);
    if (tempoPerf > 1000) {  // Log somente se levou mais de 1ms
      logInfo("[TASK-PERSIST] Tempo de execução: %lu µs\n", tempoPerf);
    }
    
    // Atraso: executar a cada 5 minutos (para não sobrecarregar o sistema)
    // A maioria do tempo a tarefa está dormindo
    vTaskDelay(300000 / portTICK_PERIOD_MS);  // 300000ms = 5min
  }
}

// ============================================================================
// FUNÇÃO AUXILIAR: Validar Integridade de Dados
// ============================================================================
void validarIntegridadeFlash() {
  // Verifica se os arquivos críticos existem e têm tamanho razoável
  
  if (SPIFFS.exists("/wifi_config.bin")) {
    File f = SPIFFS.open("/wifi_config.bin", "r");
    if (f && f.size() > 0) {
      logInfo("[PERSIST] WiFi config: OK (%d bytes)\n", f.size());
    } else {
      logWarn("[PERSIST] WiFi config: CORROMPIDO ou vazio\n");
    }
    if (f) f.close();
  } else {
    logWarn("[PERSIST] WiFi config: NÃO ENCONTRADO\n");
  }
  
  if (SPIFFS.exists("/perf_backup.bin")) {
    File f = SPIFFS.open("/perf_backup.bin", "r");
    if (f && f.size() > 0) {
      logInfo("[PERSIST] Perf backup: OK (%d bytes)\n", f.size());
    } else {
      logWarn("[PERSIST] Perf backup: VAZIO\n");
    }
    if (f) f.close();
  }
  
  if (SPIFFS.exists("/log_backup.txt")) {
    File f = SPIFFS.open("/log_backup.txt", "r");
    if (f && f.size() > 0) {
      logInfo("[PERSIST] Log backup: OK (%d bytes)\n", f.size());
    }
    if (f) f.close();
  }
  
  // Espaço livre em SPIFFS
  size_t totalBytes = SPIFFS.totalBytes();
  size_t usedBytes = SPIFFS.usedBytes();
  logInfo("[PERSIST] Flash: %d/%d bytes usados (%.1f%%)\n",
    usedBytes, totalBytes, (usedBytes * 100.0) / totalBytes);
}

// ============================================================================
// ESTRUTURA DE DADOS: EstruturaBackup (para rastreamento)
// ============================================================================
struct EstruturaBackup {
  unsigned long ultimoBackupLog;
  unsigned long ultimoBackupPerf;
  unsigned long ultimoBackupConfig;
  uint32_t crcUltimoBackup;
  bool backupEmProgresso;
};

// [TODO] Implementar:
// - Rotação de arquivos de backup (manter últimos N backups)
// - Compressão de histórico antigo
// - Sincronização com servidor remoto (nuvem)
// - Recuperação automática de corrupção de dados
