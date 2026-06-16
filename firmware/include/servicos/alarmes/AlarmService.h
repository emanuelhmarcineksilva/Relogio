#pragma once
#include "ChronosObsidian.h"

/**
 * @brief Inicializa o sistema de alarmes.
 */
void inicializarAlarmes();

/**
 * @brief Verifica se algum alarme deve disparar no momento atual.
 * 
 * @param hora Hora atual.
 * @param minuto Minuto atual.
 */
void processarAlarmes(int hora, int minuto);

/**
 * @brief Salva as configurações de todos os alarmes no SPIFFS.
 */
void salvarAlarmes();

/**
 * @brief Carrega as configurações de todos os alarmes do SPIFFS.
 */
void carregarAlarmes();

/**
 * @brief Silencia o alarme que está tocando no momento.
 */
void silenciarAlarmeAtual();

/**
 * @brief Atualiza as configurações de um alarme específico.
 * 
 * @param idx Índice do alarme (0 a NUM_ALARMES-1).
 * @param h Nova hora.
 * @param m Novo minuto.
 * @param ativo Se o alarme está ativado.
 * @param melodia Índice da melodia.
 */
void configurarAlarme(int idx, uint8_t h, uint8_t m, bool ativo, uint8_t melodia);
