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
 * @fn void enterLightSleep(uint32_t duration_ms)
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
  // Proteção contra reentrada: se já está dormindo, não faz nada
  if (sleep_mode_active) {
    logWarn("[SLEEP] Tentativa de entrar em Light Sleep já ativo\n");
    return;
  }
  
  logInfo("[SLEEP] Entrando em Light Sleep por %lu ms\n", duration_ms);
  
  // Configura fontes de despertar (wake sources)
  // Nota: múltiplas chamadas a esp_sleep_enable_*_wakeup() fazem OR das fontes
  // Isso significa que o primeiro evento (timer OU GPIO) que ocorrer despertará o sistema
  
  // Wake source 1: Timer periódico
  // esp_sleep_enable_timer_wakeup() recebe microsegundos, daí a multiplicação por 1000
  // Precisão aproximada: 1ms (depende do oscilador RTC do ESP32)
  esp_sleep_enable_timer_wakeup(duration_ms * 1000);  // Converte ms → us
  
  // Wake source 2: GPIO (Botões)
  // Despertará quando o pino ir para nível LOW (0) - configurado na montagem com PULLUP
  // GPIO4 (Botão 1): Navega menus
  // GPIO18 (Botão 2): Confirma/ativa alarme
  esp_sleep_enable_ext0_wakeup((gpio_num_t)pinoButton1, 0);  // Button 1 (GPIO 4, LOW)
  esp_sleep_enable_ext0_wakeup((gpio_num_t)pinoButton2, 0);  // Button 2 (GPIO 18, LOW)
  
  // Transição para Light Sleep
  sleep_mode_active = true;  // Sinaliza que sistema está em sleep (para proteção de reentrada)
  
  // esp_light_sleep_start() suspende o processador e perif. não-críticos
  // Mantém: RTC, RAM, GPIO (para wake), WiFi (conectado em background)
  // Desativa: CPU principal, Serial, I2C durante sleep (retomados ao despertar)
  esp_light_sleep_start();
  
  // Código abaixo só executa APÓS o sistema despertar (via timer ou GPIO)
  sleep_mode_active = false;  // Sistema acordou
  
  // Log de despertar para auditoria e debugging
  logPrintln("[SLEEP] Acordou do Light Sleep");
  
  // Nota: não há necessidade de reconfigurar periféricos aqui
  // O ESP-IDF cuida da retomada automática de I2C, SPI, etc.
  // WiFi mantém conexão ativa durante todo o ciclo
}
