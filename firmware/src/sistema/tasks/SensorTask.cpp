#include "ChronosObsidian.h"


void TaskAquisicao(void *pvParameters) { // essa task faz a leitura dos sensores e envia para a fila sensorQueue
  logPrintln("[TASK] TaskAquisicao iniciada (Core 0)");
  SensorData dados = {};
  
  while (1) {
    // Executa aquisição a cada TASK_AQUISICAO_INTERVAL ms
    if (millis() - taskAquisicaoLastRun >= TASK_AQUISICAO_INTERVAL) {
      taskAquisicaoLastRun = millis();
      
      // ==================== LEITURA DHT22 ====================
      // Lê temperatura e umidade locais medindo latência (us) via DhtService
      float t, h;
      if (lerDadosDHT(t, h)) {
        dados.dht_temp = t;
        dados.dht_humidity = h;
        dados.valid = true;
      } else {
        logWarn("[TASK-ACQ] DHT22 Service read failed\n");
        dados.dht_temp = NAN;
        dados.dht_humidity = NAN;
      }
      
      // ==================== API METEOSOURCE ====================
      // Busca previsão do tempo (a cada 10 minutos via cfgClimaIntervaloMs)
      static unsigned long lastClimaFetch = 0;
      if (millis() - lastClimaFetch >= cfgClimaIntervaloMs && WiFi.status() == WL_CONNECTED) {
        lastClimaFetch = millis();
        pegarClima();  // Atualiza temperatura global, climaAtual
        dados.api_temp = temperatura;
        dados.weather_code = climaAtual.weather_code;
        logInfo("[TASK-ACQ] API Clima atualizada\n"); // Log simplificado
      } else {
        dados.api_temp = temperatura;
        dados.weather_code = climaAtual.weather_code;
      }
      
      // Record timestamp and validity
      dados.timestamp_ms = millis();
      dados.valid = true;
      
      // ==================== ESCREVE NA FILA ====================
      // xQueueOverwrite: sempre substitui o mais antigo (fila de 1 elemento)
      // Sem espera - se a fila estiver cheia, sobrescreve; sempre tem sucesso
      if (sensorQueue != NULL) {
        xQueueOverwrite(sensorQueue, &dados);
        // Atualiza fallback global
        sensorDataCurrent = dados;
      }
    }
    
    // Atraso mínimo para evitar timeout do watchdog (cedência de 50ms)
    vTaskDelay(50 / portTICK_PERIOD_MS); // o v
  }
}
