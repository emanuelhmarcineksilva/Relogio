#include "ChronosObsidian.h"

static void lcd_rotation_callback(void* arg) {
  lcd_line2_show_ip = !lcd_line2_show_ip;
}

void iniciarLCD() {
  lcd.begin(16, 2);  // Inicializa o LCD 16x2
  lcd.backlight();
  lcdOkBoot = true;
  logPrintln("LCD OK (16x2 I2C)");
  
  // FASE 1.4: Inicializa timer de rotação do LCD (5s não bloqueante)
  // Cria um esp_timer periódico que alterna o conteúdo da linha 2 do LCD a cada 5 segundos
  esp_timer_create_args_t timer_args = {
    .callback = &lcd_rotation_callback,
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK,      // Chama no contexto de tarefa (seguro, não bloqueante)
    .name = "lcd_rotation"
  };
  
  if (esp_timer_create(&timer_args, &lcd_rotation_timer) == ESP_OK) {
    // Inicia o timer com período de 5000ms (repetindo)
    if (esp_timer_start_periodic(lcd_rotation_timer, 5000000) == ESP_OK) {  // 5s em microssegundos
      logPrintln("[LCD] Timer de rotacao de 5s iniciado");
      lcd_line2_show_ip = true;  // Começa com exibição de IP/AP
    } else {
      logError("[LCD] Falha ao iniciar timer de rotacao\n");
    }
  } else {
    logError("[LCD] Falha ao criar timer de rotacao\n");
  }
}

void atualizarLCD(DateTime agora, bool wifiConectado) {
  if (!lcdOkBoot) return;
  
  // ============================================================
  // LINHA 1 (FIXA): HH:MM | umidade% | temp°C
  // ============================================================
  // Formato: "HH:MM | UU% | TT°C" (exatamente 16 caracteres quando formatado corretamente)
  // Exemplo: "14:23 | 65% | 22°C" - cabe no display 16x2
  lcd.setCursor(0, 0);
  char linha1[17];
  
  if (isnan(dhtTemperature) || isnan(dhtHumidity)) {
    // Fallback se o DHT ainda não estiver inicializado
    snprintf(linha1, sizeof(linha1), "%02d:%02d |  -- | --c", 
      agora.hour(), agora.minute());
  } else {
    // Formato completo com temperatura e umidade
    snprintf(linha1, sizeof(linha1), "%02d:%02d  u%2.0f%% t%2.0fc", 
      agora.hour(), agora.minute(), dhtHumidity, dhtTemperature);
  }
  
  // Garante exatamente 16 caracteres (preenche com espaços se necessário)
  for (int i = strlen(linha1); i < 16; i++) {
    linha1[i] = ' ';
  }
  linha1[16] = '\0';
  lcd.print(linha1);
  
  // ============================================================
  // LINHA 2 (ROTATIVA): IP/AP ou Tmin/Tmax (alternância de 5s via esp_timer)
  // ============================================================
  // FASE 1.4: Alterna a cada 5 segundos SEM delay() bloqueante
  // Usa o callback esp_timer (lcd_rotation_callback) para inverter lcd_line2_show_ip
  lcd.setCursor(0, 1);
  char linha2[17];
  
  if (lcd_line2_show_ip) {
    // Modo 1: Mostrar endereço WiFi (IP ou AP)
    if (wifiConectado) {
      String ip = WiFi.localIP().toString();
      // Trunca para caber 16 caracteres: "IP: XXX.XXX.X.XX"
      snprintf(linha2, sizeof(linha2), "w %s", ip.c_str());
    } else {
      // Sem WiFi: mostra endereço de fallback AP
      snprintf(linha2, sizeof(linha2), "AP: 192.168.4.1");
    }
  } else {
    // Modo 2: Mostrar temperatura min/max da API
    // Formato: "Tmin:XX Tmax:XX" (14 caracteres, cabe em 16)
    if (climaAtual.valid) {
      snprintf(linha2, sizeof(linha2), "Tmin:%2.0f Tmax:%2.0f",
        climaAtual.temp_min, climaAtual.temp_max);
    } else {
      snprintf(linha2, sizeof(linha2), "Tmin: -- Tmax: --");
    }
  }
  
  // Trunca para no máximo 16 caracteres (largura do LCD)
  if (strlen(linha2) > 16) {
    linha2[16] = '\0';
  }
  
  // Preenche com espaços para completar as colunas restantes
  for (int i = strlen(linha2); i < 16; i++) {
    linha2[i] = ' ';
  }
  linha2[16] = '\0';
  lcd.print(linha2);
}
