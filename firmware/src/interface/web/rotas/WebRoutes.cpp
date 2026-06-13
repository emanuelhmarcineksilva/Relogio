#include "ChronosObsidian.h"

const char* estadoToStr(String st) {
  if (st == "RELOGIO")          return "RELOGIO";
  if (st == "MENU")             return "MENU";
  if (st == "LISTA_ALARMES")    return "LISTA_ALARMES";
  if (st == "DETALHE_ALARME")   return "DETALHE_ALARME";
  if (st == "EDITAR_ALARME_H")  return "EDITAR_ALARME_H";
  if (st == "EDITAR_ALARME_M")  return "EDITAR_ALARME_M";
  if (st == "ESCOLHER_MELODIA") return "ESCOLHER_MELODIA";
  if (st == "HORA")             return "HORA";
  if (st == "EDITAR_HORA_M")    return "EDITAR_HORA_M";
  return "?";
}

// GET / — página HTML
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

// GET /api/log — texto formatado como terminal
void handleLog() {
  DateTime agora = rtc.now();
  bool wOk = (WiFi.status() == WL_CONNECTED);
  char buf[800];
  char alarmTxt[300];
  char* p = alarmTxt;

  for (int i = 0; i < NUM_ALARMES; i++) {
    p += sprintf(p, "#%d: %02d:%02d [%s] Mel: %s\n",
      i + 1, alarmes[i].hora, alarmes[i].minuto,
      alarmes[i].ativo ? "ON " : "OFF",
      MELODIA_NOMES[alarmes[i].melodia]);
  }

   snprintf(buf, sizeof(buf),
     "=== RELOGIO INTELIGENTE ===\n"
     "Estado: %s\n"
     "Hora RTC: %02d:%02d:%02d\n"
     "Uptime: %lu s\n\n"
     "--- ALARMES ---\n"
     "%s"
     "Disparando: %s%s\n\n"
     "--- REDE ---\n"
     "WiFi: %s (RSSI %d)\n"
     "IP: %s\n\n"
     "--- CLIMA ---\n"
     "Local (DHT): %.1f C\n"
     "Previsao (API): %.1f C (%s)\n"
     "Mensagem: %s\n\n"
     "--- PERFORMANCE ---\n"
     "Loop: %lu us\n"
     "CPU: %.1f%%\n"
     "Heap: %lu / min %lu\n"
     "Stack: %lu bytes\n"
     "Light Sleep: %s\n",
     estadoToStr(estadoAtual),
     agora.hour(), agora.minute(), agora.second(),
     (unsigned long)(millis() / 1000UL),
     alarmTxt,
     alarmeDisparo ? "SIM (Alarme " : "NAO",
     alarmeDisparo ? (String(alarmeDisparoIdx + 1) + ")").c_str() : "",
     wOk ? "OK" : "OFF", wOk ? WiFi.RSSI() : 0,
     wOk ? WiFi.localIP().toString().c_str() : "...",
      isnan(dhtTemperature) ? 0.0f : dhtTemperature,
      temperatura, METEOSOURCE_PLACE_ID,
      msgClima,
     (unsigned long)tempoLoopUs,
     cpuUsoEstimado,
     (unsigned long)ESP.getFreeHeap(),
     (unsigned long)ESP.getMinFreeHeap(),
     (unsigned long)(stackLoopWords * sizeof(StackType_t)),
     sleep_mode_active ? "ATIVADO" : "DESATIVADO"
   );
  server.send(200, "text/plain", buf);
}

// GET /api/alarmes — JSON com alarmes + temp + hora + uptime + local
void handleAlarmes() {
  DateTime agora = rtc.now();
  char horaStr[12];
  snprintf(horaStr, sizeof(horaStr), "%02d:%02d:%02d", agora.hour(), agora.minute(), agora.second());
  char json[600];
  char* p = json;
  float dhtTemp = isnan(dhtTemperature) ? -127.0f : dhtTemperature;
  float dhtHum  = isnan(dhtHumidity) ? -127.0f : dhtHumidity;
  p += sprintf(p, "{\"alarmes\":[");
  for (int i = 0; i < NUM_ALARMES; i++) {
    if (i > 0) p += sprintf(p, ",");
    p += sprintf(p, "{\"h\":%d,\"m\":%d,\"a\":%s,\"mel\":%d}",
      alarmes[i].hora, alarmes[i].minuto,
      alarmes[i].ativo ? "true" : "false",
      alarmes[i].melodia);
  }
  p += sprintf(p, "],\"temp\":%.1f,\"hum\":%.1f,\"dhtTemp\":%.1f,\"dhtHum\":%.1f,\"hora\":\"%s\",\"uptime\":%lu,\"cidade\":\"%s\"}",
    temperatura,
    dhtHum,
    dhtTemp,
    dhtHum,
    horaStr, (unsigned long)(millis()/1000UL), METEOSOURCE_PLACE_ID);
  server.send(200, "application/json", json);
}

// GET /api/hora?h=HH&m=MM — ajusta o RTC
void handleSetHora() {
  if (server.hasArg("h") && server.hasArg("m")) {
    int h = server.arg("h").toInt();
    int m = server.arg("m").toInt();
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), h, m, 0));
    logPrintf("[WEB] Hora ajustada: %02d:%02d\n", h, m);
    last_activity_ms = millis();
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Faltam h e m");
  }
}

// GET /api/alarme?id=N&h=HH&m=MM&mel=M — edita um alarme
void handleSetAlarme() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id >= 0 && id < NUM_ALARMES) {
      if (server.hasArg("h"))   alarmes[id].hora    = server.arg("h").toInt();
      if (server.hasArg("m"))   alarmes[id].minuto  = server.arg("m").toInt();
      if (server.hasArg("mel")) alarmes[id].melodia = server.arg("mel").toInt() % TOTAL_MELODIAS;
      logPrintf("[WEB] Alarme %d: %02d:%02d mel=%d\n", id+1, alarmes[id].hora, alarmes[id].minuto, alarmes[id].melodia);
      last_activity_ms = millis();
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "ID invalido");
    }
  } else {
    server.send(400, "text/plain", "Falta id");
  }
}

// GET /api/alarme/toggle?id=N — liga/desliga um alarme
void handleToggleAlarme() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id >= 0 && id < NUM_ALARMES) {
      alarmes[id].ativo = !alarmes[id].ativo;
      logPrintf("[WEB] Alarme %d: %s\n", id+1, alarmes[id].ativo ? "ON" : "OFF");
      last_activity_ms = millis();
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "ID invalido");
    }
  }
}

// GET /api/clima — força atualização do clima
void handleClima() {
  pegarClima();
  char buf[30];
  snprintf(buf, sizeof(buf), "Temp: %.1f C", temperatura);
  server.send(200, "text/plain", buf);
}

// GET /api/serial — log serial em tempo real (ring buffer)
void handleSerial() {
  server.send(200, "text/plain", getLogContent());
}

// GET /api/flash — log salvo na flash (SPIFFS)
void handleFlash() {
  server.send(200, "text/plain", carregarLogFlash());
}

// GET /api/config — retorna parametros configuraveis atuais
void handleConfigGet() {
  String json = "{";
  json += "\"dhtMs\":" + String(cfgDhtIntervaloMs) + ",";
  json += "\"climaMs\":" + String(cfgClimaIntervaloMs) + ",";
  json += "\"backupMs\":" + String(cfgBackupIntervaloMs) + ",";
  json += "\"cidade\":\"" + String(cfgClimaPlaceId) + "\",";
  json += "\"ssid\":\"" + String(wifiSSID) + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// GET /api/config/set?dht=&clima=&backup=&cidade= — salva parametros da aba Config
void handleConfigSet() {
  if (server.hasArg("dht")) cfgDhtIntervaloMs = max(2000UL, (unsigned long)server.arg("dht").toInt());
  if (server.hasArg("clima")) cfgClimaIntervaloMs = max(60000UL, (unsigned long)server.arg("clima").toInt());
  if (server.hasArg("backup")) cfgBackupIntervaloMs = max(300000UL, (unsigned long)server.arg("backup").toInt());
  if (server.hasArg("cidade")) {
    String novaCidade = server.arg("cidade");
    novaCidade.trim();
    if (novaCidade.length() > 0 && novaCidade.length() < sizeof(cfgClimaPlaceId)) {
      strncpy(cfgClimaPlaceId, novaCidade.c_str(), sizeof(cfgClimaPlaceId) - 1);
      cfgClimaPlaceId[sizeof(cfgClimaPlaceId) - 1] = '\0';
    }
  }
  salvarConfigParams();
  logPrintf("[CFG] Parametros salvos: DHT=%lu Clima=%lu Backup=%lu Cidade=%s\n", cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs, cfgClimaPlaceId);
  server.send(200, "text/plain", "Parametros salvos");
}

// GET /api/wifi/set?ssid=&pass= — troca SSID/senha e tenta conectar sem regravar firmware
void handleWifiSet() {
  if (!server.hasArg("ssid") || !server.hasArg("pass")) {
    server.send(400, "text/plain", "Envie ssid e pass");
    return;
  }
  String novoSsid = server.arg("ssid");
  String novaSenha = server.arg("pass");
  novoSsid.trim();
  if (novoSsid.length() == 0) {
    server.send(400, "text/plain", "SSID vazio");
    return;
  }
  strncpy(wifiSSID, novoSsid.c_str(), sizeof(wifiSSID) - 1);
  wifiSSID[sizeof(wifiSSID) - 1] = '\0';
  strncpy(wifiPassword, novaSenha.c_str(), sizeof(wifiPassword) - 1);
  wifiPassword[sizeof(wifiPassword) - 1] = '\0';
  salvarWifiConfig();

  // Evita a queda do AP durante a configuração. Reorganizado idêntico funcionalmente,
  // mas mantendo o AP ativo ativamente.
  WiFi.disconnect(true, false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("RelogioConfig", "12345678");
  WiFi.begin(wifiSSID, wifiPassword);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - t0) < 15000) {
    yield();
  }

  if (WiFi.status() == WL_CONNECTED) {
    modoAP = false;
    logPrintf("[WIFI] Conectado em nova rede: %s | IP: %s\n", wifiSSID, WiFi.localIP().toString().c_str());
    String msg = "Conectado com sucesso. IP WiFi: ";
    msg += WiFi.localIP().toString();
    msg += " | AP de recuperacao: 192.168.4.1";
    server.send(200, "text/plain", msg);
  } else {
    // Se falhou conectar na nova rede, AP já está ativo.
    modoAP = true;
    logWarn("[WIFI] Falha ao conectar com nova rede. Mantendo configuracao salva para proximo boot.\n");
    server.send(200, "text/plain", "Falha ao conectar agora. AP ativo em 192.168.4.1 (RelogioConfig/12345678).");
  }
}

// GET /api/perf — histórico de performance (JSON)
void handlePerf() {
  String json;
  json.reserve(7000);
  json += "{\"atual\":["; 
  json += tempoLoopUs;          json += ',';
  json += tempoPegarClimaUs;    json += ',';
  json += tempoRtcNowUs;        json += ',';
  json += tempoLeituraBotoesUs; json += ',';
  json += tempoAtualizarDisplayUs; json += ',';
  json += tempoPegarDHTUS;       json += ',';
  json += tempoLogicaAlarmeUs;
  json += "],\"hist\":[";
  for (int f = 0; f < PERF_FUNCOES; f++) {
    if (f > 0) json += ',';
    json += '[';
    for (int i = 0; i < PERF_HIST_SIZE; i++) {
      if (i > 0) json += ',';
      json += perfHist[f][i];
    }
    json += ']';
  }
  json += "],\"pos\":";
  json += perfHistPos;
  json += ",\"full\":";
  json += perfHistFull ? "true" : "false";
  json += ",\"cpu\":";
  json += cpuUsoEstimado;
  json += ",\"heap\":";
  json += ESP.getFreeHeap();
  json += ",\"minHeap\":";
  json += ESP.getMinFreeHeap();
  json += ",\"stack\":";
  json += (unsigned long)(stackLoopWords * sizeof(StackType_t));
  json += ",\"flashUsed\":";
  json += SPIFFS.usedBytes();
  json += ",\"flashTotal\":";
  json += SPIFFS.totalBytes();
  json += ",\"psramTotal\":";
  json += ESP.getPsramSize();
  json += ",\"psramFree\":";
  json += ESP.getFreePsram();
  json += ",\"tasks\":";
  json += uxTaskGetNumberOfTasks();
  json += ",\"wifi\":\"";
  json += (WiFi.status() == WL_CONNECTED) ? "CONECTADO" : "DESCONECTADO";
  json += "\",\"ap\":\"";
  json += modoAP ? "ATIVO" : "INATIVO";
  json += "\"";
  json += '}';
  server.send(200, "application/json", json);
}

// GET /api/salvar — força salvamento completo na flash
void handleSalvar() {
  salvarLogFlash();
  salvarPerfFlash();
  server.send(200, "text/plain", "OK");
}

// GET /api/download_perf — retorna histórico de performance como texto CSV
void handleDownloadPerf() {
  const char* nomes[] = {"Loop","Clima","RTC","Botoes","DHT Temp","Display","DHT Umidade"};
  String txt = "# Historico de Performance - ESP32 Relogio\n";
  txt += "# Uma amostra a cada 5 segundos\n";
  txt += "# Formato: Funcao:val1,val2,...\n";
  for (int f = 0; f < PERF_FUNCOES; f++) {
    txt += String(nomes[f]) + ":";
    int total = perfHistFull ? PERF_HIST_SIZE : perfHistPos;
    for (int i = 0; i < total; i++) {
      int idx = perfHistFull ? (perfHistPos + i) % PERF_HIST_SIZE : i;
      if (i > 0) txt += ",";
      txt += String(perfHist[f][idx]);
    }
    txt += "\n";
  }
  txt += "pos:" + String(perfHistPos) + "\n";
  txt += "full:" + String(perfHistFull ? "1" : "0") + "\n";
  server.sendHeader("Content-Disposition", "attachment; filename=backup_perf.txt");
  server.send(200, "text/plain", txt);
}

// POST /api/upload_perf — restaura histórico de performance a partir de CSV
//
// IMPORTANTE: o handleDownloadPerf exporta os dados JÁ em ordem cronológica
// (faz o "unroll" do buffer circular). Portanto, ao importar de volta:
//   - Os dados ficam nas posições [0..n-1] em ordem cronológica
//   - perfHistPos = número de amostras importadas (ou PERF_HIST_SIZE se encheu)
//   - perfHistFull = true se importamos PERF_HIST_SIZE amostras
// Usar o pos/full original do arquivo seria errado pois o formato já está "unrolled".
void handleUploadPerf() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Nenhum dado recebido.");
    return;
  }
  String corpo = server.arg("plain");
  const char* nomes[] = {"Loop","Clima","RTC","Botoes","DHT Temp","Display","DHT Umidade"};
  
  // Zera o histrico antes de restaurar
  memset(perfHist, 0, sizeof(perfHist));
  int totalAmostras = 0;  // Conta quantas amostras foram importadas
  
  for (int f = 0; f < PERF_FUNCOES; f++) {
    String prefixo = String(nomes[f]) + ":";
    int inicio = corpo.indexOf(prefixo);
    if (inicio < 0) continue;
    inicio += prefixo.length();
    int fim = corpo.indexOf('\n', inicio);
    if (fim < 0) fim = corpo.length();
    String vals = corpo.substring(inicio, fim);
    vals.trim();  // Remove \r\n residuais
    if (vals.length() == 0) continue;
    int i = 0, pos = 0;
    while (i < PERF_HIST_SIZE && pos <= (int)vals.length()) {
      int virgula = vals.indexOf(',', pos);
      if (virgula < 0) virgula = vals.length();
      String token = vals.substring(pos, virgula);
      token.trim();
      if (token.length() == 0) break;
      perfHist[f][i++] = (uint32_t)token.toInt();
      pos = virgula + 1;
    }
    // Use a contagem da primeira funcao como referencia
    if (f == 0) totalAmostras = i;
  }
  
  // Como o download ja exporta em ordem cronologica (unrolled),
  // os dados estao em [0..totalAmostras-1] e pos = totalAmostras
  if (totalAmostras >= PERF_HIST_SIZE) {
    perfHistPos  = 0;           // Buffer cheio: proximo ponto sobrescreve o [0]
    perfHistFull = true;
  } else {
    perfHistPos  = totalAmostras;  // Proximo ponto vai em [totalAmostras]
    perfHistFull = false;
  }
  
  salvarPerfFlash();
  
  char msg[80];
  snprintf(msg, sizeof(msg), "Backup restaurado: %d amostras, full=%s",
    totalAmostras, perfHistFull ? "sim" : "nao");
  logPrintln(msg);
  server.send(200, "text/plain", msg);
}

// Inicializa todas as rotas
void iniciarServidorWeb() {
  server.on("/",                handleRoot);
  server.on("/api/log",         handleLog);
  server.on("/api/alarmes",     handleAlarmes);
  server.on("/api/hora",        handleSetHora);
  server.on("/api/alarme",      handleSetAlarme);
  server.on("/api/alarme/toggle",handleToggleAlarme);
  server.on("/api/clima",       handleClima);
  server.on("/api/serial",      handleSerial);
  server.on("/api/flash",       handleFlash);
  server.on("/api/config",      handleConfigGet);
  server.on("/api/config/set",  handleConfigSet);
  server.on("/api/wifi/set",    handleWifiSet);
  server.on("/api/perf",        handlePerf);
  server.on("/api/salvar",      handleSalvar);
  server.on("/api/download_perf", handleDownloadPerf);
  server.on("/api/upload_perf",   handleUploadPerf);
  server.begin();
  logPrintln("Servidor Web Iniciado!");
}
