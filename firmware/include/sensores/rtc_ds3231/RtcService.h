#pragma once
#include "ChronosObsidian.h"

/**
 * @brief Inicializa o módulo RTC DS3231.
 * 
 * @return true Se o RTC foi iniciado com sucesso.
 * @return false Se o RTC não foi encontrado ou falhou.
 */
bool iniciarRTC();

/**
 * @brief Obtém a data e hora atual do RTC.
 * 
 * @return DateTime Objeto com a data e hora atuais.
 */
DateTime obterHoraAtual();

/**
 * @brief Ajusta a hora do sistema no RTC.
 * 
 * @param h Hora (0-23)
 * @param m Minuto (0-59)
 */
void ajustarHoraSistema(int h, int m);

/**
 * @brief Sincroniza o RTC com a hora compilada ou NTP.
 * 
 * @param ti Estrutura tm com a hora vinda do NTP.
 */
void sincronizarRTC(struct tm ti);
