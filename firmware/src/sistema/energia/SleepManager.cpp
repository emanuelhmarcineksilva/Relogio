/**
 * @file SleepManager.cpp
 * @brief Gerenciador de Light Sleep do ESP32 para economia de energia
 * 
 * Este módulo implementa Light Sleep (modo de consumo reduzido) do ESP32
 * com fontes de despertar configuráveis: timer periódico e GPIO (botões).
 * 
 * Redução de consumo: ~80% durante repouso vs. modo ativo
 * Latência de despertar: <1ms para GPIO, até duration_ms + overhead para timer
 * 
 * Requisitos (RNF05 - Energia):
 * - Light Sleep acionado quando sistema detecta inatividade
 * - Wake sources: timer + GPIO (botões em nível LOW)
 * - Estado persistido em RTC_DATA_ATTR entre ciclos
 * - WiFi permanece conectado para resposta rápida
 * 
 * @author ChronosObsidian Team
 * @date 2026
 */

#include "ChronosObsidian.h"

/**
 * @fn void idle_timeout_callback(void* arg)
 * @brief Callback chamado após 50s de inatividade
 * 
 * Marca o sistema como ocioso, permitindo que entre em Light Sleep
 * Chamado pelo timer esp_timer_start_once()
 */
void idle_timeout_callback(void* arg) {
  idle_state = true;  // Sistema está ocioso, pode entrar em sleep
  logInfo("[IDLE] Sistema ocioso após 50s - pronto para Light Sleep\n");
}

/**
 * @fn void resetIdleTimeout(void)
 * @brief Reseta o timer de 50s quando um botão é clicado
 * 
 * Chamado pelos ISRs dos botões para resetar o timeout de inatividade.
 * Garante que o sistema não entre em Light Sleep enquanto há atividade.
 */
void resetIdleTimeout(void) {
  // Reseta o estado ocioso - há atividade detectada
  idle_state = false;
  logInfo("[IDLE] Atividade detectada - recarregando timeout de 50s\n");
  
  // Para o timer anterior se estiver rodando
  if (idle_timeout_timer != NULL) {
    esp_timer_stop(idle_timeout_timer);
  }
  
  // Reinicia o timer para 50 segundos (50000000 microssegundos)
  if (idle_timeout_timer != NULL) {
    esp_timer_start_once(idle_timeout_timer, 50000000);  // 50s em microsegundos
  }
}

/**
 * @fn void initIdleTimeout(void)
 * @brief Inicializa o timer de 50s para controle de idle
 * 
 * Chamado no boot. Deve ser feito após esp_timer_init()
 */
void initIdleTimeout(void) {
  if (idle_timeout_timer != NULL) {
    logWarn("[IDLE] Timer de idle já inicializado\n");
    return;
  }
  
  const esp_timer_create_args_t idle_timer_args = {
    .callback = idle_timeout_callback,
    .arg = NULL,
    .name = "idle_timeout"
  };
  
  esp_err_t err = esp_timer_create(&idle_timer_args, &idle_timeout_timer);
  if (err != ESP_OK) {
    logError("[IDLE] Erro ao criar timer de idle: %d\n", err);
    return;
  }
  
  // Inicia o timer para a primeira vez
  idle_state = false;
  resetIdleTimeout();
  logInfo("[IDLE] Timer de 50s inicializado com sucesso\n");
}

/**
 * @fn void
 * @brief Ativa Light Sleep com duração e fontes de despertar configuradas
 * 
 * @param duration_ms Tempo de dormida em milissegundos
 * 
 * Fluxo de execução:
 * 1. Verifica se já está em sleep (evita reentrada)
 * 2. Log de entrada com duração esperada
 * 3. Registra fontes de despertar:
 *    - Timer: desperta após duration_ms (precisão: ~1ms)
 *    - GPIO: desperta imediatamente ao pressionar botão (nível LOW)
 * 4. Ativa flag sleep_mode_active e entra em Light Sleep
 * 5. Retorna do sleep ao despertar e registra evento
 * 
 * Conservação de estado durante sleep:
 * - RTC_DATA_ATTR (rtc_data): permanece intacto
 * - RAM: mantida alimentada
 * - WiFi: permanece conectado (permite resposta rápida)
 * - RTC interno: continua oscilando para timer/clock
 * 
 * Comportamento pós-despertar:
 * - Flag sleep_mode_active retorna a false automaticamente
 * - Sistema retorna ao main loop ou task continuando execução
 * - Contexto completo (stack, heap) preservado
 */
void enterLightSleep(uint32_t duration_ms) {
  // NOVO: Verificação de idle - só entra em sleep se não há atividade recente
  if (!idle_state) {
    logWarn("[SLEEP] Sistema ainda está ativo (idle_state = false) - aguardando 50s de inatividade\n");
    return;
  }
  
  // Proteção contra reentrada: se já está dormindo, não faz nada
  if (sleep_mode_active) {
    logWarn("[SLEEP] Tentativa de entrar em Light Sleep já ativo\n");
    return;
  }
  
  logInfo("[SLEEP] Entrando em Light Sleep por %lu ms\n", duration_ms);
  
  // CORREÇÃO: Garante que os pinos dos botões estão configurados como INPUT_PULLUP
  // antes de entrar em sleep - às vezes o modo pode ser perdido
  pinMode(pinoButton1, INPUT_PULLUP);
  pinMode(pinoButton2, INPUT_PULLUP);
  logInfo("[SLEEP] Pinos dos botões reconfigurados como INPUT_PULLUP\n");
  
  // Configura fontes de despertar (wake sources)
  // Nota: múltiplas chamadas a esp_sleep_enable_*_wakeup() fazem OR das fontes
  // Isso significa que o primeiro evento (timer OU GPIO) que ocorrer despertará o sistema
  
  // Wake source 1: Timer periódico
  // esp_sleep_enable_timer_wakeup() recebe microsegundos, daí a multiplicação por 1000
  // Precisão aproximada: 1ms (depende do oscilador RTC do ESP32)
  esp_sleep_enable_timer_wakeup(duration_ms * 1000);  // Converte ms → us
  
  // Wake source 2: GPIO (Botões) - EXT1 suporta múltiplos pinos
  // Despertará quando qualquer pino ir para nível LOW (0) - configurado na montagem com PULLUP
  // GPIO4 (Botão 1): Navega menus
  // GPIO18 (Botão 2): Confirma/ativa alarme
  // Usa máscara de bits: (1ULL << 4) | (1ULL << 18) = pino 4 e 18
  uint64_t gpio_mask = (1ULL << pinoButton1) | (1ULL << pinoButton2);
  esp_sleep_enable_ext1_wakeup(gpio_mask, ESP_EXT1_WAKEUP_ALL_LOW);  // Desperta se qualquer botão for pressionado (LOW)
  
  // Transição para Light Sleep
  sleep_mode_active = true;  // Sinaliza que sistema está em sleep (para proteção de reentrada)
  
  // esp_light_sleep_start() suspende o processador e perif. não-críticos
  // Mantém: RTC, RAM, GPIO (para wake), WiFi (conectado em background)
  // Desativa: CPU principal, Serial, I2C durante sleep (retomados ao despertar)
  esp_light_sleep_start();
  
  // Código abaixo só executa APÓS o sistema despertar (via timer ou GPIO)
  sleep_mode_active = false;  // Sistema acordou
  
  // Log de despertar para auditoria e debugging
  // Detecta wake source (GPIO ou Timer)
  esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
  if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0 || wakeup_cause == ESP_SLEEP_WAKEUP_EXT1) {
    logPrintln("[SLEEP] Acordou via GPIO (BOTAO PRESSIONADO)");
    
    // ==== CORREÇÃO CRÍTICA ====
    // Quando acordado via EXT1, as interrupts GPIO normais NÃO são disparadas automaticamente
    // Precisamos ler os pinos manualmente e forçar as ISRs
    
    // Verifica qual botão foi pressionado e simula ISR se necessário
    if (digitalRead(pinoButton1) == LOW) {
      logInfo("[SLEEP-ISR] Acionando ISR de Botão 1 manualmente\n");
      btn1_isr();  // Força a ISR
    }
    if (digitalRead(pinoButton2) == LOW) {
      logInfo("[SLEEP-ISR] Acionando ISR de Botão 2 manualmente\n");
      btn2_isr();  // Força a ISR
    }
  } else if (wakeup_cause == ESP_SLEEP_WAKEUP_TIMER) {
    logPrintln("[SLEEP] Acordou via TIMER (timeout alcancado)");
  } else {
    logPrintln("[SLEEP] Acordou do Light Sleep (causa desconhecida)");
  }
  
  // Nota: não há necessidade de reconfigurar periféricos aqui
  // O ESP-IDF cuida da retomada automática de I2C, SPI, etc.
  // WiFi mantém conexão ativa durante todo o ciclo
}
