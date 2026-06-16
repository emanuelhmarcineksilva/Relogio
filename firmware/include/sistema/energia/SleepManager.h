/**
 * @file SleepManager.h
 * @brief Protótipos e definições para Light Sleep do ESP32
 * 
 * Módulo responsável pelo gerenciamento de energia via Light Sleep.
 * Implementa economia de ~80% com resposta sub-ms a eventos.
 */

#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <esp_sleep.h>
#include <esp_timer.h>

/**
 * @brief Entra em Light Sleep com duração configurada
 * 
 * Ativa Light Sleep com fontes de despertar:
 * - Timer: despertar periódico após duration_ms
 * - GPIO: despertar imediato ao pressionar botão
 * 
 * Estado persistido em RTC_DATA_ATTR durante sleep.
 * WiFi permanece conectado para resposta rápida.
 * 
 * @param duration_ms Duração do sleep em milissegundos
 * 
 * @note Chamada segura contra reentrada (verifica sleep_mode_active)
 * @note Latência de retorno: <1ms (GPIO) ou ~duration_ms (timer)
 * 
 * @warning Não chamar de dentro de ISR (Interrupt Service Routine)
 * 
 * Exemplo de uso:
 * @code
 * if (tempo_sem_atividade > 60000) {  // 60 segundos sem clique
 *   enterLightSleep(300000);           // Sleep por 5 minutos
 * }
 * @endcode
 */
void enterLightSleep(uint32_t duration_ms);

/**
 * @brief Inicializa o timer de 50s para controle de idle/inatividade
 * 
 * Deve ser chamado uma vez no boot (em BootSequence BOOT_BOTOES).
 * Cria o timer esp_timer para contar 50s sem atividade de botão.
 * 
 * @note Chamado automaticamente em bootStep()
 */
void initIdleTimeout(void);

/**
 * @brief Reseta o timer de 50s e marca sistema como ativo
 * 
 * Chamado sempre que um botão é clicado (em btn_debounce_callback).
 * Garante que o sistema não entre em Light Sleep enquanto há atividade.
 * 
 * - Define idle_state = false (sistema ativo)
 * - Para e reinicia o timer de 50s
 * 
 * @note Seguro chamar de callbacks ISR/timer (usa apenas esp_timer_*)
 */
void resetIdleTimeout(void);

#ifdef __cplusplus
}
#endif

#endif // SLEEP_MANAGER_H
