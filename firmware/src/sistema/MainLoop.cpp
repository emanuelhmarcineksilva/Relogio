#include "ChronosObsidian.h"

void loop() {
  if (!bootFinalizado) {
    bootFinalizado = bootStep();
    return;
  }
  uint64_t tLoop0 = esp_timer_get_time();
  unsigned long agoraMs = millis();

  // [TAG_DHT_SENSOR]
  // A leitura do sensor DHT22 agora é feita de forma assíncrona pela TaskAquisicao (Core 0).
  // Os valores são atualizados globalmente através da fila de sensores na TaskUI.


  // ==================================================================
  // 14.0 SERVIDOR WEB
  // ==================================================================
  server.handleClient();

  // ==================================================================
  // 14.1 PLAYER DE MELODIA (não-bloqueante)
  // ==================================================================
  tocarMelodiaStep();

  // ==================================================================
  // 14.2 BOTÕES (FASE 2.2: ISR + FILA DE EVENTOS)
  // ==================================================================
  // Processamento de eventos de botão não bloqueante via ISR + fila
  // Em vez de varrer digitalRead() todo loop, a ISR enfileira eventos
  // O loop principal consome a fila (não bloqueante, timeout 0ms)
  //
  // ISR → timer de debounce 20ms → Fila de eventos → Loop principal consome
  uint64_t tBotoes0 = esp_timer_get_time();

  bool clicou1 = false;
  bool clicou2 = false;
  
  // ===== CONSOME EVENTOS DE BOTÃO DA FILA =====
  // Leitura não bloqueante: retorna imediatamente se a fila estiver vazia
  ButtonEvent btn_event = {};
  while (xQueueReceive(buttonEventQueue, &btn_event, 0) == pdTRUE) {
    // Processa evento de botão confirmado
    if (btn_event.debounce_state == 1) {  // Confirmado após debounce
      if (btn_event.button_id == 1) {
        clicou1 = true;
        logPrintf("[BTN] Botao1 (Nav) | Estado: %s | Tempo: %lu ms\n", 
          estadoAtual.c_str(), btn_event.timestamp_ms);
      } else if (btn_event.button_id == 2) {
        clicou2 = true;
        logPrintf("[BTN] Botao2 (Conf) | Estado: %s | Tempo: %lu ms\n", 
          estadoAtual.c_str(), btn_event.timestamp_ms);
      }
    }
  }

  if (clicou1 || clicou2) {
    last_activity_ms = agoraMs;
  }

  tempoLeituraBotoesUs = (uint32_t)(esp_timer_get_time() - tBotoes0);

  // ==================================================================
  // 14.3 RTC + WIFI
  // ==================================================================
  DateTime agora = obterHoraAtual();
  int hora   = agora.hour();
  int minuto = agora.minute();

  static int minutoAnterior = -1;
  bool precisaRedesenhar = false;

  bool wifiConectado = (WiFi.status() == WL_CONNECTED);
  wifiRSSI = wifiConectado ? WiFi.RSSI() : 0;

  // Busca clima na primeira conexão
  if (wifiConectado && !climaIniciado) {
    pegarClima();
    climaIniciado = true;
    precisaRedesenhar = true;
  }

  if (minuto != minutoAnterior || clicou1 || clicou2) {
    precisaRedesenhar = true;
    minutoAnterior = minuto;
  }

  static String estadoPassado = "";
  if (estadoAtual != estadoPassado) {
    precisaRedesenhar = true;
    estadoPassado = estadoAtual;
    displayTela.clearDisplay();
  }

  // ==================================================================
  // 14.4 LÓGICA DOS 3 ALARMES
  // ==================================================================
  processarAlarmes(hora, minuto);

  // ==================================================================
  // 14.5 MÁQUINA DE ESTADOS (menus + botões)
  // ==================================================================
  if (estadoAtual == "RELOGIO") {
    // Clima usando o intervalo configurado na interface web
    static unsigned long tempoClima = 0;
    if (millis() - tempoClima > cfgClimaIntervaloMs && wifiConectado) {
      pegarClima();
      tempoClima = millis();
      precisaRedesenhar = true;
    }

    // Verifica timeout da mensagem de clima (10 segundos sem delay)
    if (mostrarMsgClima && (millis() - inicioMsgClima >= DURACAO_MSG_CLIMA)) {
      mostrarMsgClima = false;
    }

    // Atualiza LCD continuamente (a cada segundo a hora muda)
    static unsigned long ultimaAtualizacaoLCD = 0;
    static int segundoAnterior = -1;
    if (lcdOkBoot && agora.second() != segundoAnterior) {
      atualizarLCD(agora, wifiConectado);
      segundoAnterior = agora.second();
    }

    if (clicou1) {
      if (alarmeDisparo) {
        // Silencia o alarme
        silenciarAlarmeAtual();
      } else {
        estadoAtual = "MENU";
        menuIndex = 0;
      }
    }

    // Render Relógio
    uint64_t tD0 = esp_timer_get_time();
    if (precisaRedesenhar && estadoAtual == "RELOGIO") {
      displayTela.clearDisplay();

      // ================================================================
      // FASE 1.3: REDESIGN DO OLED - EXIBIÇÃO APENAS DO ÍCONE E HORA
      // ================================================================
      // Hora pequena no topo
      displayTela.setTextSize(1);
      displayTela.setCursor(0, 0);
      char horaOled[6];
      snprintf(horaOled, sizeof(horaOled), "%02d:%02d", hora, minuto);
      displayTela.print(horaOled);

      // Obtém o tipo de clima atual (0-4)
      int tipoClima = getTipoClima(temperatura, climaAtual.weather_code);
      
      switch(tipoClima) {
        case 0:
          desenharNeve();
          break;
        case 1:
          desenharNuvemDeChuva();
          break;
        case 2:
          desenharNuvem();
          break;
        case 3:
          desenharSol();
          break;
        case 4:
          desenharTermometro();
          break;
        default:
          desenharNuvem();
          break;
      }

      // Indicador de alarme (canto inferior)
      if (alarmeDisparo) {
        displayTela.setCursor(15, 55);
        displayTela.setTextSize(1);
        displayTela.print(">>> ALARME ");
        displayTela.print(alarmeDisparoIdx + 1);
        displayTela.print(" <<<");
      }

      displayTela.display();
    }
    tempoAtualizarDisplayUs = (uint32_t)(esp_timer_get_time() - tD0);

  // ---- MENU PRINCIPAL ----
  } else if (estadoAtual == "MENU") {
    if (clicou1) { menuIndex++; if (menuIndex >= 3) menuIndex = 0; }
     if (clicou2) {
       if (menuIndex == 0)      { estadoAtual = "LISTA_ALARMES"; alarmeListaIdx = 0; }
       else if (menuIndex == 1) { estadoAtual = "HORA"; tempHora = hora; tempMinuto = minuto; }
       else                     { estadoAtual = "RELOGIO"; }
     }

    // ---- MENU PRINCIPAL (continua no OLED) ----
    if (precisaRedesenhar && estadoAtual == "MENU") {
      displayTela.clearDisplay();
      displayTela.setTextSize(2);
      displayTela.setCursor(40, 0);
      displayTela.print("MENU");
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 25);
      displayTela.print(menuIndex == 0 ? "> ALARMES" : "  Alarmes");
      displayTela.setCursor(10, 40);
      displayTela.print(menuIndex == 1 ? "> HORA" : "  Hora");
      displayTela.setCursor(10, 55);
      displayTela.print(menuIndex == 2 ? "> SAIR" : "  Sair");
      displayTela.display();
    }

  // ---- LISTA DE 3 ALARMES + SAIR ----
  } else if (estadoAtual == "LISTA_ALARMES") {
    if (clicou1) { alarmeListaIdx++; if (alarmeListaIdx >= NUM_ALARMES + 1) alarmeListaIdx = 0; }
    if (clicou2) {
      if (alarmeListaIdx < NUM_ALARMES) {
        estadoAtual = "DETALHE_ALARME"; subMenuIndex = 0;
      } else {
        estadoAtual = "MENU"; // Sair -> volta ao menu
      }
    }

    if (precisaRedesenhar && estadoAtual == "LISTA_ALARMES") {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(30, 0);
      displayTela.print("ALARMES");
      for (int i = 0; i < NUM_ALARMES; i++) {
        displayTela.setCursor(2, 18 + i * 12);
        char linha[25];
        snprintf(linha, sizeof(linha), "%s%d: %02d:%02d %s %s",
          i == alarmeListaIdx ? ">" : " ",
          i + 1,
          alarmes[i].hora, alarmes[i].minuto,
          alarmes[i].ativo ? "ON " : "OFF",
          MELODIA_CURTO[alarmes[i].melodia]);
        displayTela.print(linha);
      }
      // Opção Sair
      displayTela.setCursor(2, 18 + NUM_ALARMES * 12);
      displayTela.print(alarmeListaIdx == NUM_ALARMES ? "> SAIR" : "  Sair");
      displayTela.display();
    }

  // ---- DETALHE DE UM ALARME ----
  } else if (estadoAtual == "DETALHE_ALARME") {
    if (clicou1) { subMenuIndex++; if (subMenuIndex >= 4) subMenuIndex = 0; }
    if (clicou2) {
      if (subMenuIndex == 0) {
        // Toggle ativo
        alarmes[alarmeListaIdx].ativo = !alarmes[alarmeListaIdx].ativo;
      } else if (subMenuIndex == 1) {
        // Editar hora
        estadoAtual = "EDITAR_ALARME_H";
        tempHora = alarmes[alarmeListaIdx].hora;
        tempMinuto = alarmes[alarmeListaIdx].minuto;
      } else if (subMenuIndex == 2) {
        // Escolher melodia
        estadoAtual = "ESCOLHER_MELODIA";
        melodiaEscolha = alarmes[alarmeListaIdx].melodia;
      } else {
        // Voltar
        estadoAtual = "LISTA_ALARMES";
      }
    }

    if (precisaRedesenhar && estadoAtual == "DETALHE_ALARME") {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      char titulo[15];
      snprintf(titulo, sizeof(titulo), "ALARME %d", alarmeListaIdx + 1);
      displayTela.setCursor(30, 0);
      displayTela.print(titulo);

      displayTela.setCursor(5, 18);
      char statusTxt[20];
      snprintf(statusTxt, sizeof(statusTxt), "%sStatus: %s",
        subMenuIndex == 0 ? "> " : "  ",
        alarmes[alarmeListaIdx].ativo ? "ON" : "OFF");
      displayTela.print(statusTxt);

      displayTela.setCursor(5, 30);
      displayTela.print(subMenuIndex == 1 ? "> Editar hora" : "  Editar hora");

      displayTela.setCursor(5, 42);
      char melTxt[25];
      snprintf(melTxt, sizeof(melTxt), "%sMelodia: %s",
        subMenuIndex == 2 ? "> " : "  ",
        MELODIA_NOMES[alarmes[alarmeListaIdx].melodia]);
      displayTela.print(melTxt);

      displayTela.setCursor(5, 54);
      displayTela.print(subMenuIndex == 3 ? "> Voltar" : "  Voltar");

      displayTela.display();
    }

  // ---- ESCOLHER MELODIA ----
  } else if (estadoAtual == "ESCOLHER_MELODIA") {
    if (clicou1) {
      melodiaEscolha++;
      if (melodiaEscolha >= TOTAL_MELODIAS) melodiaEscolha = 0;
    }
    if (clicou2) {
      alarmes[alarmeListaIdx].melodia = melodiaEscolha;
      estadoAtual = "DETALHE_ALARME";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(20, 0);
      displayTela.print("MELODIA");

      for (int i = 0; i < TOTAL_MELODIAS; i++) {
        displayTela.setCursor(5, 16 + i * 10);
        displayTela.print(i == melodiaEscolha ? "> " : "  ");
        displayTela.print(MELODIA_NOMES[i]);
      }
      displayTela.display();
    }
 
  // ---- EDITAR HORA DO ALARME ----
  } else if (estadoAtual == "EDITAR_ALARME_H") {
    if (clicou1) { tempHora++; if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_ALARME_M"; }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("AJUSTAR HORAS");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(20, 50, 55, 50, SSD1306_WHITE);
      displayTela.display();
    }

  // ---- EDITAR MINUTO DO ALARME ----
  } else if (estadoAtual == "EDITAR_ALARME_M") {
    if (clicou1) { tempMinuto++; if (tempMinuto > 59) tempMinuto = 0; }
    if (clicou2) {
      alarmes[alarmeListaIdx].hora = tempHora;
      alarmes[alarmeListaIdx].minuto = tempMinuto;
      logPrintf("[ALARME] %d salvo: %02d:%02d\n", alarmeListaIdx+1, tempHora, tempMinuto);
      estadoAtual = "RELOGIO";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("AJUSTAR MINUTOS");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(75, 50, 110, 50, SSD1306_WHITE);
      displayTela.display();
    }

  // ---- AJUSTAR HORA DO SISTEMA (RTC) ----
  } else if (estadoAtual == "HORA") {
    if (clicou1) { tempHora++; if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_HORA_M"; }

    
    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("HORA SISTEMA");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(20, 50, 55, 50, SSD1306_WHITE);
      displayTela.display();
    }

  } else if (estadoAtual == "EDITAR_HORA_M") {
    if (clicou1) { tempMinuto++; if (tempMinuto > 59) tempMinuto = 0; }
    if (clicou2) {
      ajustarHoraSistema(tempHora, tempMinuto);
      estadoAtual = "RELOGIO";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("MINUTO SISTEMA");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(75, 50, 110, 50, SSD1306_WHITE);
      displayTela.display();
    }
  }

  // [TAG_PERF_METRICS]
  // ==================================================================
  // 14.6 MÉTRICAS + BACKUP FLASH (FASE 2.4: persistência de 24h)
  // ==================================================================
  tempoLoopUs = (uint32_t)(esp_timer_get_time() - tLoop0);
  cpuUsoEstimado = (tempoLoopUs / 50000.0f) * 100.0f;
  if (cpuUsoEstimado > 100.0f) cpuUsoEstimado = 100.0f;
  stackLoopWords = uxTaskGetStackHighWaterMark(nullptr);

  // Amostragem periodica de performance (1 ponto a cada 1 segundo)
  // FASE 2.4: Ajustado para 1s para janela de ~5 minutos no grafico
  // 288 samples × 1s = 288s = ~4.8 minutos
  if (millis() - ultimaAmostraPerf >= PERF_SAMPLE_MS) {
    perfHist[0][perfHistPos] = tempoLoopUs;
    perfHist[1][perfHistPos] = tempoPegarClimaUs;
    perfHist[2][perfHistPos] = tempoRtcNowUs;
    perfHist[3][perfHistPos] = tempoLeituraBotoesUs;
    perfHist[4][perfHistPos] = tempoAtualizarDisplayUs;
    perfHist[5][perfHistPos] = tempoPegarDHTUS;
    perfHist[6][perfHistPos] = tempoLogicaAlarmeUs;
    
    perfHistPos = (perfHistPos + 1) % PERF_HIST_SIZE;
    if (perfHistPos == 0) perfHistFull = true;
    ultimaAmostraPerf = millis();
    
    logInfo("[PERF] Sample #%d: Loop=%lus API=%lus RTC=%lus DHT_Lat=%lus\n",
      perfHistPos - 1, tempoLoopUs, tempoPegarClimaUs, tempoRtcNowUs, tempoPegarDHTUS);
  }

  // ==================================================================
  // 14.7 LÓGICA DE INATIVIDADE (LIGHT SLEEP)
  // ==================================================================
  static int last_countdown_val = -1;
  unsigned long tempoOcioso = millis() - last_activity_ms;

  // Countdown: 5min (300s) idle target
  // Inicia aviso nos últimos 50 segundos de inatividade
  if (tempoOcioso >= 250000 && tempoOcioso < 300000) {
    int current_countdown = 5 - ((tempoOcioso - 250000) / 10000);
    if (current_countdown != last_countdown_val && current_countdown >= 1) {
      logInfo("[IDLE] Inatividade prolongada. Repouso em: %d...\n", current_countdown);
      Serial.printf("[IDLE] Inatividade prolongada. Repouso em: %d...\n", current_countdown);
      last_countdown_val = current_countdown;
    }
  } 
  else if (tempoOcioso >= 300000) {
    logInfo("[IDLE] Sistema ocioso por 5 min. Entrando em Light Sleep...\n");
    Serial.println("[IDLE] Entrando em Light Sleep...");
    
    // Sinaliza ANTES para que o Web Server possa mostrar no log se houver polling
    sleep_mode_active = true; 
    
    last_countdown_val = -1;
    last_activity_ms = millis(); // Reset para evitar loop de sleep ao acordar
    
    // Entra em repouso por 60 segundos (ou até botão ser pressionado)
    enterLightSleep(60000);
  } 
  else if (tempoOcioso < 250000 && last_countdown_val != -1) {
    // Caso tenha havido atividade durante o countdown
    logInfo("[IDLE] Atividade detectada! Cancelado repouso.\n");
    Serial.println("[IDLE] Atividade detectada! Cancelado repouso.");
    last_countdown_val = -1;
  }

  // Backup na flash a cada 2 horas (com validação CRC)
  if (millis() - ultimoBackupFlash >= cfgBackupIntervaloMs) {
    salvarLogFlash();
    salvarPerfFlash();  // Agora inclui CRC
    ultimoBackupFlash = millis();
    logInfo("[BACKUP] Flash salvo com CRC - Boot #%d\n", rtc_data.boot_count);
  }
}