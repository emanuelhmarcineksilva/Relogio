#include "ChronosObsidian.h"

void setup() {
  Serial.begin(115200);
  bootStepTime = millis();
}

bool bootStep() {
  unsigned long agora = millis();
  switch (bootState) {
    case BOOT_SERIAL_INIT:
      Serial.println();
      Serial.println(F("========================================"));
      Serial.println(F(" Relogiov3 - BOOT SEM BLOQUEIO"));
      Serial.println(F("========================================"));
      bootState = BOOT_DISPLAY;
      bootStepTime = agora;
      break;
    case BOOT_DISPLAY:
      Wire.begin(SDA_PIN, SCL_PIN);
      oledOkBoot = displayTela.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
      if (oledOkBoot) { displayTela.clearDisplay(); displayTela.setTextColor(SSD1306_WHITE); oledMsg("[1/9] Display", "OK"); }
      bootState = BOOT_LCD; bootStepTime = agora; break;
    case BOOT_LCD:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      iniciarLCD();
      if (oledOkBoot) oledMsg("[2/9] LCD", lcdOkBoot ? "OK" : "ERRO");
      bootState = BOOT_SPIFFS; bootStepTime = agora; break;
    case BOOT_SPIFFS:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      SPIFFS.begin(true); carregarPerfFlash(); carregarWifiConfig(); carregarConfigParams();
      if (oledOkBoot) oledMsg("[3/9] SPIFFS", "OK");
      bootState = BOOT_RTC; bootStepTime = agora; break;
    case BOOT_RTC:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (iniciarRTC()) {
        if (oledOkBoot) oledMsg("[4/9] RTC", "OK");
      } else {
        if (oledOkBoot) oledMsg("[4/9] RTC", "ERRO");
      }
      bootState = BOOT_I2S; bootStepTime = agora; break;
    case BOOT_I2S:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      iniciarI2S(); if (oledOkBoot) oledMsg("[5/9] Audio", i2sIniciado ? "OK" : "Falhou");
      bootState = BOOT_BOTOES; bootStepTime = agora; break;
     case BOOT_BOTOES: {
       if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
       
       // ===== FASE 2.2: INICIALIZAR ISR DE BOTÕES + FILA =====
       // Cria fila de eventos de botão (5 elementos)
       buttonEventQueue = xQueueCreate(5, sizeof(ButtonEvent));
       if (buttonEventQueue == NULL) {
         logError("[BOOT] Falha ao criar buttonEventQueue\n");
       } else {
         logPrintln("[BOOT] buttonEventQueue criada");
       }
       
       // Cria timers de debounce
       esp_timer_create_args_t btn1_timer_args = {
         .callback = &btn1_debounce_callback,
         .arg = NULL,
         .dispatch_method = ESP_TIMER_TASK,
         .name = "btn1_debounce"
       };
       esp_timer_create(&btn1_timer_args, &btn1_debounce_timer);
       
       esp_timer_create_args_t btn2_timer_args = {
         .callback = &btn2_debounce_callback,
         .arg = NULL,
         .dispatch_method = ESP_TIMER_TASK,
         .name = "btn2_debounce"
       };
       esp_timer_create(&btn2_timer_args, &btn2_debounce_timer);
       
       // Configura pinos GPIO com ISR na borda de descida (falling) HIGH→LOW
       pinMode(pinoButton1, INPUT_PULLUP);
       pinMode(pinoButton2, INPUT_PULLUP);
       attachInterrupt(digitalPinToInterrupt(pinoButton1), btn1_isr, FALLING);
       attachInterrupt(digitalPinToInterrupt(pinoButton2), btn2_isr, FALLING);
       
       logPrintln("[BOOT] ISRs de botões inicializadas");
       
       if (oledOkBoot) oledMsg("[6/9] Botoes", "OK + ISR");
       bootState = BOOT_WIFI_START; bootStepTime = agora; break;
     }
    case BOOT_WIFI_START:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[7/9] WiFi", wifiSSID);
      logPrintf("[BOOT] Tentando WiFi: %s\n", wifiSSID);
      WiFi.mode(WIFI_STA); WiFi.setSleep(false); WiFi.setAutoReconnect(true); WiFi.begin(wifiSSID, wifiPassword);
      bootWifiT0 = agora; bootState = BOOT_WIFI_WAIT; break;
    case BOOT_WIFI_WAIT:
      if (WiFi.status() == WL_CONNECTED || (agora - bootWifiT0) >= WIFI_TIMEOUT_MS) { bootState = BOOT_WIFI_RESULT; bootStepTime = agora; }
      break;
    case BOOT_WIFI_RESULT:
      if (WiFi.status() == WL_CONNECTED) {
        modoAP = false;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char ipBuf[30];
          snprintf(ipBuf, sizeof(ipBuf), "IP: %s", WiFi.localIP().toString().c_str());
          oledMsg("[7/9] WiFi OK", ipBuf);
        }
        logPrintf("[BOOT] WiFi conectado: %s\n", WiFi.localIP().toString().c_str());
      } else {
        modoAP = true;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char apBuf[30];
          snprintf(apBuf, sizeof(apBuf), "AP: %s", WiFi.softAPIP().toString().c_str());
          oledMsg("[7/9] MODO AP", apBuf);
        }
        logWarn("[BOOT] WiFi falhou. AP ativo: RelogioConfig / 12345678\n");
      }
      bootState = BOOT_NTP; bootStepTime = agora; break;
    case BOOT_NTP:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[8/9] NTP", "Sincronizando");
      if (WiFi.status() == WL_CONNECTED) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        struct tm ti;
        if (getLocalTime(&ti, 5000)) {
          sincronizarRTC(ti);
          if (oledOkBoot) oledMsg("[8/9] NTP", "OK");
        } else {
          if (oledOkBoot) oledMsg("[8/9] NTP", "Falhou");
        }
      } else {
        if (oledOkBoot) oledMsg("[8/9] NTP", "Sem WiFi");
      }
      bootState = BOOT_SERVER; bootStepTime = agora; break;
    case BOOT_SERVER:
      if (oledOkBoot) oledMsg("[9/9] Server", "Iniciando");
      iniciarServidorWeb(); bootState = BOOT_DHT; bootStepTime = agora; break;
     case BOOT_DHT: {
       iniciarDHT();
       inicializarAlarmes();
       ultimoBackupFlash = millis();
       
       // ===== FASE 2.1: INICIALIZAR TAREFAS RTOS =====
       // Cria fila de sensores (1 elemento, 100 bytes por elemento)
       sensorQueue = xQueueCreate(1, sizeof(SensorData));
       if (sensorQueue == NULL) {
         logError("[BOOT] Falha ao criar sensorQueue\n");
       } else {
         logPrintln("[BOOT] sensorQueue criada");
       }
       
       // Cria TaskAquisicao no Core 0 (prioridade alta)
       BaseType_t resultA = xTaskCreatePinnedToCore(
         TaskAquisicao,                    // Função a executar
         "TaskAquisicao",                  // Nome da tarefa (para depuração)
         4096,                             // Tamanho da pilha (bytes) - ops de sensor + HTTP
         NULL,                             // Parâmetros da tarefa
         TASK_AQUISICAO_PRIORITY,          // Prioridade (alta)
         &taskAquisicaoHandle,             // Handle da tarefa
         0                                 // Core 0 (WiFi/rede)
       );
       
       if (resultA == pdPASS) {
         logPrintln("[BOOT] TaskAquisicao criada no Core 0");
       } else {
         logError("[BOOT] Falha ao criar TaskAquisicao\n");
       }
       
       // Cria TaskUI no Core 1 (prioridade média-alta)
       BaseType_t resultU = xTaskCreatePinnedToCore(
         TaskUI,                           // Função a executar
         "TaskUI",                         // Nome da tarefa
         4096,                             // Tamanho da pilha (bytes) - ops de display
         NULL,                             // Parâmetros da tarefa
         TASK_UI_PRIORITY,                 // Prioridade (média-alta)
         &taskUIHandle,                    // Handle da tarefa
         1                                 // Core 1 (app/display)
       );
       
       if (resultU == pdPASS) {
         logPrintln("[BOOT] TaskUI criada no Core 1");
       } else {
         logError("[BOOT] Falha ao criar TaskUI\n");
       }
       
       // Cria TaskAudio no Core 1 (prioridade média, para não bloquear UI)
       BaseType_t resultAudio = xTaskCreatePinnedToCore(
         TaskAudio,                        // Função a executar
         "TaskAudio",                      // Nome da tarefa
         2048,                             // Tamanho da pilha (bytes) - reprodução de som
         NULL,                             // Parâmetros da tarefa
         TASK_AUDIO_PRIORITY,              // Prioridade (média)
         &taskAudioHandle,                 // Handle da tarefa
         1                                 // Core 1 (app/áudio)
       );
       
       if (resultAudio == pdPASS) {
         logPrintln("[BOOT] TaskAudio criada no Core 1");
       } else {
         logError("[BOOT] Falha ao criar TaskAudio\n");
       }
       
       // Cria TaskPersistencia (Core desindexado, baixa prioridade)
       BaseType_t resultPers = xTaskCreatePinnedToCore(
         TaskPersistencia,                 // Função a executar
         "TaskPersistencia",               // Nome da tarefa
         3072,                             // Tamanho da pilha (bytes) - operações de file I/O
         NULL,                             // Parâmetros da tarefa
         TASK_PERSISTENCIA_PRIORITY,       // Prioridade (BAIXA)
         &taskPersistenciaHandle,          // Handle da tarefa
         tskNO_AFFINITY                    // Sem afinidade de core (executa em qualquer um)
       );
       
       if (resultPers == pdPASS) {
         logPrintln("[BOOT] TaskPersistencia criada (BAIXA PRIORIDADE)");
       } else {
         logError("[BOOT] Falha ao criar TaskPersistencia\n");
       }
       
       if (oledOkBoot) {
         if (WiFi.status() == WL_CONNECTED) {
           char urlBuf[40];
           snprintf(urlBuf, sizeof(urlBuf), "http://%s", WiFi.localIP().toString().c_str());
           oledMsg("[OK] Sistema", urlBuf);
         } else {
           oledMsg("[OK] Sistema", "AP: 192.168.4.1");
         }
       }
       bootState = BOOT_COMPLETO;
       break;
     }
    case BOOT_COMPLETO:
      logPrintln("BOOT COMPLETO");
      last_activity_ms = millis();
      return true;
  }
  return false;
}
