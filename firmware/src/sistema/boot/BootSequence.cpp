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
      SPIFFS.begin(true); carregarPerfFlash(); carregarWifiConfig(); carregarConfigParams(); ChangelogService::init();
      if (oledOkBoot) oledMsg("[3/9] SPIFFS", "OK");
      bootState = BOOT_RTC; bootStepTime = agora; break;
    case BOOT_RTC:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (iniciarRTC()) {
        if (oledOkBoot) oledMsg("[4/9] RTC", "OK");
      } else {
        if (oledOkBoot) oledMsg("[4/9] RTC", "ERRO");
      }
      
      // Inicia sensores e tarefas cedo para evitar bloqueio do boot
      iniciarDHT();
      inicializarAlarmes();
      
      // ===== FASE 2.1: INICIALIZAR TAREFAS RTOS (ANTECIPADO) =====
      sensorQueue = xQueueCreate(1, sizeof(SensorData));
      xTaskCreatePinnedToCore(TaskAquisicao, "TaskAquisicao", 4096, NULL, TASK_AQUISICAO_PRIORITY, &taskAquisicaoHandle, 0);
      xTaskCreatePinnedToCore(TaskUI, "TaskUI", 4096, NULL, TASK_UI_PRIORITY, &taskUIHandle, 1);
      xTaskCreatePinnedToCore(TaskAudio, "TaskAudio", 2048, NULL, TASK_AUDIO_PRIORITY, &taskAudioHandle, 1);
      xTaskCreatePinnedToCore(TaskPersistencia, "TaskPersistencia", 3072, NULL, TASK_PERSISTENCIA_PRIORITY, &taskPersistenciaHandle, tskNO_AFFINITY);

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
       
       // NOVO: Inicializa timer de 50s para idle timeout
       initIdleTimeout();
       
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
        // FASE 1.5: Atualiza clima uma vez no boot
        logPrintln("[BOOT] Buscando clima inicial...");
        pegarClima();
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
        ultimoBackupFlash = millis();
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
