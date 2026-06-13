#include "ChronosObsidian.h"

void TaskUI(void *pvParameters) {
  logPrintln("[TASK] TaskUI iniciada (Core 1)");
  SensorData dados = {};
  
  while (1) {
    // Executa atualização de UI a cada TASK_UI_INTERVAL ms
    if (millis() - taskUILastRun >= TASK_UI_INTERVAL) {
      taskUILastRun = millis();
      
      // ==================== LER FILA DE SENSORES ====================
      // Leitura não bloqueante: retorna imediatamente se estiver vazia
      // Usa os dados mais recentes (xQueueOverwrite garante que o último esteja disponível)
      if (xQueueReceive(sensorQueue, &dados, 0) == pdTRUE) {
        // A fila tem novos dados: atualiza as variáveis globais
        if (dados.valid) {
          dhtTemperature = dados.dht_temp;
          dhtHumidity = dados.dht_humidity;
          temperatura = dados.api_temp;
          climaAtual.weather_code = dados.weather_code;
        }
      }
      // Se a fila estiver vazia, continua com os últimos valores conhecidos (sem espera)
      
      // ==================== RENDERIZAÇÃO DE UI ====================
      // Aqui você poderia adicionar lógica adicional de renderização de UI se necessário
      // (atualmente lidado no loop() principal para compatibilidade reversa)
      
      // ==================== SERVIDOR WEB ====================
      // Lida com requisições HTTP (não bloqueante, retorna imediatamente)
      server.handleClient();
    }
    
    // Atraso mínimo para evitar timeout do watchdog (cedência de 50ms)
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
