#pragma once
#include "ChronosObsidian.h"

/**
 * @brief Inicializa o sensor DHT22.
 */
void iniciarDHT();

/**
 * @brief Realiza a leitura dos dados do DHT22.
 * 
 * @param temp Referência para armazenar a temperatura.
 * @param humi Referência para armazenar a umidade.
 * @return true Se a leitura for bem-sucedida.
 * @return false Se os dados forem inválidos (NAN).
 */
bool lerDadosDHT(float &temp, float &humi);
