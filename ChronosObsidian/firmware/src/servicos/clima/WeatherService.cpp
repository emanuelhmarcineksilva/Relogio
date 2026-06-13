#include "ChronosObsidian.h"

void gerarMsgClima() {
  int tipoClima = getTipoClima(temperatura, climaAtual.weather_code);
  
  switch(tipoClima) {
    case 0:  // NEVE (T < 15)
      strncpy(msgClima, "Muito frio! Agasalho", sizeof(msgClima) - 1);
      break;
    case 1:  // GUARDA-CHUVA (chuvoso)
      strncpy(msgClima, "Leve guarda-chuva!", sizeof(msgClima) - 1);
      break;
    case 2:  // NUVEM (nublado)
      strncpy(msgClima, "Dia nublado", sizeof(msgClima) - 1);
      break;
    case 3:  // SOL (claro)
      strncpy(msgClima, "Dia lindo!", sizeof(msgClima) - 1);
      break;
    case 4:  // TERMÔMETRO (T >= 25)
      strncpy(msgClima, "Dia quente! Hidrate", sizeof(msgClima) - 1);
      break;
    default:
      strncpy(msgClima, "Clima indefinido", sizeof(msgClima) - 1);
  }
  msgClima[sizeof(msgClima) - 1] = '\0';  // Garante terminação nula
}



// [TAG_CLIMA_API]
// === 11. CLIMA (INTEGRAÇÃO METEOSOURCE - FASE 1.1) ===

// Busca a previsão do tempo diária na API Meteosource (HTTP não bloqueante)
// Atualiza a estrutura global climaAtual e a variável temperatura
// Temporização: Chamada periodicamente (a cada 10 minutos por padrão)
// Não bloqueante: Usa HTTPClient com timeout curto para evitar travamento
void pegarClima() {
  uint64_t t0 = esp_timer_get_time(); // Inicia o temporizador de desempenho
  
  if (WiFi.status() != WL_CONNECTED) {
    logWarn("[CLIMA] WiFi desconectado, pulando requisicao\n");
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }

  HTTPClient http;
  http.setConnectTimeout(5000);   // Timeout de conexão 5s
  http.setTimeout(5000);          // Timeout de resposta 5s
  
  logInfo("[CLIMA] Buscando previsao Meteosource para %s...\n", cfgClimaPlaceId);
  
  // Constrói a URL dinamicamente com a cidade configurada
  String url = String(METEOSOURCE_URL);
  url.replace("?place_id=curitiba", "?place_id=" + String(cfgClimaPlaceId));
  
  // HTTPS (seguro) - Meteosource usa HTTPS para o plano gratuito
  if (!http.begin(url)) {
    logError("[CLIMA] Falha ao inicializar HTTP\n");
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  int httpCode = http.GET();
  
  if (httpCode != 200) {
    logWarn("[CLIMA] HTTP error: %d\n", httpCode);
    http.end();
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  // Analisa a resposta JSON
  // Estrutura esperada: {daily: {data: [{all_day: {weather: "...", icon: N, temperature_min: X, temperature_max: Y}}]}}
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, http.getStream());
  
  if (error) {
    logError("[CLIMA] JSON parse error: %s\n", error.c_str());
    http.end();
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  // Extrai dados do tempo da primeira entrada diária
  // Meteosource agrupa os dados sob: daily -> data -> [0] -> all_day
  if (doc["daily"]["data"].size() > 0) {
    JsonObject daily = doc["daily"]["data"][0];
    
    // Temperatura atual (valor principal para exibição)
    if (daily["all_day"]["temperature"].is<float>()) {
      temperatura = daily["all_day"]["temperature"].as<float>();
      climaAtual.temp_current = temperatura;
    }
    
    // Extremos diários
    if (daily["all_day"]["temperature_min"].is<float>()) {
      climaAtual.temp_min = daily["all_day"]["temperature_min"].as<float>();
    }
    if (daily["all_day"]["temperature_max"].is<float>()) {
      climaAtual.temp_max = daily["all_day"]["temperature_max"].as<float>();
    }
    
    // Código do tempo (usado para seleção de ícone)
    if (daily["all_day"]["weather"].is<int>()) {
      climaAtual.weather_code = daily["all_day"]["weather"].as<int>();
    }
    
    // Marca como válido e atualiza o carimbo de tempo
    climaAtual.valid = true;
    climaAtual.last_fetch = millis();
    
    logInfo("[CLIMA] Sucesso! T=%.1f°C Tmin=%.1f Tmax=%.1f Code=%d\n",
      temperatura, climaAtual.temp_min, climaAtual.temp_max, climaAtual.weather_code);
  } else {
    logWarn("[CLIMA] Resposta vazia ou mal formatada\n");
  }
  
  http.end();
  tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0); // Stop timer
}
