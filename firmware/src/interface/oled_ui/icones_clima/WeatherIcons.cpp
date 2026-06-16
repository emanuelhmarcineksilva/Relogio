#include "ChronosObsidian.h"




// Ícones serão implementados abaixo com os novos designs.

// ============================================================================
// FASE 1.2: MAPEAMENTO DE CÓDIGO DE TEMPO + FUNÇÕES DE DESENHO DE ÍCONES
// ============================================================================
// Códigos de tempo Meteosource (veja ApiParPrevisaoDiaria.txt linhas 58-265):
//   1-5: chuva/chuva leve / possível chuva
//   6-10: variantes de nuvens
//   11-13: variantes ensolaradas (parcialmente ensolarado, céu limpo, etc.)
//   etc. (tabela completa na documentação)
//
// Nosso mapeamento simplificado (5 ícones):
//   Ícone NEVE: T < 15°C
//   Ícone GUARDA-CHUVA: weather_code EM {chuva, chuva_leve, poss_chuva}
//   Ícone NUVEM: weather_code EM {nublado, muito_nublado}
//   Ícone SOL: weather_code EM {parcialmente_ensolarado, ceu_limpo}
//   Ícone TERMÔMETRO: T >= 25°C (quente)
//
// Função: Mapeia código de tempo Meteosource → tipo de clima (0-4)
// Retorna:
//   0 = NEVE (frio, T < 15)
//   1 = GUARDA-CHUVA (chuvoso)
//   2 = NUVEM (nublado)
//   3 = SOL (limpo/parcialmente ensolarado)
//   4 = TERMÔMETRO (quente, T >= 25)
int getTipoClima(float temp, int weather_code) {
  // Prioridade 1: Extremos de temperatura (Conforme solicitação do usuário)
  if (temp <= 16.0f) return 0;   // NEVE / FRIO (0)
  if (temp > 25.0f)  return 4;   // TERMÔMETRO / CALOR (4)
  
  // Prioridade 2: Códigos de Ícone Meteosource (v1/free)
  
  // SOL / LÍMPIDO / P. ENSOLARADO (Icon 1, 2, 3, 4)
  if (weather_code >= 1 && weather_code <= 4) return 3;
  
  // NUVE / NUBLADO / ENCOBERTO (Icon 5, 6, 7, 8)
  if (weather_code >= 5 && weather_code <= 8) return 2;
  
  // CHUVA / TEMPESTADE / GAROA (Icon 9, 10, 11, 12, 13, 14)
  if (weather_code >= 9 && weather_code <= 14) return 1;
  
  // NEVE (Icon 15, 16...)
  if (weather_code >= 15 && weather_code <= 20) return 0;

  // Padrão: trata códigos desconhecidos como nublado
  return 2;  // NUVEM (fallback)
}

// ============================================================================
// FUNÇÕES DE DESENHO DE ÍCONES (ESPAÇOS RESERVADOS) (FASE 1.2)
// ============================================================================
// Estes são apenas espaços reservados. O usuário substituirá por designs SVG/BMP reais depois.
// Cada função desenha um ícone simples na posição (x, y) no OLED 128x64.
// Os ícones têm ~16-20 pixels de largura para exibição no OLED.

// Ícone 1: NuvemDeChuva (proteção contra chuva) - espaço reservado
// Formato de NuvemDeChuva simples: arco em cima, cabo embaixo
void desenharNuvemDeChuva() {



// Draw shapes

  //id: 0 pixel 41 
  displayTela.drawPixel(10, 33, SSD1306_WHITE);
  displayTela.drawPixel(10, 32, SSD1306_WHITE);
  displayTela.drawPixel(10, 31, SSD1306_WHITE);
  displayTela.drawPixel(10, 30, SSD1306_WHITE);
  displayTela.drawPixel(10, 29, SSD1306_WHITE);
  displayTela.drawPixel(10, 28, SSD1306_WHITE);
  displayTela.drawPixel(10, 27, SSD1306_WHITE);
  displayTela.drawPixel(10, 26, SSD1306_WHITE);
  displayTela.drawPixel(11, 25, SSD1306_WHITE);
  displayTela.drawPixel(11, 24, SSD1306_WHITE);
  displayTela.drawPixel(12, 24, SSD1306_WHITE);
  displayTela.drawPixel(12, 23, SSD1306_WHITE);
  displayTela.drawPixel(12, 22, SSD1306_WHITE);
  displayTela.drawPixel(13, 22, SSD1306_WHITE);
  displayTela.drawPixel(13, 21, SSD1306_WHITE);
  displayTela.drawPixel(13, 20, SSD1306_WHITE);
  displayTela.drawPixel(14, 20, SSD1306_WHITE);
  displayTela.drawPixel(14, 19, SSD1306_WHITE);
  displayTela.drawPixel(15, 18, SSD1306_WHITE);
  displayTela.drawPixel(15, 17, SSD1306_WHITE);
  displayTela.drawPixel(16, 17, SSD1306_WHITE);
  displayTela.drawPixel(16, 16, SSD1306_WHITE);
  displayTela.drawPixel(17, 16, SSD1306_WHITE);
  displayTela.drawPixel(17, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 14, SSD1306_WHITE);
  displayTela.drawPixel(18, 13, SSD1306_WHITE);
  displayTela.drawPixel(19, 13, SSD1306_WHITE);
  displayTela.drawPixel(19, 12, SSD1306_WHITE);
  displayTela.drawPixel(20, 12, SSD1306_WHITE);
  displayTela.drawPixel(20, 11, SSD1306_WHITE);
  displayTela.drawPixel(21, 11, SSD1306_WHITE);
  displayTela.drawPixel(21, 10, SSD1306_WHITE);
  displayTela.drawPixel(22, 10, SSD1306_WHITE);
  displayTela.drawPixel(23, 10, SSD1306_WHITE);
  displayTela.drawPixel(24, 10, SSD1306_WHITE);
  displayTela.drawPixel(24, 9, SSD1306_WHITE);
  displayTela.drawPixel(25, 9, SSD1306_WHITE);
  displayTela.drawPixel(26, 9, SSD1306_WHITE);
  displayTela.drawPixel(26, 8, SSD1306_WHITE);
  displayTela.drawPixel(27, 8, SSD1306_WHITE);
  displayTela.drawPixel(28, 8, SSD1306_WHITE);
  displayTela.drawPixel(28, 7, SSD1306_WHITE);
  displayTela.drawPixel(29, 7, SSD1306_WHITE);
  displayTela.drawPixel(30, 7, SSD1306_WHITE);
  displayTela.drawPixel(31, 7, SSD1306_WHITE);
  displayTela.drawPixel(31, 6, SSD1306_WHITE);
  displayTela.drawPixel(32, 6, SSD1306_WHITE);
  displayTela.drawPixel(33, 6, SSD1306_WHITE);
  displayTela.drawPixel(34, 6, SSD1306_WHITE);
  displayTela.drawPixel(35, 6, SSD1306_WHITE);
  displayTela.drawPixel(36, 6, SSD1306_WHITE);
  displayTela.drawPixel(37, 5, SSD1306_WHITE);
  displayTela.drawPixel(38, 5, SSD1306_WHITE);
  displayTela.drawPixel(39, 5, SSD1306_WHITE);
  displayTela.drawPixel(40, 5, SSD1306_WHITE);
  displayTela.drawPixel(41, 5, SSD1306_WHITE);
  displayTela.drawPixel(42, 5, SSD1306_WHITE);
  displayTela.drawPixel(44, 5, SSD1306_WHITE);
  displayTela.drawPixel(45, 5, SSD1306_WHITE);
  displayTela.drawPixel(47, 5, SSD1306_WHITE);
  displayTela.drawPixel(48, 5, SSD1306_WHITE);
  displayTela.drawPixel(49, 5, SSD1306_WHITE);
  displayTela.drawPixel(50, 5, SSD1306_WHITE);
  displayTela.drawPixel(51, 5, SSD1306_WHITE);
  displayTela.drawPixel(51, 6, SSD1306_WHITE);
  displayTela.drawPixel(52, 6, SSD1306_WHITE);
  displayTela.drawPixel(52, 7, SSD1306_WHITE);
  displayTela.drawPixel(53, 7, SSD1306_WHITE);
  displayTela.drawPixel(53, 8, SSD1306_WHITE);
  displayTela.drawPixel(54, 8, SSD1306_WHITE);
  //id: 1 pixel 42 
  displayTela.drawPixel(9, 33, SSD1306_WHITE);
  displayTela.drawPixel(9, 32, SSD1306_WHITE);
  displayTela.drawPixel(9, 31, SSD1306_WHITE);
  displayTela.drawPixel(9, 30, SSD1306_WHITE);
  displayTela.drawPixel(10, 30, SSD1306_WHITE);
  displayTela.drawPixel(10, 29, SSD1306_WHITE);
  displayTela.drawPixel(10, 28, SSD1306_WHITE);
  displayTela.drawPixel(10, 27, SSD1306_WHITE);
  displayTela.drawPixel(11, 27, SSD1306_WHITE);
  displayTela.drawPixel(11, 26, SSD1306_WHITE);
  displayTela.drawPixel(11, 25, SSD1306_WHITE);
  displayTela.drawPixel(12, 25, SSD1306_WHITE);
  displayTela.drawPixel(12, 24, SSD1306_WHITE);
  displayTela.drawPixel(12, 23, SSD1306_WHITE);
  displayTela.drawPixel(13, 23, SSD1306_WHITE);
  displayTela.drawPixel(13, 22, SSD1306_WHITE);
  displayTela.drawPixel(13, 21, SSD1306_WHITE);
  displayTela.drawPixel(14, 21, SSD1306_WHITE);
  displayTela.drawPixel(14, 20, SSD1306_WHITE);
  displayTela.drawPixel(14, 19, SSD1306_WHITE);
  displayTela.drawPixel(15, 19, SSD1306_WHITE);
  displayTela.drawPixel(15, 18, SSD1306_WHITE);
  displayTela.drawPixel(16, 18, SSD1306_WHITE);
  displayTela.drawPixel(16, 17, SSD1306_WHITE);
  displayTela.drawPixel(17, 17, SSD1306_WHITE);
  displayTela.drawPixel(17, 16, SSD1306_WHITE);
  displayTela.drawPixel(17, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 14, SSD1306_WHITE);
  displayTela.drawPixel(19, 14, SSD1306_WHITE);
  displayTela.drawPixel(19, 13, SSD1306_WHITE);
  displayTela.drawPixel(19, 12, SSD1306_WHITE);
  displayTela.drawPixel(20, 12, SSD1306_WHITE);
  displayTela.drawPixel(20, 11, SSD1306_WHITE);
  displayTela.drawPixel(21, 11, SSD1306_WHITE);
  displayTela.drawPixel(21, 10, SSD1306_WHITE);
  displayTela.drawPixel(21, 9, SSD1306_WHITE);
  displayTela.drawPixel(22, 9, SSD1306_WHITE);
  displayTela.drawPixel(22, 8, SSD1306_WHITE);
  displayTela.drawPixel(23, 8, SSD1306_WHITE);
  displayTela.drawPixel(23, 9, SSD1306_WHITE);
  displayTela.drawPixel(23, 10, SSD1306_WHITE);
  displayTela.drawPixel(22, 10, SSD1306_WHITE);
  //id: 2 pixel 43 
  displayTela.drawPixel(21, 11, SSD1306_WHITE);
  displayTela.drawPixel(22, 11, SSD1306_WHITE);
  displayTela.drawPixel(23, 10, SSD1306_WHITE);
  displayTela.drawPixel(24, 10, SSD1306_WHITE);
  displayTela.drawPixel(25, 10, SSD1306_WHITE);
  displayTela.drawPixel(26, 10, SSD1306_WHITE);
  displayTela.drawPixel(26, 9, SSD1306_WHITE);
  displayTela.drawPixel(27, 9, SSD1306_WHITE);
  displayTela.drawPixel(28, 9, SSD1306_WHITE);
  displayTela.drawPixel(29, 9, SSD1306_WHITE);
  displayTela.drawPixel(30, 9, SSD1306_WHITE);
  displayTela.drawPixel(31, 9, SSD1306_WHITE);
  displayTela.drawPixel(32, 9, SSD1306_WHITE);
  displayTela.drawPixel(33, 9, SSD1306_WHITE);
  displayTela.drawPixel(34, 9, SSD1306_WHITE);
  displayTela.drawPixel(35, 9, SSD1306_WHITE);
  displayTela.drawPixel(36, 9, SSD1306_WHITE);
  displayTela.drawPixel(37, 9, SSD1306_WHITE);
  displayTela.drawPixel(38, 9, SSD1306_WHITE);
  displayTela.drawPixel(39, 9, SSD1306_WHITE);
  displayTela.drawPixel(40, 9, SSD1306_WHITE);
  displayTela.drawPixel(40, 10, SSD1306_WHITE);
  displayTela.drawPixel(41, 10, SSD1306_WHITE);
  displayTela.drawPixel(42, 10, SSD1306_WHITE);
  displayTela.drawPixel(43, 10, SSD1306_WHITE);
  displayTela.drawPixel(43, 11, SSD1306_WHITE);
  displayTela.drawPixel(44, 11, SSD1306_WHITE);
  displayTela.drawPixel(45, 11, SSD1306_WHITE);
  displayTela.drawPixel(46, 12, SSD1306_WHITE);
  displayTela.drawPixel(47, 12, SSD1306_WHITE);
  displayTela.drawPixel(47, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 14, SSD1306_WHITE);
  displayTela.drawPixel(48, 15, SSD1306_WHITE);
  displayTela.drawPixel(49, 15, SSD1306_WHITE);
  displayTela.drawPixel(49, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 17, SSD1306_WHITE);
  displayTela.drawPixel(51, 17, SSD1306_WHITE);
  displayTela.drawPixel(51, 18, SSD1306_WHITE);
  displayTela.drawPixel(52, 19, SSD1306_WHITE);
  displayTela.drawPixel(52, 20, SSD1306_WHITE);
  displayTela.drawPixel(53, 20, SSD1306_WHITE);
  displayTela.drawPixel(53, 21, SSD1306_WHITE);
  displayTela.drawPixel(54, 21, SSD1306_WHITE);
  displayTela.drawPixel(54, 22, SSD1306_WHITE);
  displayTela.drawPixel(54, 23, SSD1306_WHITE);
  displayTela.drawPixel(55, 23, SSD1306_WHITE);
  displayTela.drawPixel(55, 24, SSD1306_WHITE);
  displayTela.drawPixel(55, 25, SSD1306_WHITE);
  displayTela.drawPixel(55, 26, SSD1306_WHITE);
  displayTela.drawPixel(56, 26, SSD1306_WHITE);
  displayTela.drawPixel(56, 27, SSD1306_WHITE);
  displayTela.drawPixel(56, 28, SSD1306_WHITE);
  displayTela.drawPixel(57, 29, SSD1306_WHITE);
  displayTela.drawPixel(57, 30, SSD1306_WHITE);
  displayTela.drawPixel(57, 31, SSD1306_WHITE);
  displayTela.drawPixel(57, 32, SSD1306_WHITE);
  displayTela.drawPixel(58, 33, SSD1306_WHITE);
  displayTela.drawPixel(58, 34, SSD1306_WHITE);
  displayTela.drawPixel(57, 34, SSD1306_WHITE);
  displayTela.drawPixel(56, 34, SSD1306_WHITE);
  displayTela.drawPixel(55, 34, SSD1306_WHITE);
  displayTela.drawPixel(54, 34, SSD1306_WHITE);
  displayTela.drawPixel(53, 34, SSD1306_WHITE);
  displayTela.drawPixel(52, 34, SSD1306_WHITE);
  displayTela.drawPixel(51, 34, SSD1306_WHITE);
  displayTela.drawPixel(50, 34, SSD1306_WHITE);
  displayTela.drawPixel(49, 34, SSD1306_WHITE);
  displayTela.drawPixel(48, 34, SSD1306_WHITE);
  displayTela.drawPixel(47, 34, SSD1306_WHITE);
  displayTela.drawPixel(46, 34, SSD1306_WHITE);
  displayTela.drawPixel(45, 34, SSD1306_WHITE);
  displayTela.drawPixel(44, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 34, SSD1306_WHITE);
  displayTela.drawPixel(42, 34, SSD1306_WHITE);
  displayTela.drawPixel(41, 34, SSD1306_WHITE);
  displayTela.drawPixel(40, 34, SSD1306_WHITE);
  displayTela.drawPixel(39, 34, SSD1306_WHITE);
  displayTela.drawPixel(38, 34, SSD1306_WHITE);
  displayTela.drawPixel(37, 34, SSD1306_WHITE);
  displayTela.drawPixel(36, 34, SSD1306_WHITE);
  displayTela.drawPixel(35, 34, SSD1306_WHITE);
  displayTela.drawPixel(34, 34, SSD1306_WHITE);
  displayTela.drawPixel(33, 34, SSD1306_WHITE);
  displayTela.drawPixel(32, 34, SSD1306_WHITE);
  displayTela.drawPixel(31, 34, SSD1306_WHITE);
  displayTela.drawPixel(30, 34, SSD1306_WHITE);
  displayTela.drawPixel(29, 34, SSD1306_WHITE);
  displayTela.drawPixel(28, 34, SSD1306_WHITE);
  displayTela.drawPixel(27, 34, SSD1306_WHITE);
  displayTela.drawPixel(26, 34, SSD1306_WHITE);
  displayTela.drawPixel(25, 34, SSD1306_WHITE);
  displayTela.drawPixel(24, 35, SSD1306_WHITE);
  displayTela.drawPixel(23, 35, SSD1306_WHITE);
  displayTela.drawPixel(22, 35, SSD1306_WHITE);
  displayTela.drawPixel(21, 35, SSD1306_WHITE);
  displayTela.drawPixel(20, 35, SSD1306_WHITE);
  displayTela.drawPixel(19, 35, SSD1306_WHITE);
  displayTela.drawPixel(18, 35, SSD1306_WHITE);
  displayTela.drawPixel(17, 35, SSD1306_WHITE);
  displayTela.drawPixel(16, 35, SSD1306_WHITE);
  displayTela.drawPixel(15, 35, SSD1306_WHITE);
  displayTela.drawPixel(14, 35, SSD1306_WHITE);
  displayTela.drawPixel(13, 35, SSD1306_WHITE);
  displayTela.drawPixel(12, 35, SSD1306_WHITE);
  displayTela.drawPixel(11, 35, SSD1306_WHITE);
  displayTela.drawPixel(10, 35, SSD1306_WHITE);
  displayTela.drawPixel(9, 35, SSD1306_WHITE);
  displayTela.drawPixel(9, 34, SSD1306_WHITE);
  displayTela.drawPixel(10, 34, SSD1306_WHITE);
  displayTela.drawPixel(11, 34, SSD1306_WHITE);
  displayTela.drawPixel(12, 34, SSD1306_WHITE);
  displayTela.drawPixel(13, 34, SSD1306_WHITE);
  displayTela.drawPixel(14, 34, SSD1306_WHITE);
  displayTela.drawPixel(15, 34, SSD1306_WHITE);
  displayTela.drawPixel(16, 34, SSD1306_WHITE);
  displayTela.drawPixel(17, 34, SSD1306_WHITE);
  displayTela.drawPixel(18, 34, SSD1306_WHITE);
  displayTela.drawPixel(19, 34, SSD1306_WHITE);
  displayTela.drawPixel(20, 34, SSD1306_WHITE);
  displayTela.drawPixel(21, 34, SSD1306_WHITE);
  displayTela.drawPixel(22, 34, SSD1306_WHITE);
  displayTela.drawPixel(23, 34, SSD1306_WHITE);
  displayTela.drawPixel(24, 34, SSD1306_WHITE);
  //id: 3 pixel 44 
  displayTela.drawPixel(55, 8, SSD1306_BLACK);
  displayTela.drawPixel(54, 8, SSD1306_BLACK);
  displayTela.drawPixel(53, 7, SSD1306_BLACK);
  displayTela.drawPixel(52, 7, SSD1306_BLACK);
  displayTela.drawPixel(54, 7, SSD1306_BLACK);
  displayTela.drawPixel(55, 7, SSD1306_BLACK);
  displayTela.drawPixel(56, 7, SSD1306_BLACK);
  displayTela.drawPixel(57, 7, SSD1306_BLACK);
  displayTela.drawPixel(56, 6, SSD1306_BLACK);
  displayTela.drawPixel(55, 6, SSD1306_BLACK);
  displayTela.drawPixel(54, 6, SSD1306_BLACK);
  displayTela.drawPixel(53, 5, SSD1306_BLACK);
  displayTela.drawPixel(51, 5, SSD1306_BLACK);
  displayTela.drawPixel(50, 5, SSD1306_BLACK);
  displayTela.drawPixel(49, 5, SSD1306_BLACK);
  displayTela.drawPixel(49, 6, SSD1306_BLACK);
  displayTela.drawPixel(48, 6, SSD1306_BLACK);
  displayTela.drawPixel(48, 7, SSD1306_BLACK);
  displayTela.drawPixel(49, 8, SSD1306_BLACK);
  displayTela.drawPixel(50, 8, SSD1306_BLACK);
  displayTela.drawPixel(50, 9, SSD1306_BLACK);
  displayTela.drawPixel(51, 9, SSD1306_BLACK);
  displayTela.drawPixel(52, 9, SSD1306_BLACK);
  displayTela.drawPixel(53, 9, SSD1306_BLACK);
  displayTela.drawPixel(54, 9, SSD1306_BLACK);
  displayTela.drawPixel(51, 6, SSD1306_BLACK);
  displayTela.drawPixel(50, 6, SSD1306_BLACK);
  displayTela.drawPixel(49, 7, SSD1306_BLACK);
  displayTela.drawPixel(53, 8, SSD1306_BLACK);
  displayTela.drawPixel(53, 6, SSD1306_BLACK);
  displayTela.drawPixel(52, 6, SSD1306_BLACK);
  displayTela.drawPixel(51, 7, SSD1306_BLACK);
  displayTela.drawPixel(51, 8, SSD1306_BLACK);
  displayTela.drawPixel(48, 5, SSD1306_BLACK);
  displayTela.drawPixel(47, 5, SSD1306_BLACK);
  displayTela.drawPixel(46, 5, SSD1306_BLACK);
  displayTela.drawPixel(45, 5, SSD1306_BLACK);
  displayTela.drawPixel(44, 5, SSD1306_BLACK);
  displayTela.drawPixel(43, 5, SSD1306_BLACK);
  displayTela.drawPixel(42, 5, SSD1306_BLACK);
  displayTela.drawPixel(41, 5, SSD1306_BLACK);
  displayTela.drawPixel(40, 5, SSD1306_BLACK);
  displayTela.drawPixel(39, 5, SSD1306_BLACK);
  displayTela.drawPixel(38, 5, SSD1306_BLACK);
  displayTela.drawPixel(37, 5, SSD1306_BLACK);
  displayTela.drawPixel(36, 5, SSD1306_BLACK);
  displayTela.drawPixel(35, 5, SSD1306_BLACK);
  displayTela.drawPixel(34, 5, SSD1306_BLACK);
  displayTela.drawPixel(33, 5, SSD1306_BLACK);
  displayTela.drawPixel(32, 6, SSD1306_BLACK);
  displayTela.drawPixel(31, 6, SSD1306_BLACK);
  displayTela.drawPixel(30, 6, SSD1306_BLACK);
  displayTela.drawPixel(29, 6, SSD1306_BLACK);
  displayTela.drawPixel(28, 6, SSD1306_BLACK);
  displayTela.drawPixel(27, 6, SSD1306_BLACK);
  displayTela.drawPixel(26, 6, SSD1306_BLACK);
  displayTela.drawPixel(26, 7, SSD1306_BLACK);
  displayTela.drawPixel(25, 7, SSD1306_BLACK);
  displayTela.drawPixel(24, 7, SSD1306_BLACK);
  displayTela.drawPixel(23, 7, SSD1306_BLACK);
  displayTela.drawPixel(22, 7, SSD1306_BLACK);
  displayTela.drawPixel(21, 7, SSD1306_BLACK);
  displayTela.drawPixel(21, 8, SSD1306_BLACK);
  displayTela.drawPixel(20, 8, SSD1306_BLACK);
  displayTela.drawPixel(19, 8, SSD1306_BLACK);
  displayTela.drawPixel(18, 8, SSD1306_BLACK);
  displayTela.drawPixel(17, 8, SSD1306_BLACK);
  displayTela.drawPixel(22, 8, SSD1306_BLACK);
  displayTela.drawPixel(23, 8, SSD1306_BLACK);
  displayTela.drawPixel(24, 8, SSD1306_BLACK);
  displayTela.drawPixel(25, 8, SSD1306_BLACK);
  displayTela.drawPixel(26, 8, SSD1306_BLACK);
  displayTela.drawPixel(27, 8, SSD1306_BLACK);
  displayTela.drawPixel(28, 8, SSD1306_BLACK);
  displayTela.drawPixel(29, 8, SSD1306_BLACK);
  displayTela.drawPixel(29, 7, SSD1306_BLACK);
  displayTela.drawPixel(30, 7, SSD1306_BLACK);
  displayTela.drawPixel(31, 7, SSD1306_BLACK);
  displayTela.drawPixel(32, 7, SSD1306_BLACK);
  displayTela.drawPixel(33, 7, SSD1306_BLACK);
  displayTela.drawPixel(33, 6, SSD1306_BLACK);
  displayTela.drawPixel(34, 6, SSD1306_BLACK);
  displayTela.drawPixel(35, 6, SSD1306_BLACK);
  displayTela.drawPixel(36, 6, SSD1306_BLACK);
  displayTela.drawPixel(28, 7, SSD1306_BLACK);
  displayTela.drawPixel(27, 7, SSD1306_BLACK);
  displayTela.drawPixel(22, 9, SSD1306_BLACK);
  displayTela.drawPixel(21, 9, SSD1306_BLACK);
  displayTela.drawPixel(21, 10, SSD1306_BLACK);
  displayTela.drawPixel(20, 10, SSD1306_BLACK);
  displayTela.drawPixel(19, 10, SSD1306_BLACK);
  displayTela.drawPixel(18, 10, SSD1306_BLACK);
  displayTela.drawPixel(17, 10, SSD1306_BLACK);
  displayTela.drawPixel(16, 11, SSD1306_BLACK);
  displayTela.drawPixel(15, 11, SSD1306_BLACK);
  displayTela.drawPixel(14, 11, SSD1306_BLACK);
  displayTela.drawPixel(13, 11, SSD1306_BLACK);
  displayTela.drawPixel(13, 12, SSD1306_BLACK);
  displayTela.drawPixel(12, 12, SSD1306_BLACK);
  //id: 4 pixel 45 
  displayTela.drawPixel(23, 11, SSD1306_WHITE);
  displayTela.drawPixel(23, 12, SSD1306_WHITE);
  displayTela.drawPixel(23, 13, SSD1306_WHITE);
  displayTela.drawPixel(22, 11, SSD1306_WHITE);
  displayTela.drawPixel(21, 12, SSD1306_WHITE);
  displayTela.drawPixel(21, 13, SSD1306_WHITE);
  displayTela.drawPixel(22, 13, SSD1306_WHITE);
  displayTela.drawPixel(22, 12, SSD1306_WHITE);
  displayTela.drawPixel(23, 10, SSD1306_WHITE);
  displayTela.drawPixel(22, 10, SSD1306_WHITE);
  displayTela.drawPixel(21, 11, SSD1306_WHITE);
  displayTela.drawPixel(20, 11, SSD1306_WHITE);
  displayTela.drawPixel(20, 12, SSD1306_WHITE);
  displayTela.drawPixel(20, 13, SSD1306_WHITE);
  displayTela.drawPixel(24, 12, SSD1306_WHITE);
  displayTela.drawPixel(24, 11, SSD1306_WHITE);
  displayTela.drawPixel(25, 11, SSD1306_WHITE);
  displayTela.drawPixel(26, 11, SSD1306_WHITE);
  displayTela.drawPixel(27, 10, SSD1306_WHITE);
  displayTela.drawPixel(28, 10, SSD1306_WHITE);
  displayTela.drawPixel(28, 9, SSD1306_WHITE);
  displayTela.drawPixel(29, 9, SSD1306_WHITE);
  displayTela.drawPixel(30, 9, SSD1306_WHITE);
  displayTela.drawPixel(31, 9, SSD1306_WHITE);
  displayTela.drawPixel(32, 9, SSD1306_WHITE);
  displayTela.drawPixel(33, 9, SSD1306_WHITE);
  displayTela.drawPixel(34, 9, SSD1306_WHITE);
  displayTela.drawPixel(35, 9, SSD1306_WHITE);
  displayTela.drawPixel(35, 10, SSD1306_WHITE);
  displayTela.drawPixel(36, 10, SSD1306_WHITE);
  displayTela.drawPixel(37, 10, SSD1306_WHITE);
  displayTela.drawPixel(38, 10, SSD1306_WHITE);
  displayTela.drawPixel(39, 10, SSD1306_WHITE);
  displayTela.drawPixel(39, 11, SSD1306_WHITE);
  displayTela.drawPixel(40, 11, SSD1306_WHITE);
  displayTela.drawPixel(41, 11, SSD1306_WHITE);
  displayTela.drawPixel(41, 12, SSD1306_WHITE);
  displayTela.drawPixel(42, 12, SSD1306_WHITE);
  displayTela.drawPixel(43, 13, SSD1306_WHITE);
  displayTela.drawPixel(44, 13, SSD1306_WHITE);
  displayTela.drawPixel(44, 14, SSD1306_WHITE);
  displayTela.drawPixel(45, 14, SSD1306_WHITE);
  displayTela.drawPixel(45, 13, SSD1306_WHITE);
  displayTela.drawPixel(44, 12, SSD1306_WHITE);
  displayTela.drawPixel(43, 12, SSD1306_WHITE);
  displayTela.drawPixel(43, 11, SSD1306_WHITE);
  displayTela.drawPixel(42, 11, SSD1306_WHITE);
  displayTela.drawPixel(44, 11, SSD1306_WHITE);
  displayTela.drawPixel(45, 11, SSD1306_WHITE);
  displayTela.drawPixel(45, 12, SSD1306_WHITE);
  displayTela.drawPixel(46, 12, SSD1306_WHITE);
  displayTela.drawPixel(47, 12, SSD1306_WHITE);
  displayTela.drawPixel(48, 12, SSD1306_WHITE);
  displayTela.drawPixel(48, 13, SSD1306_WHITE);
  displayTela.drawPixel(49, 13, SSD1306_WHITE);
  displayTela.drawPixel(49, 14, SSD1306_WHITE);
  displayTela.drawPixel(49, 15, SSD1306_WHITE);
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  displayTela.drawPixel(48, 17, SSD1306_WHITE);
  displayTela.drawPixel(48, 15, SSD1306_WHITE);
  displayTela.drawPixel(47, 15, SSD1306_WHITE);
  displayTela.drawPixel(47, 14, SSD1306_WHITE);
  displayTela.drawPixel(47, 13, SSD1306_WHITE);
  displayTela.drawPixel(46, 13, SSD1306_WHITE);
  displayTela.drawPixel(45, 15, SSD1306_WHITE);
  displayTela.drawPixel(46, 16, SSD1306_WHITE);
  displayTela.drawPixel(46, 17, SSD1306_WHITE);
  displayTela.drawPixel(47, 17, SSD1306_WHITE);
  //id: 5 pixel 46 
  displayTela.drawPixel(49, 13, SSD1306_BLACK);
  displayTela.drawPixel(49, 12, SSD1306_BLACK);
  displayTela.drawPixel(48, 11, SSD1306_BLACK);
  displayTela.drawPixel(48, 10, SSD1306_BLACK);
  displayTela.drawPixel(48, 9, SSD1306_BLACK);
  displayTela.drawPixel(47, 9, SSD1306_BLACK);
  //id: 6 pixel 47 
  displayTela.drawPixel(40, 0, SSD1306_WHITE);
  //id: 7 pixel 48 
  displayTela.drawPixel(40, 0, SSD1306_WHITE);
  //id: 8 pixel 49 
  displayTela.drawPixel(37, 3, SSD1306_WHITE);
  //id: 9 pixel 50 
  displayTela.drawPixel(120, 25, SSD1306_WHITE);
  //id: 10 pixel 51 
  displayTela.drawPixel(12, 35, SSD1306_WHITE);
  displayTela.drawPixel(13, 35, SSD1306_WHITE);
  displayTela.drawPixel(14, 35, SSD1306_WHITE);
  displayTela.drawPixel(15, 35, SSD1306_WHITE);
  displayTela.drawPixel(16, 35, SSD1306_WHITE);
  displayTela.drawPixel(17, 35, SSD1306_WHITE);
  displayTela.drawPixel(18, 35, SSD1306_WHITE);
  displayTela.drawPixel(19, 35, SSD1306_WHITE);
  displayTela.drawPixel(20, 35, SSD1306_WHITE);
  displayTela.drawPixel(21, 35, SSD1306_WHITE);
  displayTela.drawPixel(22, 35, SSD1306_WHITE);
  displayTela.drawPixel(23, 35, SSD1306_WHITE);
  displayTela.drawPixel(24, 35, SSD1306_WHITE);
  displayTela.drawPixel(24, 34, SSD1306_WHITE);
  displayTela.drawPixel(25, 34, SSD1306_WHITE);
  displayTela.drawPixel(26, 34, SSD1306_WHITE);
  displayTela.drawPixel(27, 34, SSD1306_WHITE);
  displayTela.drawPixel(28, 34, SSD1306_WHITE);
  displayTela.drawPixel(29, 34, SSD1306_WHITE);
  displayTela.drawPixel(30, 34, SSD1306_WHITE);
  //id: 11 pixel 52 
  displayTela.drawPixel(24, 35, SSD1306_WHITE);
  displayTela.drawPixel(25, 35, SSD1306_WHITE);
  //id: 12 pixel 53 
  displayTela.drawPixel(25, 35, SSD1306_WHITE);
  displayTela.drawPixel(26, 35, SSD1306_WHITE);
  displayTela.drawPixel(27, 35, SSD1306_WHITE);
  displayTela.drawPixel(28, 35, SSD1306_WHITE);
  displayTela.drawPixel(28, 34, SSD1306_WHITE);
  displayTela.drawPixel(29, 34, SSD1306_WHITE);
  displayTela.drawPixel(30, 34, SSD1306_WHITE);
  displayTela.drawPixel(31, 34, SSD1306_WHITE);
  displayTela.drawPixel(32, 34, SSD1306_WHITE);
  displayTela.drawPixel(33, 34, SSD1306_WHITE);
  displayTela.drawPixel(34, 34, SSD1306_WHITE);
  displayTela.drawPixel(35, 34, SSD1306_WHITE);
  displayTela.drawPixel(36, 34, SSD1306_WHITE);
  displayTela.drawPixel(37, 34, SSD1306_WHITE);
  displayTela.drawPixel(38, 34, SSD1306_WHITE);
  displayTela.drawPixel(39, 34, SSD1306_WHITE);
  displayTela.drawPixel(40, 34, SSD1306_WHITE);
  displayTela.drawPixel(39, 35, SSD1306_WHITE);
  displayTela.drawPixel(38, 35, SSD1306_WHITE);
  displayTela.drawPixel(37, 35, SSD1306_WHITE);
  displayTela.drawPixel(36, 35, SSD1306_WHITE);
  displayTela.drawPixel(35, 35, SSD1306_WHITE);
  displayTela.drawPixel(34, 35, SSD1306_WHITE);
  displayTela.drawPixel(33, 35, SSD1306_WHITE);
  displayTela.drawPixel(32, 35, SSD1306_WHITE);
  displayTela.drawPixel(31, 35, SSD1306_WHITE);
  displayTela.drawPixel(30, 35, SSD1306_WHITE);
  displayTela.drawPixel(29, 35, SSD1306_WHITE);
  displayTela.drawPixel(40, 35, SSD1306_WHITE);
  displayTela.drawPixel(41, 35, SSD1306_WHITE);
  displayTela.drawPixel(42, 35, SSD1306_WHITE);
  displayTela.drawPixel(43, 35, SSD1306_WHITE);
  displayTela.drawPixel(44, 35, SSD1306_WHITE);
  displayTela.drawPixel(45, 35, SSD1306_WHITE);
  displayTela.drawPixel(46, 35, SSD1306_WHITE);
  displayTela.drawPixel(47, 35, SSD1306_WHITE);
  displayTela.drawPixel(48, 35, SSD1306_WHITE);
  displayTela.drawPixel(49, 35, SSD1306_WHITE);
  displayTela.drawPixel(50, 35, SSD1306_WHITE);
  displayTela.drawPixel(51, 35, SSD1306_WHITE);
  displayTela.drawPixel(52, 35, SSD1306_WHITE);
  displayTela.drawPixel(53, 35, SSD1306_WHITE);
  displayTela.drawPixel(54, 35, SSD1306_WHITE);
  displayTela.drawPixel(55, 35, SSD1306_WHITE);
  displayTela.drawPixel(56, 35, SSD1306_WHITE);
  displayTela.drawPixel(57, 35, SSD1306_WHITE);
  displayTela.drawPixel(58, 35, SSD1306_WHITE);
  //id: 13 line 54 
  displayTela.drawLine(9, 30, 58, 30, SSD1306_WHITE);
  //id: 14 line 55 
  displayTela.drawLine(58, 30, 58, 34, SSD1306_WHITE);
  //id: 15 line 56 
  displayTela.drawLine(57, 33, 57, 33, SSD1306_WHITE);
  //id: 16 line 57 
  displayTela.drawLine(10, 26, 56, 26, SSD1306_WHITE);
  //id: 17 line 58 
  displayTela.drawLine(10, 31, 10, 33, SSD1306_BLACK);
  //id: 18 line 59 
  displayTela.drawLine(57, 31, 57, 35, SSD1306_BLACK);
  //id: 19 line 60 
  displayTela.drawLine(57, 34, 10, 34, SSD1306_BLACK);
  //id: 20 line 61 
  displayTela.drawLine(57, 35, 57, 35, SSD1306_WHITE);
  //id: 21 line 62 
  displayTela.drawLine(57, 26, 57, 29, SSD1306_WHITE);
  //id: 22 line 63 
  displayTela.drawLine(55, 28, 55, 27, SSD1306_WHITE);
  //id: 23 line 64 
  displayTela.drawLine(55, 28, 55, 28, SSD1306_WHITE);
  //id: 24 line 65 
  displayTela.drawLine(11, 24, 19, 23, SSD1306_WHITE);
  //id: 25 line 66 
  displayTela.drawLine(20, 23, 13, 24, SSD1306_WHITE);
  //id: 26 line 67 
  displayTela.drawLine(11, 24, 11, 24, SSD1306_WHITE);
  //id: 27 line 68 
  displayTela.drawLine(19, 24, 42, 24, SSD1306_WHITE);
  //id: 28 line 69 
  displayTela.drawLine(16, 24, 54, 24, SSD1306_WHITE);
  //id: 29 line 70 
  displayTela.drawLine(12, 22, 53, 22, SSD1306_WHITE);
  //id: 30 line 71 
  displayTela.drawLine(14, 20, 14, 20, SSD1306_WHITE);
  //id: 31 line 72 
  displayTela.drawLine(18, 20, 39, 20, SSD1306_WHITE);
  //id: 32 line 73 
  displayTela.drawLine(14, 20, 52, 20, SSD1306_WHITE);
  //id: 33 line 74 
  displayTela.drawLine(15, 19, 52, 19, SSD1306_WHITE);
  //id: 34 line 75 
  displayTela.drawLine(15, 17, 51, 17, SSD1306_WHITE);
  //id: 35 line 76 
  displayTela.drawLine(11, 27, 11, 27, SSD1306_BLACK);
  //id: 36 line 77 
  displayTela.drawLine(12, 25, 12, 25, SSD1306_BLACK);
  //id: 37 line 78 
  displayTela.drawLine(13, 23, 13, 23, SSD1306_BLACK);
  //id: 38 line 79 
  displayTela.drawLine(14, 21, 14, 21, SSD1306_BLACK);
  //id: 39 line 80 
  displayTela.drawLine(16, 18, 16, 18, SSD1306_BLACK);
  //id: 40 line 81 
  displayTela.drawLine(15, 23, 30, 23, SSD1306_BLACK);
  //id: 41 line 82 
  displayTela.drawLine(56, 27, 56, 27, SSD1306_BLACK);
  //id: 42 line 83 
  displayTela.drawLine(56, 27, 56, 28, SSD1306_BLACK);
  //id: 43 line 84 
  displayTela.drawLine(55, 28, 55, 27, SSD1306_BLACK);
  //id: 44 line 86 
  displayTela.drawLine(54, 23, 54, 23, SSD1306_BLACK);
  //id: 45 line 87 
  displayTela.drawLine(53, 21, 53, 21, SSD1306_BLACK);
  //id: 46 line 88 
  displayTela.drawLine(54, 23, 54, 23, SSD1306_WHITE);
  //id: 47 line 89 
  displayTela.drawLine(54, 20, 54, 20, SSD1306_WHITE);
  //id: 48 line 90 
  displayTela.drawLine(53, 19, 53, 19, SSD1306_WHITE);
  //id: 49 line 91 
  displayTela.drawLine(55, 23, 55, 23, SSD1306_BLACK);
  //id: 50 line 92 
  displayTela.drawLine(54, 20, 54, 20, SSD1306_BLACK);
  //id: 51 line 93 
  displayTela.drawLine(54, 21, 54, 21, SSD1306_BLACK);
  //id: 52 line 94 
  displayTela.drawLine(53, 21, 53, 21, SSD1306_WHITE);
  //id: 53 line 95 
  displayTela.drawLine(56, 25, 56, 25, SSD1306_WHITE);
  //id: 54 line 96 
  displayTela.drawLine(56, 24, 56, 24, SSD1306_WHITE);
  //id: 55 line 97 
  displayTela.drawLine(56, 24, 56, 24, SSD1306_WHITE);
  //id: 56 line 98 
  displayTela.drawLine(55, 23, 55, 23, SSD1306_WHITE);
  //id: 57 line 99 
  displayTela.drawLine(55, 23, 55, 22, SSD1306_WHITE);
  //id: 58 line 100 
  displayTela.drawLine(54, 21, 54, 20, SSD1306_WHITE);
  //id: 59 line 101 
  displayTela.drawLine(54, 20, 54, 20, SSD1306_WHITE);
  //id: 60 line 105 
  displayTela.drawLine(55, 25, 55, 25, SSD1306_BLACK);
  //id: 61 line 106 
  displayTela.drawLine(54, 23, 54, 23, SSD1306_BLACK);
  //id: 62 line 107 
  displayTela.drawLine(53, 21, 53, 21, SSD1306_BLACK);
  //id: 63 line 108 
  displayTela.drawLine(16, 16, 48, 16, SSD1306_WHITE);
  //id: 64 line 109 
  displayTela.drawLine(17, 15, 19, 15, SSD1306_WHITE);
  //id: 65 line 110 
  displayTela.drawLine(21, 15, 25, 14, SSD1306_WHITE);
  //id: 66 line 111 
  displayTela.drawLine(27, 14, 48, 15, SSD1306_WHITE);
  //id: 67 pixel 112 
  displayTela.drawPixel(18, 13, SSD1306_BLACK);
  //id: 68 pixel 113 
  displayTela.drawPixel(18, 13, SSD1306_BLACK);
  displayTela.drawPixel(18, 12, SSD1306_BLACK);
  displayTela.drawPixel(19, 12, SSD1306_BLACK);
  displayTela.drawPixel(19, 11, SSD1306_BLACK);
  displayTela.drawPixel(19, 10, SSD1306_BLACK);
  displayTela.drawPixel(20, 10, SSD1306_BLACK);
  displayTela.drawPixel(21, 10, SSD1306_BLACK);
  displayTela.drawPixel(21, 9, SSD1306_BLACK);
  displayTela.drawPixel(22, 9, SSD1306_BLACK);
  displayTela.drawPixel(23, 9, SSD1306_BLACK);
  displayTela.drawPixel(24, 8, SSD1306_BLACK);
  displayTela.drawPixel(25, 8, SSD1306_BLACK);
  displayTela.drawPixel(23, 8, SSD1306_BLACK);
  displayTela.drawPixel(20, 11, SSD1306_BLACK);
  displayTela.drawPixel(24, 9, SSD1306_BLACK);
  displayTela.drawPixel(26, 8, SSD1306_BLACK);
  displayTela.drawPixel(22, 10, SSD1306_BLACK);
  displayTela.drawPixel(25, 9, SSD1306_BLACK);
  displayTela.drawPixel(27, 8, SSD1306_BLACK);
  displayTela.drawPixel(27, 7, SSD1306_BLACK);
  displayTela.drawPixel(28, 7, SSD1306_BLACK);
  displayTela.drawPixel(28, 6, SSD1306_BLACK);
  //id: 69 line 114 
  displayTela.drawLine(18, 14, 48, 14, SSD1306_WHITE);
  //id: 70 line 115 
  displayTela.drawLine(43, 15, 18, 15, SSD1306_WHITE);
  //id: 71 line 116 
  displayTela.drawLine(32, 12, 44, 13, SSD1306_WHITE);
  //id: 72 line 117 
  displayTela.drawLine(43, 13, 21, 13, SSD1306_WHITE);
  //id: 73 line 118 
  displayTela.drawLine(24, 12, 24, 12, SSD1306_WHITE);
  //id: 74 line 119 
  displayTela.drawLine(24, 12, 43, 12, SSD1306_WHITE);
  //id: 75 line 120 
  displayTela.drawLine(38, 10, 38, 10, SSD1306_WHITE);
  //id: 76 line 121 
  displayTela.drawLine(38, 11, 24, 11, SSD1306_WHITE);
  //id: 77 line 122 
  displayTela.drawLine(26, 10, 28, 10, SSD1306_WHITE);
  //id: 78 line 123 
  displayTela.drawLine(28, 10, 36, 10, SSD1306_WHITE);
  //id: 79 pixel 124 
  displayTela.drawPixel(53, 18, SSD1306_WHITE);
  //id: 80 pixel 125 
  displayTela.drawPixel(53, 18, SSD1306_WHITE);
  displayTela.drawPixel(53, 17, SSD1306_WHITE);
  //id: 81 pixel 126 
  displayTela.drawPixel(53, 18, SSD1306_BLACK);
  //id: 82 pixel 127 
  displayTela.drawPixel(53, 18, SSD1306_BLACK);
  displayTela.drawPixel(53, 17, SSD1306_BLACK);
  //id: 83 pixel 128 
  displayTela.drawPixel(52, 18, SSD1306_WHITE);
  //id: 84 pixel 129 
  displayTela.drawPixel(52, 18, SSD1306_WHITE);
  displayTela.drawPixel(52, 17, SSD1306_WHITE);
  //id: 85 pixel 130 
  displayTela.drawPixel(52, 17, SSD1306_WHITE);
  //id: 86 pixel 131 
  displayTela.drawPixel(51, 16, SSD1306_WHITE);
  //id: 87 pixel 132 
  displayTela.drawPixel(51, 16, SSD1306_WHITE);
  //id: 88 pixel 133 
  displayTela.drawPixel(50, 15, SSD1306_WHITE);
  //id: 89 pixel 134 
  displayTela.drawPixel(48, 12, SSD1306_BLACK);
  //id: 90 pixel 135 
  displayTela.drawPixel(48, 12, SSD1306_BLACK);
  //id: 91 pixel 136 
  displayTela.drawPixel(46, 11, SSD1306_WHITE);
  //id: 92 pixel 137 
  displayTela.drawPixel(44, 10, SSD1306_WHITE);
  displayTela.drawPixel(43, 10, SSD1306_WHITE);
  //id: 93 pixel 138 
  displayTela.drawPixel(44, 10, SSD1306_WHITE);
  displayTela.drawPixel(45, 10, SSD1306_WHITE);
  //id: 94 pixel 139 
  displayTela.drawPixel(45, 10, SSD1306_WHITE);
  //id: 95 pixel 140 
  displayTela.drawPixel(45, 9, SSD1306_WHITE);
  displayTela.drawPixel(44, 9, SSD1306_WHITE);
  displayTela.drawPixel(43, 9, SSD1306_WHITE);
  displayTela.drawPixel(42, 9, SSD1306_WHITE);
  displayTela.drawPixel(41, 9, SSD1306_WHITE);
  displayTela.drawPixel(40, 9, SSD1306_WHITE);
  //id: 96 pixel 141 
  displayTela.drawPixel(44, 9, SSD1306_BLACK);
  //id: 97 pixel 142 
  displayTela.drawPixel(45, 9, SSD1306_BLACK);
  //id: 98 pixel 143 
  displayTela.drawPixel(45, 10, SSD1306_BLACK);
  //id: 99 pixel 144 
  displayTela.drawPixel(42, 9, SSD1306_BLACK);
  displayTela.drawPixel(43, 9, SSD1306_BLACK);
  //id: 100 line 145 
  displayTela.drawLine(16, 18, 50, 18, SSD1306_WHITE);
  //id: 101 line 146 
  displayTela.drawLine(53, 21, 53, 21, SSD1306_WHITE);
  //id: 102 line 147 
  displayTela.drawLine(53, 21, 14, 21, SSD1306_WHITE);
  //id: 103 line 148 
  displayTela.drawLine(13, 23, 14, 23, SSD1306_WHITE);
  //id: 104 line 149 
  displayTela.drawLine(19, 23, 55, 23, SSD1306_WHITE);
  //id: 105 line 150 
  displayTela.drawLine(18, 23, 15, 23, SSD1306_WHITE);
  //id: 106 line 151 
  displayTela.drawLine(12, 25, 12, 25, SSD1306_WHITE);
  //id: 107 line 152 
  displayTela.drawLine(12, 25, 55, 25, SSD1306_WHITE);
  //id: 108 line 153 
  displayTela.drawLine(56, 27, 56, 27, SSD1306_WHITE);
  //id: 109 line 154 
  displayTela.drawLine(55, 27, 11, 27, SSD1306_WHITE);
  //id: 110 pixel 155 
  displayTela.drawPixel(12, 28, SSD1306_WHITE);
  displayTela.drawPixel(11, 28, SSD1306_WHITE);
  displayTela.drawPixel(11, 29, SSD1306_WHITE);
  displayTela.drawPixel(12, 29, SSD1306_WHITE);
  displayTela.drawPixel(13, 29, SSD1306_WHITE);
  displayTela.drawPixel(14, 28, SSD1306_WHITE);
  displayTela.drawPixel(15, 28, SSD1306_WHITE);
  displayTela.drawPixel(16, 28, SSD1306_WHITE);
  displayTela.drawPixel(17, 28, SSD1306_WHITE);
  displayTela.drawPixel(18, 28, SSD1306_WHITE);
  displayTela.drawPixel(19, 28, SSD1306_WHITE);
  displayTela.drawPixel(20, 28, SSD1306_WHITE);
  displayTela.drawPixel(21, 28, SSD1306_WHITE);
  displayTela.drawPixel(22, 28, SSD1306_WHITE);
  displayTela.drawPixel(23, 28, SSD1306_WHITE);
  displayTela.drawPixel(24, 28, SSD1306_WHITE);
  displayTela.drawPixel(25, 28, SSD1306_WHITE);
  displayTela.drawPixel(26, 28, SSD1306_WHITE);
  displayTela.drawPixel(27, 28, SSD1306_WHITE);
  displayTela.drawPixel(28, 28, SSD1306_WHITE);
  displayTela.drawPixel(29, 28, SSD1306_WHITE);
  displayTela.drawPixel(30, 28, SSD1306_WHITE);
  displayTela.drawPixel(31, 28, SSD1306_WHITE);
  displayTela.drawPixel(32, 28, SSD1306_WHITE);
  displayTela.drawPixel(33, 28, SSD1306_WHITE);
  displayTela.drawPixel(34, 28, SSD1306_WHITE);
  displayTela.drawPixel(35, 28, SSD1306_WHITE);
  displayTela.drawPixel(36, 28, SSD1306_WHITE);
  displayTela.drawPixel(37, 28, SSD1306_WHITE);
  displayTela.drawPixel(38, 28, SSD1306_WHITE);
  displayTela.drawPixel(39, 28, SSD1306_WHITE);
  displayTela.drawPixel(40, 28, SSD1306_WHITE);
  displayTela.drawPixel(41, 28, SSD1306_WHITE);
  displayTela.drawPixel(42, 28, SSD1306_WHITE);
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  displayTela.drawPixel(44, 28, SSD1306_WHITE);
  displayTela.drawPixel(45, 28, SSD1306_WHITE);
  displayTela.drawPixel(46, 28, SSD1306_WHITE);
  displayTela.drawPixel(47, 28, SSD1306_WHITE);
  displayTela.drawPixel(48, 28, SSD1306_WHITE);
  displayTela.drawPixel(49, 28, SSD1306_WHITE);
  displayTela.drawPixel(50, 28, SSD1306_WHITE);
  displayTela.drawPixel(51, 29, SSD1306_WHITE);
  displayTela.drawPixel(52, 29, SSD1306_WHITE);
  displayTela.drawPixel(53, 29, SSD1306_WHITE);
  displayTela.drawPixel(54, 29, SSD1306_WHITE);
  displayTela.drawPixel(55, 29, SSD1306_WHITE);
  displayTela.drawPixel(56, 29, SSD1306_WHITE);
  displayTela.drawPixel(56, 28, SSD1306_WHITE);
  displayTela.drawPixel(55, 28, SSD1306_WHITE);
  displayTela.drawPixel(54, 28, SSD1306_WHITE);
  displayTela.drawPixel(53, 28, SSD1306_WHITE);
  displayTela.drawPixel(52, 28, SSD1306_WHITE);
  displayTela.drawPixel(51, 28, SSD1306_WHITE);
  displayTela.drawPixel(50, 29, SSD1306_WHITE);
  displayTela.drawPixel(49, 29, SSD1306_WHITE);
  displayTela.drawPixel(48, 29, SSD1306_WHITE);
  displayTela.drawPixel(47, 29, SSD1306_WHITE);
  displayTela.drawPixel(46, 29, SSD1306_WHITE);
  displayTela.drawPixel(45, 29, SSD1306_WHITE);
  displayTela.drawPixel(44, 29, SSD1306_WHITE);
  displayTela.drawPixel(43, 29, SSD1306_WHITE);
  displayTela.drawPixel(32, 29, SSD1306_WHITE);
  displayTela.drawPixel(31, 29, SSD1306_WHITE);
  displayTela.drawPixel(30, 29, SSD1306_WHITE);
  displayTela.drawPixel(29, 29, SSD1306_WHITE);
  displayTela.drawPixel(28, 29, SSD1306_WHITE);
  displayTela.drawPixel(27, 29, SSD1306_WHITE);
  displayTela.drawPixel(26, 29, SSD1306_WHITE);
  displayTela.drawPixel(25, 29, SSD1306_WHITE);
  displayTela.drawPixel(24, 29, SSD1306_WHITE);
  displayTela.drawPixel(23, 29, SSD1306_WHITE);
  displayTela.drawPixel(22, 29, SSD1306_WHITE);
  displayTela.drawPixel(21, 29, SSD1306_WHITE);
  displayTela.drawPixel(20, 29, SSD1306_WHITE);
  displayTela.drawPixel(19, 29, SSD1306_WHITE);
  displayTela.drawPixel(18, 29, SSD1306_WHITE);
  displayTela.drawPixel(17, 29, SSD1306_WHITE);
  displayTela.drawPixel(16, 29, SSD1306_WHITE);
  displayTela.drawPixel(15, 29, SSD1306_WHITE);
  displayTela.drawPixel(14, 29, SSD1306_WHITE);
  displayTela.drawPixel(13, 28, SSD1306_WHITE);
  displayTela.drawPixel(13, 27, SSD1306_WHITE);
  displayTela.drawPixel(14, 27, SSD1306_WHITE);
  displayTela.drawPixel(15, 27, SSD1306_WHITE);
  displayTela.drawPixel(16, 27, SSD1306_WHITE);
  displayTela.drawPixel(17, 27, SSD1306_WHITE);
  displayTela.drawPixel(19, 27, SSD1306_WHITE);
  displayTela.drawPixel(20, 27, SSD1306_WHITE);
  displayTela.drawPixel(21, 27, SSD1306_WHITE);
  displayTela.drawPixel(23, 27, SSD1306_WHITE);
  displayTela.drawPixel(24, 27, SSD1306_WHITE);
  displayTela.drawPixel(26, 27, SSD1306_WHITE);
  displayTela.drawPixel(28, 27, SSD1306_WHITE);
  displayTela.drawPixel(30, 27, SSD1306_WHITE);
  displayTela.drawPixel(31, 27, SSD1306_WHITE);
  displayTela.drawPixel(33, 27, SSD1306_WHITE);
  displayTela.drawPixel(34, 27, SSD1306_WHITE);
  displayTela.drawPixel(36, 27, SSD1306_WHITE);
  displayTela.drawPixel(37, 27, SSD1306_WHITE);
  displayTela.drawPixel(38, 27, SSD1306_WHITE);
  displayTela.drawPixel(39, 27, SSD1306_WHITE);
  displayTela.drawPixel(40, 27, SSD1306_WHITE);
  displayTela.drawPixel(34, 29, SSD1306_WHITE);
  displayTela.drawPixel(33, 29, SSD1306_WHITE);
  displayTela.drawPixel(42, 29, SSD1306_WHITE);
  displayTela.drawPixel(41, 29, SSD1306_WHITE);
  displayTela.drawPixel(40, 29, SSD1306_WHITE);
  displayTela.drawPixel(39, 29, SSD1306_WHITE);
  displayTela.drawPixel(38, 29, SSD1306_WHITE);
  displayTela.drawPixel(37, 29, SSD1306_WHITE);
  displayTela.drawPixel(36, 29, SSD1306_WHITE);
  displayTela.drawPixel(35, 29, SSD1306_WHITE);
  displayTela.drawPixel(46, 30, SSD1306_WHITE);
  displayTela.drawPixel(47, 30, SSD1306_WHITE);
  displayTela.drawPixel(48, 30, SSD1306_WHITE);
  displayTela.drawPixel(48, 31, SSD1306_WHITE);
  displayTela.drawPixel(49, 31, SSD1306_WHITE);
  displayTela.drawPixel(50, 31, SSD1306_WHITE);
  displayTela.drawPixel(51, 31, SSD1306_WHITE);
  displayTela.drawPixel(52, 31, SSD1306_WHITE);
  displayTela.drawPixel(52, 32, SSD1306_WHITE);
  displayTela.drawPixel(53, 32, SSD1306_WHITE);
  displayTela.drawPixel(54, 32, SSD1306_WHITE);
  displayTela.drawPixel(54, 31, SSD1306_WHITE);
  displayTela.drawPixel(54, 30, SSD1306_WHITE);
  displayTela.drawPixel(55, 31, SSD1306_WHITE);
  displayTela.drawPixel(55, 32, SSD1306_WHITE);
  displayTela.drawPixel(55, 33, SSD1306_WHITE);
  displayTela.drawPixel(56, 31, SSD1306_WHITE);
  displayTela.drawPixel(56, 32, SSD1306_WHITE);
  displayTela.drawPixel(56, 33, SSD1306_WHITE);
  displayTela.drawPixel(56, 30, SSD1306_WHITE);
  displayTela.drawPixel(57, 31, SSD1306_WHITE);
  displayTela.drawPixel(57, 32, SSD1306_WHITE);
  displayTela.drawPixel(57, 33, SSD1306_WHITE);
  displayTela.drawPixel(54, 33, SSD1306_WHITE);
  displayTela.drawPixel(53, 33, SSD1306_WHITE);
  displayTela.drawPixel(52, 33, SSD1306_WHITE);
  displayTela.drawPixel(51, 33, SSD1306_WHITE);
  displayTela.drawPixel(50, 33, SSD1306_WHITE);
  displayTela.drawPixel(49, 33, SSD1306_WHITE);
  displayTela.drawPixel(48, 33, SSD1306_WHITE);
  displayTela.drawPixel(47, 33, SSD1306_WHITE);
  displayTela.drawPixel(46, 33, SSD1306_WHITE);
  displayTela.drawPixel(45, 33, SSD1306_WHITE);
  displayTela.drawPixel(44, 33, SSD1306_WHITE);
  displayTela.drawPixel(43, 33, SSD1306_WHITE);
  displayTela.drawPixel(42, 33, SSD1306_WHITE);
  displayTela.drawPixel(41, 33, SSD1306_WHITE);
  displayTela.drawPixel(40, 33, SSD1306_WHITE);
  displayTela.drawPixel(39, 33, SSD1306_WHITE);
  displayTela.drawPixel(38, 34, SSD1306_WHITE);
  displayTela.drawPixel(37, 34, SSD1306_WHITE);
  displayTela.drawPixel(36, 34, SSD1306_WHITE);
  displayTela.drawPixel(35, 34, SSD1306_WHITE);
  displayTela.drawPixel(34, 34, SSD1306_WHITE);
  displayTela.drawPixel(33, 34, SSD1306_WHITE);
  displayTela.drawPixel(32, 34, SSD1306_WHITE);
  displayTela.drawPixel(31, 34, SSD1306_WHITE);
  displayTela.drawPixel(30, 34, SSD1306_WHITE);
  displayTela.drawPixel(29, 34, SSD1306_WHITE);
  displayTela.drawPixel(28, 34, SSD1306_WHITE);
  displayTela.drawPixel(27, 34, SSD1306_WHITE);
  displayTela.drawPixel(26, 34, SSD1306_WHITE);
  displayTela.drawPixel(25, 34, SSD1306_WHITE);
  displayTela.drawPixel(24, 34, SSD1306_WHITE);
  displayTela.drawPixel(23, 34, SSD1306_WHITE);
  displayTela.drawPixel(22, 34, SSD1306_WHITE);
  displayTela.drawPixel(21, 34, SSD1306_WHITE);
  displayTela.drawPixel(20, 34, SSD1306_WHITE);
  displayTela.drawPixel(19, 34, SSD1306_WHITE);
  displayTela.drawPixel(18, 34, SSD1306_WHITE);
  displayTela.drawPixel(17, 34, SSD1306_WHITE);
  displayTela.drawPixel(16, 34, SSD1306_WHITE);
  displayTela.drawPixel(15, 34, SSD1306_WHITE);
  displayTela.drawPixel(14, 34, SSD1306_WHITE);
  displayTela.drawPixel(13, 34, SSD1306_WHITE);
  displayTela.drawPixel(12, 34, SSD1306_WHITE);
  displayTela.drawPixel(11, 34, SSD1306_WHITE);
  displayTela.drawPixel(10, 34, SSD1306_WHITE);
  displayTela.drawPixel(10, 33, SSD1306_WHITE);
  displayTela.drawPixel(10, 32, SSD1306_WHITE);
  displayTela.drawPixel(10, 31, SSD1306_WHITE);
  displayTela.drawPixel(11, 31, SSD1306_WHITE);
  displayTela.drawPixel(12, 31, SSD1306_WHITE);
  displayTela.drawPixel(13, 31, SSD1306_WHITE);
  displayTela.drawPixel(14, 31, SSD1306_WHITE);
  displayTela.drawPixel(15, 31, SSD1306_WHITE);
  displayTela.drawPixel(16, 31, SSD1306_WHITE);
  displayTela.drawPixel(17, 31, SSD1306_WHITE);
  displayTela.drawPixel(18, 31, SSD1306_WHITE);
  displayTela.drawPixel(19, 31, SSD1306_WHITE);
  displayTela.drawPixel(20, 31, SSD1306_WHITE);
  displayTela.drawPixel(21, 31, SSD1306_WHITE);
  displayTela.drawPixel(22, 31, SSD1306_WHITE);
  displayTela.drawPixel(23, 31, SSD1306_WHITE);
  displayTela.drawPixel(24, 31, SSD1306_WHITE);
  displayTela.drawPixel(25, 31, SSD1306_WHITE);
  displayTela.drawPixel(26, 31, SSD1306_WHITE);
  displayTela.drawPixel(27, 31, SSD1306_WHITE);
  displayTela.drawPixel(28, 31, SSD1306_WHITE);
  displayTela.drawPixel(29, 31, SSD1306_WHITE);
  displayTela.drawPixel(30, 31, SSD1306_WHITE);
  displayTela.drawPixel(31, 31, SSD1306_WHITE);
  displayTela.drawPixel(32, 31, SSD1306_WHITE);
  displayTela.drawPixel(33, 31, SSD1306_WHITE);
  displayTela.drawPixel(34, 31, SSD1306_WHITE);
  displayTela.drawPixel(35, 31, SSD1306_WHITE);
  displayTela.drawPixel(36, 31, SSD1306_WHITE);
  displayTela.drawPixel(37, 31, SSD1306_WHITE);
  displayTela.drawPixel(38, 31, SSD1306_WHITE);
  displayTela.drawPixel(39, 31, SSD1306_WHITE);
  displayTela.drawPixel(40, 31, SSD1306_WHITE);
  displayTela.drawPixel(41, 31, SSD1306_WHITE);
  displayTela.drawPixel(42, 31, SSD1306_WHITE);
  displayTela.drawPixel(43, 31, SSD1306_WHITE);
  displayTela.drawPixel(44, 31, SSD1306_WHITE);
  displayTela.drawPixel(45, 31, SSD1306_WHITE);
  displayTela.drawPixel(46, 31, SSD1306_WHITE);
  displayTela.drawPixel(47, 31, SSD1306_WHITE);
  displayTela.drawPixel(53, 31, SSD1306_WHITE);
  displayTela.drawPixel(55, 34, SSD1306_WHITE);
  displayTela.drawPixel(56, 34, SSD1306_WHITE);
  displayTela.drawPixel(57, 34, SSD1306_WHITE);
  displayTela.drawPixel(54, 34, SSD1306_WHITE);
  displayTela.drawPixel(53, 34, SSD1306_WHITE);
  displayTela.drawPixel(52, 34, SSD1306_WHITE);
  displayTela.drawPixel(51, 34, SSD1306_WHITE);
  displayTela.drawPixel(50, 34, SSD1306_WHITE);
  displayTela.drawPixel(49, 34, SSD1306_WHITE);
  displayTela.drawPixel(48, 34, SSD1306_WHITE);
  displayTela.drawPixel(47, 34, SSD1306_WHITE);
  displayTela.drawPixel(46, 34, SSD1306_WHITE);
  displayTela.drawPixel(45, 34, SSD1306_WHITE);
  displayTela.drawPixel(44, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 34, SSD1306_WHITE);
  displayTela.drawPixel(42, 34, SSD1306_WHITE);
  displayTela.drawPixel(41, 34, SSD1306_WHITE);
  displayTela.drawPixel(40, 34, SSD1306_WHITE);
  displayTela.drawPixel(39, 34, SSD1306_WHITE);
  displayTela.drawPixel(38, 33, SSD1306_WHITE);
  displayTela.drawPixel(38, 32, SSD1306_WHITE);
  displayTela.drawPixel(37, 32, SSD1306_WHITE);
  displayTela.drawPixel(36, 32, SSD1306_WHITE);
  displayTela.drawPixel(35, 32, SSD1306_WHITE);
  displayTela.drawPixel(34, 32, SSD1306_WHITE);
  displayTela.drawPixel(33, 32, SSD1306_WHITE);
  displayTela.drawPixel(32, 32, SSD1306_WHITE);
  displayTela.drawPixel(31, 32, SSD1306_WHITE);
  displayTela.drawPixel(31, 33, SSD1306_WHITE);
  displayTela.drawPixel(30, 33, SSD1306_WHITE);
  displayTela.drawPixel(29, 33, SSD1306_WHITE);
  displayTela.drawPixel(28, 33, SSD1306_WHITE);
  displayTela.drawPixel(27, 33, SSD1306_WHITE);
  displayTela.drawPixel(26, 33, SSD1306_WHITE);
  displayTela.drawPixel(9, 33, SSD1306_WHITE);
  displayTela.drawPixel(9, 32, SSD1306_WHITE);
  displayTela.drawPixel(11, 32, SSD1306_WHITE);
  displayTela.drawPixel(12, 32, SSD1306_WHITE);
  displayTela.drawPixel(13, 32, SSD1306_WHITE);
  displayTela.drawPixel(14, 32, SSD1306_WHITE);
  displayTela.drawPixel(15, 32, SSD1306_WHITE);
  displayTela.drawPixel(16, 32, SSD1306_WHITE);
  displayTela.drawPixel(17, 32, SSD1306_WHITE);
  displayTela.drawPixel(18, 32, SSD1306_WHITE);
  displayTela.drawPixel(19, 32, SSD1306_WHITE);
  displayTela.drawPixel(20, 32, SSD1306_WHITE);
  displayTela.drawPixel(21, 32, SSD1306_WHITE);
  displayTela.drawPixel(22, 32, SSD1306_WHITE);
  displayTela.drawPixel(23, 32, SSD1306_WHITE);
  displayTela.drawPixel(24, 32, SSD1306_WHITE);
  displayTela.drawPixel(25, 32, SSD1306_WHITE);
  displayTela.drawPixel(26, 32, SSD1306_WHITE);
  displayTela.drawPixel(27, 32, SSD1306_WHITE);
  displayTela.drawPixel(28, 32, SSD1306_WHITE);
  displayTela.drawPixel(29, 32, SSD1306_WHITE);
  displayTela.drawPixel(30, 32, SSD1306_WHITE);
  displayTela.drawPixel(39, 32, SSD1306_WHITE);
  displayTela.drawPixel(40, 32, SSD1306_WHITE);
  displayTela.drawPixel(41, 32, SSD1306_WHITE);
  displayTela.drawPixel(51, 32, SSD1306_WHITE);
  displayTela.drawPixel(50, 32, SSD1306_WHITE);
  displayTela.drawPixel(49, 32, SSD1306_WHITE);
  displayTela.drawPixel(48, 32, SSD1306_WHITE);
  displayTela.drawPixel(47, 32, SSD1306_WHITE);
  displayTela.drawPixel(46, 32, SSD1306_WHITE);
  displayTela.drawPixel(45, 32, SSD1306_WHITE);
  displayTela.drawPixel(44, 32, SSD1306_WHITE);
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  displayTela.drawPixel(42, 32, SSD1306_WHITE);
  displayTela.drawPixel(32, 33, SSD1306_WHITE);
  displayTela.drawPixel(25, 33, SSD1306_WHITE);
  displayTela.drawPixel(24, 33, SSD1306_WHITE);
  displayTela.drawPixel(23, 33, SSD1306_WHITE);
  displayTela.drawPixel(22, 33, SSD1306_WHITE);
  displayTela.drawPixel(21, 33, SSD1306_WHITE);
  displayTela.drawPixel(20, 33, SSD1306_WHITE);
  displayTela.drawPixel(19, 33, SSD1306_WHITE);
  displayTela.drawPixel(18, 33, SSD1306_WHITE);
  displayTela.drawPixel(17, 33, SSD1306_WHITE);
  displayTela.drawPixel(16, 33, SSD1306_WHITE);
  displayTela.drawPixel(15, 33, SSD1306_WHITE);
  displayTela.drawPixel(14, 33, SSD1306_WHITE);
  displayTela.drawPixel(13, 33, SSD1306_WHITE);
  displayTela.drawPixel(12, 33, SSD1306_WHITE);
  displayTela.drawPixel(11, 33, SSD1306_WHITE);
  displayTela.drawPixel(33, 33, SSD1306_WHITE);
  displayTela.drawPixel(34, 33, SSD1306_WHITE);
  displayTela.drawPixel(35, 33, SSD1306_WHITE);
  displayTela.drawPixel(36, 33, SSD1306_WHITE);
  displayTela.drawPixel(37, 33, SSD1306_WHITE);
  //id: 111 pixel 157 
  displayTela.drawPixel(9, 35, SSD1306_WHITE);
  displayTela.drawPixel(9, 34, SSD1306_WHITE);
  displayTela.drawPixel(9, 33, SSD1306_WHITE);
  displayTela.drawPixel(9, 32, SSD1306_WHITE);
  displayTela.drawPixel(9, 31, SSD1306_WHITE);
  displayTela.drawPixel(9, 30, SSD1306_WHITE);
  displayTela.drawPixel(10, 30, SSD1306_WHITE);
  displayTela.drawPixel(10, 29, SSD1306_WHITE);
  displayTela.drawPixel(10, 28, SSD1306_WHITE);
  displayTela.drawPixel(10, 27, SSD1306_WHITE);
  displayTela.drawPixel(10, 26, SSD1306_WHITE);
  displayTela.drawPixel(11, 26, SSD1306_WHITE);
  displayTela.drawPixel(11, 25, SSD1306_WHITE);
  displayTela.drawPixel(11, 24, SSD1306_WHITE);
  displayTela.drawPixel(12, 24, SSD1306_WHITE);
  displayTela.drawPixel(12, 23, SSD1306_WHITE);
  displayTela.drawPixel(12, 22, SSD1306_WHITE);
  displayTela.drawPixel(13, 22, SSD1306_WHITE);
  displayTela.drawPixel(13, 21, SSD1306_WHITE);
  displayTela.drawPixel(13, 20, SSD1306_WHITE);
  displayTela.drawPixel(14, 20, SSD1306_WHITE);
  displayTela.drawPixel(14, 19, SSD1306_WHITE);
  displayTela.drawPixel(15, 19, SSD1306_WHITE);
  displayTela.drawPixel(15, 18, SSD1306_WHITE);
  displayTela.drawPixel(15, 17, SSD1306_WHITE);
  displayTela.drawPixel(16, 17, SSD1306_WHITE);
  displayTela.drawPixel(16, 16, SSD1306_WHITE);
  displayTela.drawPixel(17, 16, SSD1306_WHITE);
  displayTela.drawPixel(17, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 15, SSD1306_WHITE);
  displayTela.drawPixel(18, 14, SSD1306_WHITE);
  displayTela.drawPixel(19, 14, SSD1306_WHITE);
  displayTela.drawPixel(19, 13, SSD1306_WHITE);
  displayTela.drawPixel(20, 13, SSD1306_WHITE);
  displayTela.drawPixel(20, 12, SSD1306_WHITE);
  displayTela.drawPixel(21, 12, SSD1306_WHITE);
  displayTela.drawPixel(21, 11, SSD1306_WHITE);
  displayTela.drawPixel(22, 11, SSD1306_WHITE);
  displayTela.drawPixel(23, 11, SSD1306_WHITE);
  displayTela.drawPixel(23, 10, SSD1306_WHITE);
  displayTela.drawPixel(24, 10, SSD1306_WHITE);
  displayTela.drawPixel(25, 10, SSD1306_WHITE);
  displayTela.drawPixel(26, 10, SSD1306_WHITE);
  //id: 112 pixel 158 
  displayTela.drawPixel(34, 29, SSD1306_WHITE);
  displayTela.drawPixel(34, 30, SSD1306_WHITE);
  displayTela.drawPixel(34, 31, SSD1306_WHITE);
  displayTela.drawPixel(34, 32, SSD1306_WHITE);
  displayTela.drawPixel(34, 33, SSD1306_WHITE);
  displayTela.drawPixel(34, 34, SSD1306_WHITE);
  displayTela.drawPixel(34, 35, SSD1306_WHITE);
  displayTela.drawPixel(34, 36, SSD1306_WHITE);
  displayTela.drawPixel(34, 37, SSD1306_WHITE);
  displayTela.drawPixel(34, 38, SSD1306_WHITE);
  displayTela.drawPixel(34, 39, SSD1306_WHITE);
  displayTela.drawPixel(34, 40, SSD1306_WHITE);
  displayTela.drawPixel(34, 41, SSD1306_WHITE);
  displayTela.drawPixel(34, 42, SSD1306_WHITE);
  displayTela.drawPixel(34, 43, SSD1306_WHITE);
  displayTela.drawPixel(34, 44, SSD1306_WHITE);
  displayTela.drawPixel(34, 45, SSD1306_WHITE);
  displayTela.drawPixel(34, 46, SSD1306_WHITE);
  displayTela.drawPixel(34, 47, SSD1306_WHITE);
  displayTela.drawPixel(34, 48, SSD1306_WHITE);
  displayTela.drawPixel(35, 48, SSD1306_WHITE);
  displayTela.drawPixel(35, 49, SSD1306_WHITE);
  displayTela.drawPixel(35, 50, SSD1306_WHITE);
  displayTela.drawPixel(35, 51, SSD1306_WHITE);
  displayTela.drawPixel(35, 52, SSD1306_WHITE);
  displayTela.drawPixel(35, 53, SSD1306_WHITE);
  displayTela.drawPixel(34, 53, SSD1306_WHITE);
  displayTela.drawPixel(34, 54, SSD1306_WHITE);
  displayTela.drawPixel(34, 55, SSD1306_WHITE);
  displayTela.drawPixel(33, 55, SSD1306_WHITE);
  displayTela.drawPixel(33, 56, SSD1306_WHITE);
  displayTela.drawPixel(32, 56, SSD1306_WHITE);
  displayTela.drawPixel(32, 57, SSD1306_WHITE);
  displayTela.drawPixel(31, 57, SSD1306_WHITE);
  displayTela.drawPixel(31, 58, SSD1306_WHITE);
  displayTela.drawPixel(30, 58, SSD1306_WHITE);
  displayTela.drawPixel(29, 58, SSD1306_WHITE);
  displayTela.drawPixel(29, 59, SSD1306_WHITE);
  displayTela.drawPixel(28, 59, SSD1306_WHITE);
  displayTela.drawPixel(27, 59, SSD1306_WHITE);
  displayTela.drawPixel(26, 59, SSD1306_WHITE);
  displayTela.drawPixel(25, 58, SSD1306_WHITE);
  displayTela.drawPixel(24, 58, SSD1306_WHITE);
  displayTela.drawPixel(23, 58, SSD1306_WHITE);
  displayTela.drawPixel(23, 57, SSD1306_WHITE);
  displayTela.drawPixel(22, 57, SSD1306_WHITE);
  displayTela.drawPixel(22, 56, SSD1306_WHITE);
  displayTela.drawPixel(21, 56, SSD1306_WHITE);
  displayTela.drawPixel(21, 55, SSD1306_WHITE);
  displayTela.drawPixel(21, 54, SSD1306_WHITE);
  displayTela.drawPixel(20, 54, SSD1306_WHITE);
  displayTela.drawPixel(20, 53, SSD1306_WHITE);
  displayTela.drawPixel(20, 52, SSD1306_WHITE);
  displayTela.drawPixel(20, 51, SSD1306_WHITE);
  displayTela.drawPixel(21, 50, SSD1306_WHITE);
  displayTela.drawPixel(21, 49, SSD1306_WHITE);
  displayTela.drawPixel(22, 49, SSD1306_WHITE);
  displayTela.drawPixel(22, 48, SSD1306_WHITE);
  displayTela.drawPixel(23, 48, SSD1306_WHITE);
  displayTela.drawPixel(23, 47, SSD1306_WHITE);
  displayTela.drawPixel(21, 48, SSD1306_WHITE);
  displayTela.drawPixel(21, 51, SSD1306_WHITE);
  displayTela.drawPixel(21, 52, SSD1306_WHITE);
  displayTela.drawPixel(21, 53, SSD1306_WHITE);
  displayTela.drawPixel(22, 55, SSD1306_WHITE);
  displayTela.drawPixel(23, 56, SSD1306_WHITE);
  displayTela.drawPixel(24, 57, SSD1306_WHITE);
  displayTela.drawPixel(25, 57, SSD1306_WHITE);
  displayTela.drawPixel(26, 58, SSD1306_WHITE);
  displayTela.drawPixel(27, 58, SSD1306_WHITE);
  displayTela.drawPixel(28, 58, SSD1306_WHITE);
  displayTela.drawPixel(33, 54, SSD1306_WHITE);
  displayTela.drawPixel(33, 53, SSD1306_WHITE);
  displayTela.drawPixel(34, 52, SSD1306_WHITE);
  displayTela.drawPixel(34, 51, SSD1306_WHITE);
  displayTela.drawPixel(34, 50, SSD1306_WHITE);
  displayTela.drawPixel(34, 49, SSD1306_WHITE);
  displayTela.drawPixel(35, 47, SSD1306_WHITE);
  displayTela.drawPixel(35, 46, SSD1306_WHITE);
  displayTela.drawPixel(35, 45, SSD1306_WHITE);
  displayTela.drawPixel(35, 44, SSD1306_WHITE);
  displayTela.drawPixel(35, 43, SSD1306_WHITE);
  displayTela.drawPixel(35, 42, SSD1306_WHITE);
  displayTela.drawPixel(35, 41, SSD1306_WHITE);
  displayTela.drawPixel(35, 40, SSD1306_WHITE);
  displayTela.drawPixel(35, 39, SSD1306_WHITE);
  displayTela.drawPixel(35, 38, SSD1306_WHITE);
  displayTela.drawPixel(35, 37, SSD1306_WHITE);
  displayTela.drawPixel(35, 36, SSD1306_WHITE);
  displayTela.drawPixel(35, 35, SSD1306_WHITE);
  displayTela.drawPixel(33, 35, SSD1306_WHITE);
  displayTela.drawPixel(33, 36, SSD1306_WHITE);
  displayTela.drawPixel(33, 37, SSD1306_WHITE);
  displayTela.drawPixel(33, 38, SSD1306_WHITE);
  displayTela.drawPixel(33, 39, SSD1306_WHITE);
  displayTela.drawPixel(32, 37, SSD1306_WHITE);
  displayTela.drawPixel(32, 36, SSD1306_WHITE);
  displayTela.drawPixel(32, 35, SSD1306_WHITE);
  displayTela.drawPixel(32, 38, SSD1306_WHITE);
  displayTela.drawPixel(32, 39, SSD1306_WHITE);
  displayTela.drawPixel(32, 40, SSD1306_WHITE);
  displayTela.drawPixel(32, 41, SSD1306_WHITE);
  displayTela.drawPixel(33, 41, SSD1306_WHITE);
  displayTela.drawPixel(33, 42, SSD1306_WHITE);
  displayTela.drawPixel(33, 43, SSD1306_WHITE);
  displayTela.drawPixel(33, 44, SSD1306_WHITE);
  displayTela.drawPixel(33, 40, SSD1306_WHITE);
  displayTela.drawPixel(32, 42, SSD1306_WHITE);
  displayTela.drawPixel(32, 43, SSD1306_WHITE);
  displayTela.drawPixel(32, 44, SSD1306_WHITE);
  displayTela.drawPixel(32, 45, SSD1306_WHITE);
  displayTela.drawPixel(33, 45, SSD1306_WHITE);
  displayTela.drawPixel(33, 46, SSD1306_WHITE);
  displayTela.drawPixel(33, 47, SSD1306_WHITE);
  displayTela.drawPixel(33, 48, SSD1306_WHITE);
  displayTela.drawPixel(33, 49, SSD1306_WHITE);
  displayTela.drawPixel(33, 52, SSD1306_WHITE);
  displayTela.drawPixel(32, 54, SSD1306_WHITE);
  displayTela.drawPixel(32, 55, SSD1306_WHITE);
  displayTela.drawPixel(31, 56, SSD1306_WHITE);
  displayTela.drawPixel(30, 57, SSD1306_WHITE);
  displayTela.drawPixel(29, 57, SSD1306_WHITE);
  displayTela.drawPixel(28, 57, SSD1306_WHITE);
  displayTela.drawPixel(27, 57, SSD1306_WHITE);
  displayTela.drawPixel(26, 57, SSD1306_WHITE);
  displayTela.drawPixel(25, 56, SSD1306_WHITE);
  displayTela.drawPixel(24, 56, SSD1306_WHITE);
  displayTela.drawPixel(24, 55, SSD1306_WHITE);
  displayTela.drawPixel(23, 55, SSD1306_WHITE);
  displayTela.drawPixel(23, 54, SSD1306_WHITE);
  displayTela.drawPixel(22, 54, SSD1306_WHITE);
  displayTela.drawPixel(22, 53, SSD1306_WHITE);
  displayTela.drawPixel(23, 49, SSD1306_WHITE);
  displayTela.drawPixel(24, 48, SSD1306_WHITE);
  displayTela.drawPixel(24, 49, SSD1306_WHITE);
  displayTela.drawPixel(25, 49, SSD1306_WHITE);
  displayTela.drawPixel(23, 50, SSD1306_WHITE);
  displayTela.drawPixel(22, 50, SSD1306_WHITE);
  displayTela.drawPixel(22, 51, SSD1306_WHITE);
  displayTela.drawPixel(22, 52, SSD1306_WHITE);
  displayTela.drawPixel(24, 50, SSD1306_WHITE);
  displayTela.drawPixel(25, 50, SSD1306_WHITE);
  displayTela.drawPixel(26, 50, SSD1306_WHITE);
  displayTela.drawPixel(27, 50, SSD1306_WHITE);
  displayTela.drawPixel(27, 49, SSD1306_WHITE);
  displayTela.drawPixel(28, 50, SSD1306_WHITE);
  displayTela.drawPixel(27, 48, SSD1306_WHITE);
  displayTela.drawPixel(27, 47, SSD1306_WHITE);
  displayTela.drawPixel(27, 46, SSD1306_WHITE);
  displayTela.drawPixel(26, 46, SSD1306_WHITE);
  displayTela.drawPixel(25, 46, SSD1306_WHITE);
  displayTela.drawPixel(25, 47, SSD1306_WHITE);
  displayTela.drawPixel(24, 47, SSD1306_WHITE);
  displayTela.drawPixel(25, 48, SSD1306_WHITE);
  displayTela.drawPixel(26, 49, SSD1306_WHITE);
  displayTela.drawPixel(26, 48, SSD1306_WHITE);
  displayTela.drawPixel(26, 47, SSD1306_WHITE);
  displayTela.drawPixel(26, 51, SSD1306_WHITE);
  displayTela.drawPixel(25, 51, SSD1306_WHITE);
  displayTela.drawPixel(27, 51, SSD1306_WHITE);
  //id: 113 pixel 159 
  displayTela.drawPixel(27, 46, SSD1306_BLACK);
  displayTela.drawPixel(26, 46, SSD1306_BLACK);
  displayTela.drawPixel(25, 46, SSD1306_BLACK);
  displayTela.drawPixel(24, 46, SSD1306_BLACK);
  displayTela.drawPixel(23, 46, SSD1306_BLACK);
  displayTela.drawPixel(23, 45, SSD1306_BLACK);
  displayTela.drawPixel(24, 45, SSD1306_BLACK);
  displayTela.drawPixel(25, 45, SSD1306_BLACK);
  displayTela.drawPixel(26, 47, SSD1306_BLACK);
  displayTela.drawPixel(27, 48, SSD1306_BLACK);
  displayTela.drawPixel(27, 49, SSD1306_BLACK);
  displayTela.drawPixel(28, 49, SSD1306_BLACK);
  displayTela.drawPixel(28, 48, SSD1306_BLACK);
  displayTela.drawPixel(28, 47, SSD1306_BLACK);
  displayTela.drawPixel(27, 47, SSD1306_BLACK);
  displayTela.drawPixel(27, 45, SSD1306_BLACK);
  //id: 114 pixel 160 
  displayTela.drawPixel(28, 50, SSD1306_BLACK);
  //id: 115 pixel 161 
  displayTela.drawPixel(38, 3, SSD1306_BLACK);
  displayTela.drawPixel(37, 3, SSD1306_BLACK);
  displayTela.drawPixel(36, 3, SSD1306_BLACK);
  displayTela.drawPixel(35, 3, SSD1306_BLACK);
  //id: 116 pixel 162 
  displayTela.drawPixel(34, 8, SSD1306_WHITE);
  displayTela.drawPixel(33, 8, SSD1306_WHITE);
  //id: 117 pixel 163 
  displayTela.drawPixel(29, 8, SSD1306_WHITE);
  //id: 118 pixel 164 
  displayTela.drawPixel(38, 8, SSD1306_WHITE);
  //id: 119 pixel 165 
  displayTela.drawPixel(34, 7, SSD1306_WHITE);
  displayTela.drawPixel(33, 7, SSD1306_WHITE);
  //id: 120 pixel 166 
  displayTela.drawPixel(28, 8, SSD1306_BLACK);
  //id: 121 pixel 167 
  displayTela.drawPixel(28, 8, SSD1306_BLACK);
  //id: 122 pixel 168 
  displayTela.drawPixel(28, 8, SSD1306_BLACK);
  //id: 123 pixel 169 
  displayTela.drawPixel(29, 8, SSD1306_BLACK);
  //id: 124 pixel 170 
  displayTela.drawPixel(38, 7, SSD1306_BLACK);
  //id: 125 pixel 171 
  displayTela.drawPixel(38, 8, SSD1306_BLACK);
  //id: 126 pixel 172 
  displayTela.drawPixel(38, 8, SSD1306_BLACK);
  //id: 127 pixel 173 
  displayTela.drawPixel(32, 7, SSD1306_WHITE);
  //id: 128 pixel 174 
  displayTela.drawPixel(32, 7, SSD1306_WHITE);
  //id: 129 pixel 175 
  displayTela.drawPixel(35, 7, SSD1306_WHITE);
  //id: 130 pixel 176 
  displayTela.drawPixel(32, 8, SSD1306_WHITE);
  //id: 131 pixel 177 
  displayTela.drawPixel(35, 8, SSD1306_WHITE);
  //id: 132 pixel 178 
  displayTela.drawPixel(32, 7, SSD1306_BLACK);
  //id: 133 pixel 179 
  displayTela.drawPixel(35, 6, SSD1306_BLACK);
  //id: 134 pixel 180 
  displayTela.drawPixel(35, 7, SSD1306_BLACK);
  //id: 135 pixel 181 
  displayTela.drawPixel(32, 45, SSD1306_WHITE);
  displayTela.drawPixel(32, 46, SSD1306_WHITE);
  displayTela.drawPixel(32, 47, SSD1306_WHITE);
  displayTela.drawPixel(32, 48, SSD1306_WHITE);
  displayTela.drawPixel(32, 49, SSD1306_WHITE);
  displayTela.drawPixel(32, 50, SSD1306_WHITE);
  displayTela.drawPixel(32, 51, SSD1306_WHITE);
  displayTela.drawPixel(33, 50, SSD1306_WHITE);
  displayTela.drawPixel(33, 51, SSD1306_WHITE);
  //id: 136 pixel 182 
  displayTela.drawPixel(33, 51, SSD1306_WHITE);
  //id: 137 pixel 183 
  displayTela.drawPixel(23, 51, SSD1306_WHITE);
  displayTela.drawPixel(23, 52, SSD1306_WHITE);
  displayTela.drawPixel(23, 53, SSD1306_WHITE);
  //id: 138 pixel 184 
  displayTela.drawPixel(33, 52, SSD1306_WHITE);
  displayTela.drawPixel(33, 53, SSD1306_WHITE);
  displayTela.drawPixel(33, 54, SSD1306_WHITE);
  displayTela.drawPixel(32, 54, SSD1306_WHITE);
  displayTela.drawPixel(32, 55, SSD1306_WHITE);
  displayTela.drawPixel(32, 53, SSD1306_WHITE);
  displayTela.drawPixel(32, 52, SSD1306_WHITE);
  displayTela.drawPixel(33, 51, SSD1306_WHITE);
  displayTela.drawPixel(32, 51, SSD1306_WHITE);
  displayTela.drawPixel(31, 53, SSD1306_WHITE);
  displayTela.drawPixel(31, 54, SSD1306_WHITE);
  displayTela.drawPixel(30, 55, SSD1306_WHITE);
  displayTela.drawPixel(29, 55, SSD1306_WHITE);
  displayTela.drawPixel(28, 55, SSD1306_WHITE);
  displayTela.drawPixel(27, 55, SSD1306_WHITE);
  displayTela.drawPixel(26, 55, SSD1306_WHITE);
  displayTela.drawPixel(26, 56, SSD1306_WHITE);
  displayTela.drawPixel(27, 56, SSD1306_WHITE);
  displayTela.drawPixel(28, 56, SSD1306_WHITE);
  displayTela.drawPixel(29, 56, SSD1306_WHITE);
  displayTela.drawPixel(30, 56, SSD1306_WHITE);
  displayTela.drawPixel(31, 56, SSD1306_WHITE);
  displayTela.drawPixel(31, 55, SSD1306_WHITE);
  displayTela.drawPixel(25, 56, SSD1306_WHITE);
  displayTela.drawPixel(25, 55, SSD1306_WHITE);
  displayTela.drawPixel(24, 55, SSD1306_WHITE);
  displayTela.drawPixel(24, 54, SSD1306_WHITE);
  displayTela.drawPixel(24, 53, SSD1306_WHITE);
  displayTela.drawPixel(24, 52, SSD1306_WHITE);
  displayTela.drawPixel(24, 51, SSD1306_WHITE);
  displayTela.drawPixel(24, 50, SSD1306_WHITE);
  displayTela.drawPixel(24, 49, SSD1306_WHITE);
  displayTela.drawPixel(24, 48, SSD1306_WHITE);
  displayTela.drawPixel(23, 50, SSD1306_WHITE);
  displayTela.drawPixel(23, 51, SSD1306_WHITE);
  displayTela.drawPixel(23, 52, SSD1306_WHITE);
  displayTela.drawPixel(22, 52, SSD1306_WHITE);
  displayTela.drawPixel(22, 53, SSD1306_WHITE);
  displayTela.drawPixel(22, 54, SSD1306_WHITE);
  displayTela.drawPixel(22, 55, SSD1306_WHITE);
  displayTela.drawPixel(22, 56, SSD1306_WHITE);
  displayTela.drawPixel(23, 56, SSD1306_WHITE);
  displayTela.drawPixel(24, 56, SSD1306_WHITE);
  displayTela.drawPixel(25, 54, SSD1306_WHITE);
  displayTela.drawPixel(23, 55, SSD1306_WHITE);
  //id: 139 pixel 185 
  displayTela.drawPixel(27, 50, SSD1306_BLACK);
  displayTela.drawPixel(27, 51, SSD1306_BLACK);
  displayTela.drawPixel(26, 51, SSD1306_BLACK);
  displayTela.drawPixel(25, 51, SSD1306_BLACK);
  displayTela.drawPixel(25, 50, SSD1306_BLACK);
  displayTela.drawPixel(25, 49, SSD1306_BLACK);
  displayTela.drawPixel(25, 48, SSD1306_BLACK);
  displayTela.drawPixel(26, 48, SSD1306_BLACK);
  displayTela.drawPixel(26, 47, SSD1306_BLACK);
  displayTela.drawPixel(26, 46, SSD1306_BLACK);
  displayTela.drawPixel(26, 49, SSD1306_BLACK);
  displayTela.drawPixel(25, 47, SSD1306_BLACK);
  displayTela.drawPixel(26, 50, SSD1306_BLACK);
  displayTela.drawPixel(24, 47, SSD1306_BLACK);
  displayTela.drawPixel(24, 50, SSD1306_BLACK);
  displayTela.drawPixel(24, 49, SSD1306_BLACK);
  displayTela.drawPixel(24, 48, SSD1306_BLACK);
  displayTela.drawPixel(24, 46, SSD1306_BLACK);
  displayTela.drawPixel(23, 46, SSD1306_BLACK);
  displayTela.drawPixel(23, 45, SSD1306_BLACK);
  displayTela.drawPixel(23, 47, SSD1306_BLACK);
  displayTela.drawPixel(27, 52, SSD1306_BLACK);
  displayTela.drawPixel(28, 52, SSD1306_BLACK);
  displayTela.drawPixel(29, 52, SSD1306_BLACK);
  displayTela.drawPixel(29, 51, SSD1306_BLACK);
  displayTela.drawPixel(29, 50, SSD1306_BLACK);
  displayTela.drawPixel(30, 50, SSD1306_BLACK);
  displayTela.drawPixel(30, 51, SSD1306_BLACK);
  displayTela.drawPixel(30, 52, SSD1306_BLACK);
  displayTela.drawPixel(26, 52, SSD1306_BLACK);
  displayTela.drawPixel(26, 53, SSD1306_BLACK);
  displayTela.drawPixel(26, 54, SSD1306_BLACK);
  displayTela.drawPixel(26, 55, SSD1306_BLACK);
  displayTela.drawPixel(27, 55, SSD1306_BLACK);
  displayTela.drawPixel(28, 55, SSD1306_BLACK);
  displayTela.drawPixel(29, 55, SSD1306_BLACK);
  displayTela.drawPixel(29, 54, SSD1306_BLACK);
  displayTela.drawPixel(30, 54, SSD1306_BLACK);
  displayTela.drawPixel(30, 53, SSD1306_BLACK);
  displayTela.drawPixel(30, 55, SSD1306_BLACK);
  displayTela.drawPixel(28, 56, SSD1306_BLACK);
  displayTela.drawPixel(27, 56, SSD1306_BLACK);
  displayTela.drawPixel(27, 54, SSD1306_BLACK);
  displayTela.drawPixel(27, 53, SSD1306_BLACK);
  displayTela.drawPixel(25, 53, SSD1306_BLACK);
  displayTela.drawPixel(25, 52, SSD1306_BLACK);
  displayTela.drawPixel(24, 52, SSD1306_BLACK);
  displayTela.drawPixel(24, 51, SSD1306_BLACK);
  displayTela.drawPixel(25, 54, SSD1306_BLACK);
  displayTela.drawPixel(25, 55, SSD1306_BLACK);
  displayTela.drawPixel(24, 54, SSD1306_BLACK);
  displayTela.drawPixel(24, 53, SSD1306_BLACK);
  displayTela.drawPixel(23, 51, SSD1306_BLACK);
  displayTela.drawPixel(23, 52, SSD1306_BLACK);
  displayTela.drawPixel(23, 53, SSD1306_BLACK);
  displayTela.drawPixel(23, 54, SSD1306_BLACK);
  displayTela.drawPixel(23, 50, SSD1306_BLACK);
  displayTela.drawPixel(29, 53, SSD1306_BLACK);
  displayTela.drawPixel(31, 53, SSD1306_BLACK);
  displayTela.drawPixel(31, 52, SSD1306_BLACK);
  displayTela.drawPixel(31, 51, SSD1306_BLACK);
  //id: 140 pixel 186 
  displayTela.drawPixel(35, 53, SSD1306_WHITE);
  displayTela.drawPixel(35, 54, SSD1306_WHITE);
  displayTela.drawPixel(35, 55, SSD1306_WHITE);
  displayTela.drawPixel(34, 55, SSD1306_WHITE);
  displayTela.drawPixel(34, 56, SSD1306_WHITE);
  displayTela.drawPixel(34, 57, SSD1306_WHITE);
  displayTela.drawPixel(33, 57, SSD1306_WHITE);
  displayTela.drawPixel(33, 58, SSD1306_WHITE);
  displayTela.drawPixel(32, 58, SSD1306_WHITE);
  displayTela.drawPixel(31, 58, SSD1306_WHITE);
  displayTela.drawPixel(31, 59, SSD1306_WHITE);
  displayTela.drawPixel(30, 59, SSD1306_WHITE);
  displayTela.drawPixel(29, 59, SSD1306_WHITE);
  displayTela.drawPixel(28, 59, SSD1306_WHITE);
  displayTela.drawPixel(27, 59, SSD1306_WHITE);
  displayTela.drawPixel(26, 59, SSD1306_WHITE);
  //id: 141 pixel 187 
  displayTela.drawPixel(27, 59, SSD1306_WHITE);
  displayTela.drawPixel(26, 59, SSD1306_WHITE);
  displayTela.drawPixel(25, 59, SSD1306_WHITE);
  displayTela.drawPixel(25, 58, SSD1306_WHITE);
  displayTela.drawPixel(24, 58, SSD1306_WHITE);
  displayTela.drawPixel(24, 57, SSD1306_WHITE);
  displayTela.drawPixel(23, 57, SSD1306_WHITE);
  displayTela.drawPixel(22, 57, SSD1306_WHITE);
  displayTela.drawPixel(22, 56, SSD1306_WHITE);
  displayTela.drawPixel(22, 55, SSD1306_WHITE);
  displayTela.drawPixel(21, 55, SSD1306_WHITE);
  displayTela.drawPixel(21, 54, SSD1306_WHITE);
  displayTela.drawPixel(21, 53, SSD1306_WHITE);
  displayTela.drawPixel(21, 52, SSD1306_WHITE);
  displayTela.drawPixel(21, 51, SSD1306_WHITE);
  displayTela.drawPixel(21, 50, SSD1306_WHITE);
  displayTela.drawPixel(21, 49, SSD1306_WHITE);
  displayTela.drawPixel(20, 49, SSD1306_WHITE);
  //id: 142 pixel 188 
  displayTela.drawPixel(28, 60, SSD1306_WHITE);
  //id: 143 pixel 189 
  displayTela.drawPixel(21, 56, SSD1306_BLACK);
  //id: 144 pixel 190 
  displayTela.drawPixel(23, 54, SSD1306_WHITE);
  displayTela.drawPixel(24, 54, SSD1306_WHITE);
  //id: 145 pixel 191 
  displayTela.drawPixel(27, 59, SSD1306_WHITE);
  //id: 146 pixel 192 
  displayTela.drawPixel(27, 60, SSD1306_WHITE);
  //id: 147 pixel 193 
  displayTela.drawPixel(24, 59, SSD1306_WHITE);
  //id: 148 pixel 194 
  displayTela.drawPixel(22, 58, SSD1306_WHITE);
  //id: 149 pixel 195 
  displayTela.drawPixel(21, 56, SSD1306_WHITE);
  displayTela.drawPixel(21, 57, SSD1306_WHITE);
  //id: 150 pixel 196 
  displayTela.drawPixel(20, 55, SSD1306_WHITE);
  //id: 151 pixel 197 
  displayTela.drawPixel(23, 53, SSD1306_WHITE);
  displayTela.drawPixel(23, 52, SSD1306_WHITE);
  displayTela.drawPixel(23, 51, SSD1306_WHITE);
  displayTela.drawPixel(23, 50, SSD1306_WHITE);
  displayTela.drawPixel(23, 49, SSD1306_WHITE);
  displayTela.drawPixel(22, 49, SSD1306_WHITE);
  displayTela.drawPixel(21, 49, SSD1306_WHITE);
  displayTela.drawPixel(20, 49, SSD1306_WHITE);
  displayTela.drawPixel(20, 50, SSD1306_WHITE);
  displayTela.drawPixel(21, 48, SSD1306_WHITE);
  //id: 152 pixel 198 
  displayTela.drawPixel(20, 48, SSD1306_BLACK);
  //id: 153 pixel 199 
  displayTela.drawPixel(20, 49, SSD1306_BLACK);
  displayTela.drawPixel(20, 50, SSD1306_BLACK);
  //id: 154 pixel 200 
  displayTela.drawPixel(21, 48, SSD1306_BLACK);
  //id: 155 pixel 201 
  displayTela.drawPixel(121, 25, SSD1306_BLACK);
  displayTela.drawPixel(120, 25, SSD1306_BLACK);
  displayTela.drawPixel(119, 24, SSD1306_BLACK);
  displayTela.drawPixel(119, 23, SSD1306_BLACK);
  displayTela.drawPixel(118, 23, SSD1306_BLACK);
  displayTela.drawPixel(117, 21, SSD1306_BLACK);
  displayTela.drawPixel(115, 20, SSD1306_BLACK);
  displayTela.drawPixel(114, 19, SSD1306_BLACK);
  displayTela.drawPixel(109, 16, SSD1306_BLACK);
  displayTela.drawPixel(105, 15, SSD1306_BLACK);
  displayTela.drawPixel(96, 11, SSD1306_BLACK);
  displayTela.drawPixel(91, 10, SSD1306_BLACK);
  displayTela.drawPixel(86, 8, SSD1306_BLACK);
  displayTela.drawPixel(79, 5, SSD1306_BLACK);
  displayTela.drawPixel(77, 5, SSD1306_BLACK);
  displayTela.drawPixel(75, 4, SSD1306_BLACK);
  displayTela.drawPixel(72, 3, SSD1306_BLACK);
  displayTela.drawPixel(71, 2, SSD1306_BLACK);
  displayTela.drawPixel(69, 2, SSD1306_BLACK);
  displayTela.drawPixel(68, 1, SSD1306_BLACK);
  displayTela.drawPixel(66, 1, SSD1306_BLACK);
  displayTela.drawPixel(64, 1, SSD1306_BLACK);
  displayTela.drawPixel(62, 1, SSD1306_BLACK);
  displayTela.drawPixel(60, 1, SSD1306_BLACK);
  displayTela.drawPixel(57, 0, SSD1306_BLACK);
  displayTela.drawPixel(56, 0, SSD1306_BLACK);
  displayTela.drawPixel(54, 0, SSD1306_BLACK);
  displayTela.drawPixel(47, 0, SSD1306_BLACK);
  displayTela.drawPixel(46, 0, SSD1306_BLACK);
  displayTela.drawPixel(45, 0, SSD1306_BLACK);
  displayTela.drawPixel(44, 0, SSD1306_BLACK);
  displayTela.drawPixel(43, 0, SSD1306_BLACK);
  displayTela.drawPixel(42, 0, SSD1306_BLACK);
  displayTela.drawPixel(41, 0, SSD1306_BLACK);
  displayTela.drawPixel(40, 0, SSD1306_BLACK);
  displayTela.drawPixel(39, 0, SSD1306_BLACK);
  //id: 156 pixel 202 
  displayTela.drawPixel(29, 35, SSD1306_BLACK);
  displayTela.drawPixel(29, 34, SSD1306_BLACK);
  displayTela.drawPixel(28, 34, SSD1306_BLACK);
  displayTela.drawPixel(27, 34, SSD1306_BLACK);
  displayTela.drawPixel(27, 33, SSD1306_BLACK);
  displayTela.drawPixel(26, 33, SSD1306_BLACK);
  displayTela.drawPixel(26, 34, SSD1306_BLACK);
  displayTela.drawPixel(25, 34, SSD1306_BLACK);
  displayTela.drawPixel(26, 35, SSD1306_BLACK);
  displayTela.drawPixel(27, 35, SSD1306_BLACK);
  displayTela.drawPixel(25, 35, SSD1306_BLACK);
  displayTela.drawPixel(28, 35, SSD1306_BLACK);
  displayTela.drawPixel(24, 34, SSD1306_BLACK);
  displayTela.drawPixel(24, 35, SSD1306_BLACK);
  displayTela.drawPixel(23, 35, SSD1306_BLACK);
  //id: 157 pixel 203 
  displayTela.drawPixel(30, 35, SSD1306_BLACK);
  //id: 158 pixel 204 
  displayTela.drawPixel(22, 35, SSD1306_BLACK);
  //id: 159 pixel 205 
  displayTela.drawPixel(10, 35, SSD1306_BLACK);
  displayTela.drawPixel(11, 35, SSD1306_BLACK);
  displayTela.drawPixel(12, 35, SSD1306_BLACK);
  displayTela.drawPixel(13, 35, SSD1306_BLACK);
  displayTela.drawPixel(11, 34, SSD1306_BLACK);
  displayTela.drawPixel(12, 34, SSD1306_BLACK);
  //id: 160 pixel 206 
  displayTela.drawPixel(13, 34, SSD1306_BLACK);
  //id: 161 pixel 207 
  displayTela.drawPixel(13, 34, SSD1306_BLACK);
  //id: 162 pixel 208 
  displayTela.drawPixel(13, 34, SSD1306_BLACK);
  //id: 163 pixel 209 
  displayTela.drawPixel(13, 33, SSD1306_BLACK);
  displayTela.drawPixel(14, 33, SSD1306_BLACK);
  //id: 164 pixel 210 
  displayTela.drawPixel(14, 35, SSD1306_BLACK);
  //id: 165 pixel 211 
  displayTela.drawPixel(14, 34, SSD1306_BLACK);
  //id: 166 pixel 212 
  displayTela.drawPixel(14, 34, SSD1306_BLACK);
  displayTela.drawPixel(15, 34, SSD1306_BLACK);
  displayTela.drawPixel(15, 35, SSD1306_BLACK);
  displayTela.drawPixel(16, 35, SSD1306_BLACK);
  displayTela.drawPixel(16, 34, SSD1306_BLACK);
  //id: 167 pixel 213 
  displayTela.drawPixel(16, 35, SSD1306_BLACK);
  displayTela.drawPixel(17, 35, SSD1306_BLACK);
  //id: 168 pixel 214 
  displayTela.drawPixel(12, 33, SSD1306_BLACK);
  //id: 169 pixel 215 
  displayTela.drawPixel(14, 32, SSD1306_BLACK);
  //id: 170 pixel 216 
  displayTela.drawPixel(15, 33, SSD1306_BLACK);
  //id: 171 pixel 217 
  displayTela.drawPixel(15, 33, SSD1306_BLACK);
  //id: 172 pixel 218 
  displayTela.drawPixel(15, 32, SSD1306_BLACK);
  //id: 173 pixel 219 
  displayTela.drawPixel(15, 32, SSD1306_BLACK);
  displayTela.drawPixel(15, 33, SSD1306_BLACK);
  displayTela.drawPixel(16, 33, SSD1306_BLACK);
  displayTela.drawPixel(17, 33, SSD1306_BLACK);
  displayTela.drawPixel(17, 34, SSD1306_BLACK);
  displayTela.drawPixel(17, 35, SSD1306_BLACK);
  displayTela.drawPixel(18, 35, SSD1306_BLACK);
  displayTela.drawPixel(18, 34, SSD1306_BLACK);
  displayTela.drawPixel(19, 35, SSD1306_BLACK);
  //id: 174 pixel 220 
  displayTela.drawPixel(21, 35, SSD1306_BLACK);
  //id: 175 pixel 221 
  displayTela.drawPixel(22, 34, SSD1306_BLACK);
  displayTela.drawPixel(23, 34, SSD1306_BLACK);
  displayTela.drawPixel(24, 34, SSD1306_BLACK);
  //id: 176 pixel 222 
  displayTela.drawPixel(24, 34, SSD1306_BLACK);
  displayTela.drawPixel(24, 33, SSD1306_BLACK);
  displayTela.drawPixel(23, 33, SSD1306_BLACK);
  //id: 177 pixel 223 
  displayTela.drawPixel(23, 33, SSD1306_BLACK);
  //id: 178 pixel 224 
  displayTela.drawPixel(24, 33, SSD1306_BLACK);
  //id: 179 pixel 225 
  displayTela.drawPixel(24, 33, SSD1306_BLACK);
  displayTela.drawPixel(25, 33, SSD1306_BLACK);
  //id: 180 pixel 226 
  displayTela.drawPixel(25, 33, SSD1306_BLACK);
  //id: 181 pixel 227 
  displayTela.drawPixel(25, 33, SSD1306_BLACK);
  displayTela.drawPixel(25, 32, SSD1306_BLACK);
  //id: 182 pixel 228 
  displayTela.drawPixel(25, 32, SSD1306_BLACK);
  //id: 183 pixel 229 
  displayTela.drawPixel(25, 32, SSD1306_BLACK);
  displayTela.drawPixel(26, 32, SSD1306_BLACK);
  //id: 184 pixel 230 
  displayTela.drawPixel(28, 33, SSD1306_BLACK);
  //id: 185 pixel 231 
  displayTela.drawPixel(30, 34, SSD1306_BLACK);
  //id: 186 pixel 232 
  displayTela.drawPixel(27, 32, SSD1306_BLACK);
  //id: 187 pixel 233 
  displayTela.drawPixel(29, 33, SSD1306_BLACK);
  displayTela.drawPixel(28, 33, SSD1306_BLACK);
  //id: 188 pixel 234 
  displayTela.drawPixel(37, 35, SSD1306_BLACK);
  //id: 189 pixel 235 
  displayTela.drawPixel(37, 34, SSD1306_BLACK);
  displayTela.drawPixel(38, 34, SSD1306_BLACK);
  displayTela.drawPixel(38, 35, SSD1306_BLACK);
  displayTela.drawPixel(39, 35, SSD1306_BLACK);
  displayTela.drawPixel(39, 34, SSD1306_BLACK);
  displayTela.drawPixel(39, 33, SSD1306_BLACK);
  displayTela.drawPixel(38, 33, SSD1306_BLACK);
  //id: 190 pixel 236 
  displayTela.drawPixel(40, 32, SSD1306_BLACK);
  displayTela.drawPixel(40, 33, SSD1306_BLACK);
  displayTela.drawPixel(40, 34, SSD1306_BLACK);
  displayTela.drawPixel(40, 35, SSD1306_BLACK);
  //id: 191 pixel 237 
  displayTela.drawPixel(41, 33, SSD1306_BLACK);
  displayTela.drawPixel(41, 32, SSD1306_BLACK);
  //id: 192 pixel 238 
  displayTela.drawPixel(41, 32, SSD1306_BLACK);
  //id: 193 pixel 239 
  displayTela.drawPixel(41, 36, SSD1306_BLACK);
  displayTela.drawPixel(41, 35, SSD1306_BLACK);
  displayTela.drawPixel(41, 34, SSD1306_BLACK);
  displayTela.drawPixel(41, 33, SSD1306_BLACK);
  //id: 194 pixel 240 
  displayTela.drawPixel(41, 33, SSD1306_BLACK);
  displayTela.drawPixel(42, 33, SSD1306_BLACK);
  //id: 195 pixel 241 
  displayTela.drawPixel(42, 35, SSD1306_BLACK);
  //id: 196 pixel 242 
  displayTela.drawPixel(42, 35, SSD1306_BLACK);
  displayTela.drawPixel(42, 34, SSD1306_BLACK);
  //id: 197 pixel 243 
  displayTela.drawPixel(42, 34, SSD1306_BLACK);
  //id: 198 pixel 244 
  displayTela.drawPixel(42, 34, SSD1306_BLACK);
  //id: 199 pixel 245 
  displayTela.drawPixel(42, 32, SSD1306_BLACK);
  //id: 200 pixel 246 
  displayTela.drawPixel(43, 33, SSD1306_BLACK);
  displayTela.drawPixel(43, 34, SSD1306_BLACK);
  displayTela.drawPixel(43, 35, SSD1306_BLACK);
  displayTela.drawPixel(44, 34, SSD1306_BLACK);
  displayTela.drawPixel(44, 33, SSD1306_BLACK);
  displayTela.drawPixel(44, 35, SSD1306_BLACK);
  displayTela.drawPixel(45, 35, SSD1306_BLACK);
  displayTela.drawPixel(45, 34, SSD1306_BLACK);
  //id: 201 pixel 247 
  displayTela.drawPixel(45, 34, SSD1306_BLACK);
  displayTela.drawPixel(45, 35, SSD1306_BLACK);
  //id: 202 pixel 248 
  displayTela.drawPixel(47, 35, SSD1306_BLACK);
  displayTela.drawPixel(47, 34, SSD1306_BLACK);
  //id: 203 pixel 249 
  displayTela.drawPixel(48, 34, SSD1306_BLACK);
  displayTela.drawPixel(48, 35, SSD1306_BLACK);
  displayTela.drawPixel(49, 34, SSD1306_BLACK);
  displayTela.drawPixel(49, 33, SSD1306_BLACK);
  displayTela.drawPixel(49, 35, SSD1306_BLACK);
  displayTela.drawPixel(49, 36, SSD1306_BLACK);
  displayTela.drawPixel(48, 33, SSD1306_BLACK);
  //id: 204 pixel 250 
  displayTela.drawPixel(48, 33, SSD1306_BLACK);
  displayTela.drawPixel(49, 33, SSD1306_BLACK);
  //id: 205 pixel 251 
  displayTela.drawPixel(49, 33, SSD1306_BLACK);
  //id: 206 pixel 252 
  displayTela.drawPixel(50, 33, SSD1306_BLACK);
  displayTela.drawPixel(50, 32, SSD1306_BLACK);
  displayTela.drawPixel(51, 32, SSD1306_BLACK);
  displayTela.drawPixel(52, 32, SSD1306_BLACK);
  displayTela.drawPixel(52, 33, SSD1306_BLACK);
  displayTela.drawPixel(51, 33, SSD1306_BLACK);
  displayTela.drawPixel(50, 34, SSD1306_BLACK);
  displayTela.drawPixel(50, 35, SSD1306_BLACK);
  displayTela.drawPixel(51, 35, SSD1306_BLACK);
  displayTela.drawPixel(51, 34, SSD1306_BLACK);
  displayTela.drawPixel(52, 34, SSD1306_BLACK);
  displayTela.drawPixel(52, 35, SSD1306_BLACK);
  //id: 207 pixel 253 
  displayTela.drawPixel(53, 33, SSD1306_BLACK);
  //id: 208 pixel 254 
  displayTela.drawPixel(53, 35, SSD1306_BLACK);
  //id: 209 pixel 255 
  displayTela.drawPixel(53, 34, SSD1306_BLACK);
  displayTela.drawPixel(54, 33, SSD1306_BLACK);
  displayTela.drawPixel(54, 34, SSD1306_BLACK);
  displayTela.drawPixel(54, 35, SSD1306_BLACK);
  displayTela.drawPixel(55, 35, SSD1306_BLACK);
  //id: 210 pixel 256 
  displayTela.drawPixel(55, 34, SSD1306_BLACK);
  //id: 211 pixel 257 
  displayTela.drawPixel(47, 35, SSD1306_WHITE);
  displayTela.drawPixel(47, 34, SSD1306_WHITE);
  //id: 212 pixel 258 
  displayTela.drawPixel(47, 34, SSD1306_WHITE);
  //id: 213 pixel 259 
  displayTela.drawPixel(47, 33, SSD1306_WHITE);
  displayTela.drawPixel(48, 33, SSD1306_WHITE);
  //id: 214 pixel 260 
  displayTela.drawPixel(49, 32, SSD1306_WHITE);
  //id: 215 pixel 261 
  displayTela.drawPixel(50, 32, SSD1306_WHITE);
  //id: 216 pixel 262 
  displayTela.drawPixel(53, 32, SSD1306_BLACK);
  //id: 217 pixel 263 
  displayTela.drawPixel(55, 33, SSD1306_BLACK);
  //id: 218 pixel 264 
  displayTela.drawPixel(56, 36, SSD1306_BLACK);
  //id: 219 pixel 265 
  displayTela.drawPixel(56, 34, SSD1306_BLACK);
  displayTela.drawPixel(56, 35, SSD1306_BLACK);
  //id: 220 pixel 266 
  displayTela.drawPixel(21, 34, SSD1306_WHITE);
  displayTela.drawPixel(21, 35, SSD1306_WHITE);
  //id: 221 pixel 267 
  displayTela.drawPixel(21, 35, SSD1306_WHITE);
  //id: 222 pixel 268 
  displayTela.drawPixel(19, 34, SSD1306_BLACK);
  displayTela.drawPixel(18, 34, SSD1306_BLACK);
  //id: 223 pixel 269 
  displayTela.drawPixel(10, 34, SSD1306_BLACK);
  //id: 224 pixel 270 
  displayTela.drawPixel(18, 33, SSD1306_BLACK);
  //id: 225 pixel 271 
  displayTela.drawPixel(11, 33, SSD1306_BLACK);
  //id: 226 pixel 272 
  displayTela.drawPixel(13, 32, SSD1306_BLACK);
  //id: 227 pixel 273 
  displayTela.drawPixel(16, 32, SSD1306_BLACK);
  //id: 228 pixel 274 
  displayTela.drawPixel(10, 35, SSD1306_WHITE);
  displayTela.drawPixel(10, 34, SSD1306_WHITE);
  displayTela.drawPixel(10, 33, SSD1306_WHITE);
  //id: 229 pixel 275 
  displayTela.drawPixel(11, 33, SSD1306_WHITE);
  //id: 230 pixel 276 
  displayTela.drawPixel(13, 32, SSD1306_WHITE);
  //id: 231 fillCircle 277 
  displayTela.fillCircle(76, 28, 6, SSD1306_WHITE);
  //id: 232 fillCircle 278 
  displayTela.fillCircle(83, 18, 6, SSD1306_WHITE);
  //id: 233 fillCircle 279 
  displayTela.fillCircle(79, 22, 4, SSD1306_WHITE);
  //id: 234 fillCircle 280 
  displayTela.fillCircle(91, 15, 4, SSD1306_WHITE);
  //id: 235 fillCircle 281 
  displayTela.fillCircle(98, 19, 6, SSD1306_WHITE);
  //id: 236 fillCircle 282 
  displayTela.fillCircle(105, 27, 5, SSD1306_WHITE);
  //id: 237 fillCircle 283 
  displayTela.fillCircle(103, 33, 4, SSD1306_WHITE);
  //id: 238 fillCircle 284 
  displayTela.fillCircle(93, 32, 5, SSD1306_WHITE);
  //id: 239 fillCircle 285 
  displayTela.fillCircle(89, 20, 3, SSD1306_WHITE);
  //id: 240 fillCircle 286 
  displayTela.fillCircle(95, 23, 4, SSD1306_WHITE);
  //id: 241 fillCircle 287 
  displayTela.fillCircle(87, 25, 5, SSD1306_WHITE);
  //id: 242 fillCircle 288 
  displayTela.fillCircle(98, 27, 4, SSD1306_WHITE);
  //id: 243 fillCircle 289 
  displayTela.fillCircle(83, 29, 5, SSD1306_WHITE);
  //id: 244 fillCircle 290 
  displayTela.fillCircle(85, 34, 5, SSD1306_WHITE);
  //id: 245 fillCircle 291 
  displayTela.fillCircle(94, 52, 3, SSD1306_WHITE);
  //id: 246 pixel 292 
  displayTela.drawPixel(94, 48, SSD1306_WHITE);
  displayTela.drawPixel(94, 47, SSD1306_WHITE);
  displayTela.drawPixel(95, 48, SSD1306_WHITE);
  displayTela.drawPixel(93, 48, SSD1306_WHITE);
  //id: 247 pixel 293 
  displayTela.drawPixel(95, 49, SSD1306_WHITE);
  displayTela.drawPixel(96, 49, SSD1306_WHITE);
  //id: 248 pixel 294 
  displayTela.drawPixel(92, 49, SSD1306_WHITE);
  //id: 249 pixel 295 
  displayTela.drawPixel(82, 41, SSD1306_WHITE);
  displayTela.drawPixel(82, 42, SSD1306_WHITE);
  displayTela.drawPixel(81, 42, SSD1306_WHITE);
  displayTela.drawPixel(83, 42, SSD1306_WHITE);
  displayTela.drawPixel(81, 43, SSD1306_WHITE);
  displayTela.drawPixel(82, 43, SSD1306_WHITE);
  displayTela.drawPixel(83, 43, SSD1306_WHITE);
  displayTela.drawPixel(83, 44, SSD1306_WHITE);
  displayTela.drawPixel(84, 44, SSD1306_WHITE);
  displayTela.drawPixel(84, 43, SSD1306_WHITE);
  displayTela.drawPixel(80, 43, SSD1306_WHITE);
  displayTela.drawPixel(80, 44, SSD1306_WHITE);
  displayTela.drawPixel(81, 44, SSD1306_WHITE);
  displayTela.drawPixel(82, 44, SSD1306_WHITE);
  //id: 250 pixel 296 
  displayTela.drawPixel(85, 45, SSD1306_WHITE);
  displayTela.drawPixel(85, 46, SSD1306_WHITE);
  displayTela.drawPixel(85, 47, SSD1306_WHITE);
  displayTela.drawPixel(84, 47, SSD1306_WHITE);
  displayTela.drawPixel(83, 47, SSD1306_WHITE);
  displayTela.drawPixel(82, 48, SSD1306_WHITE);
  displayTela.drawPixel(81, 48, SSD1306_WHITE);
  displayTela.drawPixel(80, 47, SSD1306_WHITE);
  displayTela.drawPixel(79, 47, SSD1306_WHITE);
  displayTela.drawPixel(79, 46, SSD1306_WHITE);
  displayTela.drawPixel(79, 45, SSD1306_WHITE);
  displayTela.drawPixel(80, 45, SSD1306_WHITE);
  displayTela.drawPixel(81, 45, SSD1306_WHITE);
  displayTela.drawPixel(82, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 45, SSD1306_WHITE);
  displayTela.drawPixel(84, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 46, SSD1306_WHITE);
  displayTela.drawPixel(82, 46, SSD1306_WHITE);
  displayTela.drawPixel(81, 46, SSD1306_WHITE);
  displayTela.drawPixel(80, 46, SSD1306_WHITE);
  displayTela.drawPixel(84, 46, SSD1306_WHITE);
  displayTela.drawPixel(82, 47, SSD1306_WHITE);
  displayTela.drawPixel(81, 47, SSD1306_WHITE);
  //id: 251 pixel 297 
  displayTela.drawPixel(83, 47, SSD1306_WHITE);
  displayTela.drawPixel(84, 47, SSD1306_WHITE);
  displayTela.drawPixel(84, 48, SSD1306_WHITE);
  displayTela.drawPixel(83, 48, SSD1306_WHITE);
  displayTela.drawPixel(82, 48, SSD1306_WHITE);
  displayTela.drawPixel(81, 48, SSD1306_WHITE);
  displayTela.drawPixel(80, 48, SSD1306_WHITE);
  displayTela.drawPixel(81, 49, SSD1306_WHITE);
  displayTela.drawPixel(82, 49, SSD1306_WHITE);
  displayTela.drawPixel(83, 49, SSD1306_WHITE);
  //id: 252 pixel 298 
  displayTela.drawPixel(99, 38, SSD1306_WHITE);
  displayTela.drawPixel(98, 38, SSD1306_WHITE);
  displayTela.drawPixel(98, 39, SSD1306_WHITE);
  //id: 253 pixel 299 
  displayTela.drawPixel(99, 39, SSD1306_WHITE);
  displayTela.drawPixel(100, 39, SSD1306_WHITE);
  //id: 254 pixel 300 
  displayTela.drawPixel(98, 38, SSD1306_BLACK);
  //id: 255 pixel 301 
  displayTela.drawPixel(97, 40, SSD1306_WHITE);
  displayTela.drawPixel(97, 41, SSD1306_WHITE);
  displayTela.drawPixel(97, 42, SSD1306_WHITE);
  displayTela.drawPixel(98, 41, SSD1306_WHITE);
  displayTela.drawPixel(99, 41, SSD1306_WHITE);
  displayTela.drawPixel(99, 40, SSD1306_WHITE);
  displayTela.drawPixel(100, 40, SSD1306_WHITE);
  displayTela.drawPixel(101, 40, SSD1306_WHITE);
  displayTela.drawPixel(101, 41, SSD1306_WHITE);
  displayTela.drawPixel(100, 41, SSD1306_WHITE);
  displayTela.drawPixel(98, 40, SSD1306_WHITE);
  displayTela.drawPixel(98, 42, SSD1306_WHITE);
  displayTela.drawPixel(96, 42, SSD1306_WHITE);
  displayTela.drawPixel(99, 42, SSD1306_WHITE);
  displayTela.drawPixel(100, 42, SSD1306_WHITE);
  displayTela.drawPixel(101, 42, SSD1306_WHITE);
  displayTela.drawPixel(102, 42, SSD1306_WHITE);
  displayTela.drawPixel(102, 43, SSD1306_WHITE);
  displayTela.drawPixel(102, 44, SSD1306_WHITE);
  displayTela.drawPixel(101, 44, SSD1306_WHITE);
  displayTela.drawPixel(100, 44, SSD1306_WHITE);
  displayTela.drawPixel(99, 44, SSD1306_WHITE);
  displayTela.drawPixel(98, 44, SSD1306_WHITE);
  displayTela.drawPixel(97, 44, SSD1306_WHITE);
  displayTela.drawPixel(96, 44, SSD1306_WHITE);
  displayTela.drawPixel(96, 43, SSD1306_WHITE);
  displayTela.drawPixel(97, 43, SSD1306_WHITE);
  displayTela.drawPixel(98, 43, SSD1306_WHITE);
  displayTela.drawPixel(99, 43, SSD1306_WHITE);
  displayTela.drawPixel(100, 43, SSD1306_WHITE);
  displayTela.drawPixel(101, 43, SSD1306_WHITE);
  displayTela.drawPixel(97, 45, SSD1306_WHITE);
  displayTela.drawPixel(98, 45, SSD1306_WHITE);
  displayTela.drawPixel(99, 45, SSD1306_WHITE);
  displayTela.drawPixel(100, 45, SSD1306_WHITE);
  displayTela.drawPixel(101, 45, SSD1306_WHITE);
  displayTela.drawPixel(99, 46, SSD1306_WHITE);
  displayTela.drawPixel(98, 46, SSD1306_WHITE);
  displayTela.drawPixel(100, 46, SSD1306_WHITE);


}

// Ícone 2: NUVEM (nublado) - espaço reservado
// Já definido acima como desenharNuvem() - nenhuma ação necessária
void desenharNuvem() { 


  //id: 0 fillCircle 293 
  displayTela.fillCircle(69, 7, 4, SSD1306_WHITE);
  //id: 1 fillCircle 294 
  displayTela.fillCircle(65, 28, 16, SSD1306_WHITE);
  //id: 2 fillCircle 295 
  displayTela.fillCircle(62, 9, 6, SSD1306_WHITE);
  //id: 3 fillCircle 296 
  displayTela.fillCircle(53, 15, 6, SSD1306_WHITE);
  //id: 4 fillCircle 297 
  displayTela.fillCircle(48, 27, 7, SSD1306_WHITE);
  //id: 5 fillCircle 298 
  displayTela.fillCircle(52, 38, 5, SSD1306_WHITE);
  //id: 6 fillCircle 299 
  displayTela.fillCircle(82, 19, 5, SSD1306_WHITE);
  //id: 7 fillCircle 300 
  displayTela.fillCircle(77, 11, 5, SSD1306_WHITE);
  //id: 8 fillCircle 301 
  displayTela.fillCircle(72, 13, 1, SSD1306_WHITE);
  //id: 9 fillCircle 302 
  displayTela.fillCircle(70, 11, 0, SSD1306_WHITE);
  //id: 10 fillCircle 303 
  displayTela.fillCircle(70, 12, 0, SSD1306_WHITE);
  //id: 11 fillCircle 304 
  displayTela.fillCircle(87, 26, 6, SSD1306_WHITE);
  //id: 12 fillCircle 305 
  displayTela.fillCircle(83, 35, 5, SSD1306_WHITE);
  //id: 13 fillCircle 306 
  displayTela.fillCircle(46, 36, 6, SSD1306_WHITE);
  //id: 14 fillCircle 307 
  displayTela.fillCircle(78, 40, 3, SSD1306_WHITE);
  //id: 15 fillCircle 308 
  displayTela.fillCircle(93, 36, 6, SSD1306_WHITE);
  //id: 16 fillCircle 309 
  displayTela.fillCircle(67, 28, 0, SSD1306_WHITE);
  //id: 17 fillCircle 310 
  displayTela.fillCircle(93, 28, 4, SSD1306_WHITE);
  //id: 18 fillCircle 311 
  displayTela.fillCircle(90, 20, 5, SSD1306_WHITE);
  //id: 19 fillCircle 312 
  displayTela.fillCircle(70, 22, 0, SSD1306_WHITE);
  //id: 20 fillCircle 313 
  displayTela.fillCircle(84, 14, 2, SSD1306_WHITE);
  //id: 21 fillCircle 314 
  displayTela.fillCircle(43, 18, 5, SSD1306_WHITE);
  //id: 22 fillCircle 315 
  displayTela.fillCircle(37, 24, 4, SSD1306_WHITE);
  //id: 23 fillCircle 316 
  displayTela.fillCircle(18, 34, 8, SSD1306_WHITE);
  //id: 24 fillCircle 317 
  displayTela.fillCircle(31, 33, 4, SSD1306_WHITE);
  //id: 25 fillCircle 318 
  displayTela.fillCircle(39, 36, 6, SSD1306_WHITE);
  //id: 26 fillCircle 319 
  displayTela.fillCircle(39, 30, 3, SSD1306_WHITE);
  //id: 27 fillCircle 320 
  displayTela.fillCircle(29, 26, 5, SSD1306_WHITE);
  //id: 28 fillCircle 321 
  displayTela.fillCircle(36, 30, 2, SSD1306_WHITE);
  //id: 29 fillCircle 322 
  displayTela.fillCircle(26, 31, 0, SSD1306_WHITE);
  //id: 30 fillCircle 323 
  displayTela.fillCircle(28, 37, 3, SSD1306_WHITE);
  //id: 31 fillCircle 324 
  displayTela.fillCircle(34, 20, 3, SSD1306_WHITE);
  //id: 32 fillCircle 325 
  displayTela.fillCircle(29, 38, 5, SSD1306_WHITE);
  //id: 33 fillCircle 326 
  displayTela.fillCircle(41, 42, 5, SSD1306_WHITE);
  //id: 34 fillCircle 327 
  displayTela.fillCircle(49, 43, 2, SSD1306_WHITE);
  //id: 35 fillCircle 328 
  displayTela.fillCircle(32, 43, 3, SSD1306_WHITE);
  //id: 36 fillCircle 329 
  displayTela.fillCircle(126, 9, 7, SSD1306_WHITE);
  //id: 37 fillCircle 331 
  displayTela.fillCircle(118, 14, 5, SSD1306_WHITE);
  //id: 38 fillCircle 332 
  displayTela.fillCircle(114, 19, 5, SSD1306_WHITE);
  //id: 39 fillCircle 333 
  displayTela.fillCircle(124, 22, 5, SSD1306_WHITE);
  //id: 40 fillCircle 334 
  displayTela.fillCircle(127, 16, 0, SSD1306_WHITE);
  //id: 41 fillCircle 335 
  displayTela.fillCircle(127, 17, 0, SSD1306_WHITE);
  //id: 42 fillCircle 336 
  displayTela.fillCircle(118, 23, 1, SSD1306_WHITE);
  //id: 43 fillCircle 338 
  displayTela.fillCircle(96, 57, 7, SSD1306_WHITE);
  //id: 44 fillCircle 340 
  displayTela.fillCircle(88, 63, 3, SSD1306_WHITE);
  //id: 45 fillCircle 342 
  displayTela.fillCircle(123, 61, 12, SSD1306_WHITE);
  //id: 46 fillCircle 343 
  displayTela.fillCircle(109, 58, 9, SSD1306_WHITE);
  //id: 47 fillCircle 344 
  displayTela.fillCircle(114, 48, 4, SSD1306_WHITE);
  //id: 48 fillCircle 345 
  displayTela.fillCircle(124, 47, 6, SSD1306_WHITE);
  //id: 49 fillCircle 347 
  displayTela.fillCircle(2, 62, 6, SSD1306_WHITE);
  //id: 50 fillCircle 348 
  displayTela.fillCircle(0, 55, 2, SSD1306_WHITE);
  //id: 51 fillCircle 349 
  displayTela.fillCircle(19, 9, 5, SSD1306_WHITE);
  //id: 52 fillCircle 351 
  displayTela.fillCircle(4, 3, 10, SSD1306_WHITE);
  //id: 53 fillCircle 352 
  displayTela.fillCircle(13, 12, 3, SSD1306_WHITE);
  //id: 54 fillCircle 353 
  displayTela.fillCircle(15, 4, 3, SSD1306_WHITE);



}

// Ícone 3: SOL (claro/ensolarado) - espaço reservado
// Já definido acima como desenharSol() - nenhuma ação necessária

void desenharSol() {
  // Desenha um sol


// Draw shapes

  //id: 0 fillCircle 303 
  displayTela.fillCircle(63, 30, 17, SSD1306_WHITE);
  //id: 1 pixel 304 
  displayTela.drawPixel(65, 10, SSD1306_WHITE);
  displayTela.drawPixel(64, 10, SSD1306_WHITE);
  displayTela.drawPixel(63, 10, SSD1306_WHITE);
  displayTela.drawPixel(63, 9, SSD1306_WHITE);
  displayTela.drawPixel(62, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 8, SSD1306_WHITE);
  displayTela.drawPixel(61, 7, SSD1306_WHITE);
  displayTela.drawPixel(62, 7, SSD1306_WHITE);
  displayTela.drawPixel(62, 6, SSD1306_WHITE);
  displayTela.drawPixel(62, 5, SSD1306_WHITE);
  displayTela.drawPixel(62, 4, SSD1306_WHITE);
  displayTela.drawPixel(62, 3, SSD1306_WHITE);
  displayTela.drawPixel(62, 2, SSD1306_WHITE);
  displayTela.drawPixel(62, 1, SSD1306_WHITE);
  displayTela.drawPixel(61, 1, SSD1306_WHITE);
  //id: 2 pixel 305 
  displayTela.drawPixel(62, 1, SSD1306_WHITE);
  displayTela.drawPixel(61, 1, SSD1306_WHITE);
  displayTela.drawPixel(61, 2, SSD1306_WHITE);
  displayTela.drawPixel(61, 3, SSD1306_WHITE);
  displayTela.drawPixel(62, 3, SSD1306_WHITE);
  displayTela.drawPixel(62, 4, SSD1306_WHITE);
  displayTela.drawPixel(62, 5, SSD1306_WHITE);
  displayTela.drawPixel(62, 6, SSD1306_WHITE);
  displayTela.drawPixel(63, 6, SSD1306_WHITE);
  displayTela.drawPixel(63, 5, SSD1306_WHITE);
  displayTela.drawPixel(64, 5, SSD1306_WHITE);
  displayTela.drawPixel(63, 4, SSD1306_WHITE);
  displayTela.drawPixel(64, 4, SSD1306_WHITE);
  displayTela.drawPixel(65, 5, SSD1306_WHITE);
  displayTela.drawPixel(65, 6, SSD1306_WHITE);
  displayTela.drawPixel(65, 7, SSD1306_WHITE);
  displayTela.drawPixel(65, 8, SSD1306_WHITE);
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 6, SSD1306_WHITE);
  displayTela.drawPixel(65, 9, SSD1306_WHITE);
  displayTela.drawPixel(64, 9, SSD1306_WHITE);
  displayTela.drawPixel(63, 8, SSD1306_WHITE);
  displayTela.drawPixel(62, 8, SSD1306_WHITE);
  displayTela.drawPixel(62, 7, SSD1306_WHITE);
  displayTela.drawPixel(61, 5, SSD1306_WHITE);
  displayTela.drawPixel(61, 4, SSD1306_WHITE);
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  //id: 3 pixel 306 
  displayTela.drawPixel(60, 4, SSD1306_WHITE);
  displayTela.drawPixel(60, 5, SSD1306_WHITE);
  displayTela.drawPixel(60, 6, SSD1306_WHITE);
  displayTela.drawPixel(61, 5, SSD1306_WHITE);
  displayTela.drawPixel(61, 6, SSD1306_WHITE);
  displayTela.drawPixel(62, 6, SSD1306_WHITE);
  //id: 4 pixel 307 
  displayTela.drawPixel(64, 4, SSD1306_BLACK);
  //id: 5 pixel 308 
  displayTela.drawPixel(64, 4, SSD1306_BLACK);
  displayTela.drawPixel(63, 5, SSD1306_BLACK);
  displayTela.drawPixel(64, 5, SSD1306_BLACK);
  displayTela.drawPixel(65, 5, SSD1306_BLACK);
  displayTela.drawPixel(65, 6, SSD1306_BLACK);
  displayTela.drawPixel(65, 7, SSD1306_BLACK);
  displayTela.drawPixel(64, 7, SSD1306_BLACK);
  displayTela.drawPixel(64, 6, SSD1306_BLACK);
  displayTela.drawPixel(63, 4, SSD1306_BLACK);
  displayTela.drawPixel(63, 3, SSD1306_BLACK);
  displayTela.drawPixel(64, 3, SSD1306_BLACK);
  displayTela.drawPixel(65, 4, SSD1306_BLACK);
  //id: 6 pixel 309 
  displayTela.drawPixel(63, 4, SSD1306_WHITE);
  displayTela.drawPixel(63, 5, SSD1306_WHITE);
  displayTela.drawPixel(63, 6, SSD1306_WHITE);
  displayTela.drawPixel(64, 6, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  //id: 7 pixel 310 
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  //id: 8 pixel 311 
  displayTela.drawPixel(70, 11, SSD1306_WHITE);
  displayTela.drawPixel(71, 11, SSD1306_WHITE);
  displayTela.drawPixel(71, 10, SSD1306_WHITE);
  displayTela.drawPixel(72, 10, SSD1306_WHITE);
  displayTela.drawPixel(72, 9, SSD1306_WHITE);
  displayTela.drawPixel(73, 9, SSD1306_WHITE);
  displayTela.drawPixel(73, 8, SSD1306_WHITE);
  displayTela.drawPixel(74, 7, SSD1306_WHITE);
  displayTela.drawPixel(74, 6, SSD1306_WHITE);
  displayTela.drawPixel(75, 6, SSD1306_WHITE);
  displayTela.drawPixel(75, 5, SSD1306_WHITE);
  displayTela.drawPixel(75, 7, SSD1306_WHITE);
  displayTela.drawPixel(74, 8, SSD1306_WHITE);
  displayTela.drawPixel(74, 9, SSD1306_WHITE);
  displayTela.drawPixel(73, 10, SSD1306_WHITE);
  displayTela.drawPixel(72, 11, SSD1306_WHITE);
  //id: 9 pixel 312 
  displayTela.drawPixel(83, 31, SSD1306_WHITE);
  displayTela.drawPixel(83, 30, SSD1306_WHITE);
  displayTela.drawPixel(83, 29, SSD1306_WHITE);
  displayTela.drawPixel(83, 28, SSD1306_WHITE);
  displayTela.drawPixel(84, 28, SSD1306_WHITE);
  displayTela.drawPixel(85, 28, SSD1306_WHITE);
  displayTela.drawPixel(86, 28, SSD1306_WHITE);
  displayTela.drawPixel(87, 28, SSD1306_WHITE);
  displayTela.drawPixel(88, 28, SSD1306_WHITE);
  displayTela.drawPixel(89, 28, SSD1306_WHITE);
  displayTela.drawPixel(90, 28, SSD1306_WHITE);
  displayTela.drawPixel(91, 28, SSD1306_WHITE);
  displayTela.drawPixel(92, 28, SSD1306_WHITE);
  displayTela.drawPixel(93, 28, SSD1306_WHITE);
  displayTela.drawPixel(94, 28, SSD1306_WHITE);
  //id: 10 pixel 313 
  displayTela.drawPixel(95, 28, SSD1306_WHITE);
  displayTela.drawPixel(94, 28, SSD1306_WHITE);
  displayTela.drawPixel(93, 28, SSD1306_WHITE);
  displayTela.drawPixel(92, 28, SSD1306_WHITE);
  displayTela.drawPixel(91, 28, SSD1306_WHITE);
  displayTela.drawPixel(90, 28, SSD1306_WHITE);
  displayTela.drawPixel(90, 29, SSD1306_WHITE);
  displayTela.drawPixel(89, 29, SSD1306_WHITE);
  displayTela.drawPixel(88, 29, SSD1306_WHITE);
  displayTela.drawPixel(87, 29, SSD1306_WHITE);
  displayTela.drawPixel(86, 29, SSD1306_WHITE);
  displayTela.drawPixel(86, 30, SSD1306_WHITE);
  displayTela.drawPixel(85, 30, SSD1306_WHITE);
  displayTela.drawPixel(87, 31, SSD1306_WHITE);
  displayTela.drawPixel(88, 31, SSD1306_WHITE);
  displayTela.drawPixel(89, 31, SSD1306_WHITE);
  displayTela.drawPixel(90, 31, SSD1306_WHITE);
  displayTela.drawPixel(90, 30, SSD1306_WHITE);
  displayTela.drawPixel(91, 30, SSD1306_WHITE);
  displayTela.drawPixel(91, 29, SSD1306_WHITE);
  displayTela.drawPixel(89, 30, SSD1306_WHITE);
  displayTela.drawPixel(88, 30, SSD1306_WHITE);
  displayTela.drawPixel(87, 30, SSD1306_WHITE);
  displayTela.drawPixel(86, 31, SSD1306_WHITE);
  displayTela.drawPixel(85, 31, SSD1306_WHITE);
  displayTela.drawPixel(85, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 32, SSD1306_WHITE);
  displayTela.drawPixel(83, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 31, SSD1306_WHITE);
  displayTela.drawPixel(84, 30, SSD1306_WHITE);
  displayTela.drawPixel(84, 29, SSD1306_WHITE);
  displayTela.drawPixel(84, 28, SSD1306_WHITE);
  displayTela.drawPixel(85, 28, SSD1306_WHITE);
  displayTela.drawPixel(85, 29, SSD1306_WHITE);
  //id: 11 pixel 314 
  displayTela.drawPixel(87, 29, SSD1306_WHITE);
  //id: 12 pixel 315 
  displayTela.drawPixel(91, 27, SSD1306_WHITE);
  displayTela.drawPixel(90, 27, SSD1306_WHITE);
  displayTela.drawPixel(89, 27, SSD1306_WHITE);
  displayTela.drawPixel(88, 27, SSD1306_WHITE);
  displayTela.drawPixel(87, 27, SSD1306_WHITE);
  displayTela.drawPixel(86, 27, SSD1306_WHITE);
  //id: 13 pixel 316 
  displayTela.drawPixel(62, 50, SSD1306_WHITE);
  displayTela.drawPixel(63, 50, SSD1306_WHITE);
  displayTela.drawPixel(64, 50, SSD1306_WHITE);
  displayTela.drawPixel(64, 51, SSD1306_WHITE);
  displayTela.drawPixel(65, 51, SSD1306_WHITE);
  displayTela.drawPixel(65, 52, SSD1306_WHITE);
  displayTela.drawPixel(65, 53, SSD1306_WHITE);
  displayTela.drawPixel(65, 54, SSD1306_WHITE);
  displayTela.drawPixel(65, 55, SSD1306_WHITE);
  displayTela.drawPixel(65, 56, SSD1306_WHITE);
  displayTela.drawPixel(65, 57, SSD1306_WHITE);
  displayTela.drawPixel(66, 57, SSD1306_WHITE);
  displayTela.drawPixel(66, 58, SSD1306_WHITE);
  displayTela.drawPixel(67, 58, SSD1306_WHITE);
  displayTela.drawPixel(67, 59, SSD1306_WHITE);
  displayTela.drawPixel(67, 60, SSD1306_WHITE);
  displayTela.drawPixel(66, 56, SSD1306_WHITE);
  displayTela.drawPixel(66, 55, SSD1306_WHITE);
  displayTela.drawPixel(66, 54, SSD1306_WHITE);
  displayTela.drawPixel(66, 53, SSD1306_WHITE);
  displayTela.drawPixel(66, 52, SSD1306_WHITE);
  displayTela.drawPixel(66, 51, SSD1306_WHITE);
  displayTela.drawPixel(65, 50, SSD1306_WHITE);
  displayTela.drawPixel(63, 51, SSD1306_WHITE);
  displayTela.drawPixel(63, 52, SSD1306_WHITE);
  displayTela.drawPixel(63, 53, SSD1306_WHITE);
  displayTela.drawPixel(64, 53, SSD1306_WHITE);
  displayTela.drawPixel(64, 54, SSD1306_WHITE);
  displayTela.drawPixel(64, 52, SSD1306_WHITE);
  displayTela.drawPixel(66, 50, SSD1306_WHITE);
  displayTela.drawPixel(67, 54, SSD1306_WHITE);
  displayTela.drawPixel(67, 55, SSD1306_WHITE);
  displayTela.drawPixel(67, 56, SSD1306_WHITE);
  displayTela.drawPixel(67, 57, SSD1306_WHITE);
  //id: 14 pixel 317 
  displayTela.drawPixel(68, 56, SSD1306_WHITE);
  //id: 15 pixel 318 
  displayTela.drawPixel(68, 57, SSD1306_WHITE);
  //id: 16 pixel 319 
  displayTela.drawPixel(62, 51, SSD1306_WHITE);
  //id: 17 pixel 320 
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  //id: 18 pixel 321 
  displayTela.drawPixel(43, 29, SSD1306_WHITE);
  displayTela.drawPixel(42, 29, SSD1306_WHITE);
  displayTela.drawPixel(42, 28, SSD1306_WHITE);
  displayTela.drawPixel(41, 28, SSD1306_WHITE);
  displayTela.drawPixel(40, 28, SSD1306_WHITE);
  displayTela.drawPixel(39, 28, SSD1306_WHITE);
  displayTela.drawPixel(38, 28, SSD1306_WHITE);
  displayTela.drawPixel(37, 28, SSD1306_WHITE);
  displayTela.drawPixel(36, 28, SSD1306_WHITE);
  displayTela.drawPixel(39, 29, SSD1306_WHITE);
  displayTela.drawPixel(39, 30, SSD1306_WHITE);
  displayTela.drawPixel(40, 30, SSD1306_WHITE);
  displayTela.drawPixel(41, 31, SSD1306_WHITE);
  displayTela.drawPixel(42, 31, SSD1306_WHITE);
  displayTela.drawPixel(42, 30, SSD1306_WHITE);
  displayTela.drawPixel(43, 30, SSD1306_WHITE);
  displayTela.drawPixel(44, 30, SSD1306_WHITE);
  displayTela.drawPixel(44, 31, SSD1306_WHITE);
  displayTela.drawPixel(44, 32, SSD1306_WHITE);
  displayTela.drawPixel(44, 33, SSD1306_WHITE);
  displayTela.drawPixel(41, 29, SSD1306_WHITE);
  displayTela.drawPixel(40, 29, SSD1306_WHITE);
  displayTela.drawPixel(38, 29, SSD1306_WHITE);
  displayTela.drawPixel(41, 30, SSD1306_WHITE);
  displayTela.drawPixel(43, 31, SSD1306_WHITE);
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  //id: 19 pixel 322 
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  //id: 20 pixel 323 
  displayTela.drawPixel(44, 28, SSD1306_BLACK);
  displayTela.drawPixel(44, 29, SSD1306_BLACK);
  displayTela.drawPixel(44, 30, SSD1306_BLACK);
  displayTela.drawPixel(44, 31, SSD1306_BLACK);
  displayTela.drawPixel(44, 32, SSD1306_BLACK);
  displayTela.drawPixel(44, 33, SSD1306_BLACK);
  displayTela.drawPixel(44, 34, SSD1306_BLACK);
  //id: 21 pixel 324 
  displayTela.drawPixel(40, 31, SSD1306_WHITE);
  displayTela.drawPixel(39, 31, SSD1306_WHITE);
  displayTela.drawPixel(38, 31, SSD1306_WHITE);
  displayTela.drawPixel(38, 30, SSD1306_WHITE);
  displayTela.drawPixel(38, 29, SSD1306_WHITE);
  displayTela.drawPixel(37, 29, SSD1306_WHITE);
  displayTela.drawPixel(36, 29, SSD1306_WHITE);
  displayTela.drawPixel(35, 29, SSD1306_WHITE);
  displayTela.drawPixel(34, 29, SSD1306_WHITE);
  displayTela.drawPixel(34, 28, SSD1306_WHITE);
  displayTela.drawPixel(33, 28, SSD1306_WHITE);
  displayTela.drawPixel(35, 28, SSD1306_WHITE);
  displayTela.drawPixel(36, 28, SSD1306_WHITE);
  displayTela.drawPixel(39, 29, SSD1306_WHITE);
  displayTela.drawPixel(39, 28, SSD1306_WHITE);
  displayTela.drawPixel(40, 28, SSD1306_WHITE);
  displayTela.drawPixel(41, 28, SSD1306_WHITE);
  displayTela.drawPixel(42, 27, SSD1306_WHITE);
  displayTela.drawPixel(41, 27, SSD1306_WHITE);
  displayTela.drawPixel(40, 27, SSD1306_WHITE);
  displayTela.drawPixel(39, 27, SSD1306_WHITE);
  displayTela.drawPixel(38, 27, SSD1306_WHITE);
  displayTela.drawPixel(37, 27, SSD1306_WHITE);
  //id: 22 pixel 325 
  displayTela.drawPixel(36, 26, SSD1306_WHITE);
  displayTela.drawPixel(36, 27, SSD1306_WHITE);
  displayTela.drawPixel(35, 27, SSD1306_WHITE);
  displayTela.drawPixel(36, 28, SSD1306_WHITE);
  displayTela.drawPixel(37, 28, SSD1306_WHITE);
  //id: 23 pixel 326 
  displayTela.drawPixel(49, 44, SSD1306_WHITE);
  displayTela.drawPixel(48, 44, SSD1306_WHITE);
  displayTela.drawPixel(47, 44, SSD1306_WHITE);
  displayTela.drawPixel(46, 44, SSD1306_WHITE);
  displayTela.drawPixel(46, 45, SSD1306_WHITE);
  displayTela.drawPixel(46, 46, SSD1306_WHITE);
  displayTela.drawPixel(46, 47, SSD1306_WHITE);
  displayTela.drawPixel(46, 48, SSD1306_WHITE);
  displayTela.drawPixel(45, 48, SSD1306_WHITE);
  displayTela.drawPixel(45, 49, SSD1306_WHITE);
  displayTela.drawPixel(44, 49, SSD1306_WHITE);
  displayTela.drawPixel(44, 50, SSD1306_WHITE);
  displayTela.drawPixel(43, 50, SSD1306_WHITE);
  displayTela.drawPixel(43, 51, SSD1306_WHITE);
  displayTela.drawPixel(43, 52, SSD1306_WHITE);
  displayTela.drawPixel(44, 51, SSD1306_WHITE);
  displayTela.drawPixel(45, 51, SSD1306_WHITE);
  displayTela.drawPixel(46, 51, SSD1306_WHITE);
  displayTela.drawPixel(46, 50, SSD1306_WHITE);
  displayTela.drawPixel(47, 50, SSD1306_WHITE);
  displayTela.drawPixel(47, 49, SSD1306_WHITE);
  displayTela.drawPixel(47, 48, SSD1306_WHITE);
  displayTela.drawPixel(48, 48, SSD1306_WHITE);
  displayTela.drawPixel(49, 48, SSD1306_WHITE);
  displayTela.drawPixel(49, 47, SSD1306_WHITE);
  displayTela.drawPixel(49, 46, SSD1306_WHITE);
  displayTela.drawPixel(48, 46, SSD1306_WHITE);
  displayTela.drawPixel(48, 45, SSD1306_WHITE);
  displayTela.drawPixel(47, 46, SSD1306_WHITE);
  displayTela.drawPixel(47, 47, SSD1306_WHITE);
  displayTela.drawPixel(46, 49, SSD1306_WHITE);
  displayTela.drawPixel(45, 50, SSD1306_WHITE);
  displayTela.drawPixel(48, 47, SSD1306_WHITE);
  displayTela.drawPixel(49, 45, SSD1306_WHITE);
  displayTela.drawPixel(50, 45, SSD1306_WHITE);
  displayTela.drawPixel(49, 49, SSD1306_WHITE);
  displayTela.drawPixel(48, 49, SSD1306_WHITE);
  displayTela.drawPixel(48, 50, SSD1306_WHITE);
  displayTela.drawPixel(47, 51, SSD1306_WHITE);
  displayTela.drawPixel(50, 46, SSD1306_WHITE);
  displayTela.drawPixel(46, 52, SSD1306_WHITE);
  displayTela.drawPixel(46, 53, SSD1306_WHITE);
  displayTela.drawPixel(45, 53, SSD1306_WHITE);
  displayTela.drawPixel(45, 52, SSD1306_WHITE);
  displayTela.drawPixel(44, 52, SSD1306_WHITE);
  displayTela.drawPixel(44, 53, SSD1306_WHITE);
  displayTela.drawPixel(44, 54, SSD1306_WHITE);
  //id: 24 pixel 327 
  displayTela.drawPixel(43, 53, SSD1306_WHITE);
  displayTela.drawPixel(43, 54, SSD1306_WHITE);
  displayTela.drawPixel(43, 55, SSD1306_WHITE);
  displayTela.drawPixel(44, 55, SSD1306_WHITE);
  displayTela.drawPixel(44, 56, SSD1306_WHITE);
  //id: 25 pixel 328 
  displayTela.drawPixel(44, 53, SSD1306_WHITE);
  displayTela.drawPixel(45, 53, SSD1306_WHITE);
  displayTela.drawPixel(46, 53, SSD1306_WHITE);
  displayTela.drawPixel(47, 53, SSD1306_WHITE);
  displayTela.drawPixel(47, 52, SSD1306_WHITE);
  displayTela.drawPixel(47, 51, SSD1306_WHITE);
  displayTela.drawPixel(48, 50, SSD1306_WHITE);
  displayTela.drawPixel(48, 51, SSD1306_WHITE);
  displayTela.drawPixel(48, 52, SSD1306_WHITE);
  displayTela.drawPixel(48, 53, SSD1306_WHITE);
  displayTela.drawPixel(49, 52, SSD1306_WHITE);
  displayTela.drawPixel(49, 51, SSD1306_WHITE);
  displayTela.drawPixel(49, 50, SSD1306_WHITE);
  displayTela.drawPixel(49, 49, SSD1306_WHITE);
  displayTela.drawPixel(49, 48, SSD1306_WHITE);
  displayTela.drawPixel(50, 48, SSD1306_WHITE);
  //id: 26 pixel 329 
  displayTela.drawPixel(50, 48, SSD1306_WHITE);
  displayTela.drawPixel(50, 47, SSD1306_WHITE);
  displayTela.drawPixel(50, 46, SSD1306_WHITE);
  displayTela.drawPixel(50, 45, SSD1306_WHITE);
  displayTela.drawPixel(49, 45, SSD1306_WHITE);
  displayTela.drawPixel(49, 44, SSD1306_WHITE);
  displayTela.drawPixel(49, 46, SSD1306_WHITE);
  displayTela.drawPixel(49, 47, SSD1306_WHITE);
  //id: 27 pixel 330 
  displayTela.drawPixel(47, 46, SSD1306_BLACK);
  displayTela.drawPixel(47, 45, SSD1306_BLACK);
  displayTela.drawPixel(47, 44, SSD1306_BLACK);
  displayTela.drawPixel(47, 43, SSD1306_BLACK);
  //id: 28 pixel 331 
  displayTela.drawPixel(47, 43, SSD1306_BLACK);
  displayTela.drawPixel(46, 44, SSD1306_BLACK);
  displayTela.drawPixel(46, 45, SSD1306_BLACK);
  displayTela.drawPixel(46, 46, SSD1306_BLACK);
  displayTela.drawPixel(47, 46, SSD1306_BLACK);
  displayTela.drawPixel(47, 45, SSD1306_BLACK);
  displayTela.drawPixel(47, 44, SSD1306_BLACK);
  displayTela.drawPixel(47, 47, SSD1306_BLACK);
  displayTela.drawPixel(46, 47, SSD1306_BLACK);
  displayTela.drawPixel(45, 47, SSD1306_BLACK);
  displayTela.drawPixel(46, 48, SSD1306_BLACK);
  displayTela.drawPixel(46, 49, SSD1306_BLACK);
  displayTela.drawPixel(46, 50, SSD1306_BLACK);
  displayTela.drawPixel(45, 49, SSD1306_BLACK);
  displayTela.drawPixel(45, 48, SSD1306_BLACK);
  displayTela.drawPixel(44, 48, SSD1306_BLACK);
  displayTela.drawPixel(44, 49, SSD1306_BLACK);
  displayTela.drawPixel(45, 46, SSD1306_BLACK);
  displayTela.drawPixel(45, 45, SSD1306_BLACK);
  //id: 29 pixel 332 
  displayTela.drawPixel(42, 52, SSD1306_WHITE);
  displayTela.drawPixel(42, 53, SSD1306_WHITE);
  displayTela.drawPixel(42, 54, SSD1306_WHITE);
  displayTela.drawPixel(42, 55, SSD1306_WHITE);
  //id: 30 pixel 333 
  displayTela.drawPixel(76, 15, SSD1306_WHITE);
  displayTela.drawPixel(77, 15, SSD1306_WHITE);
  displayTela.drawPixel(77, 14, SSD1306_WHITE);
  displayTela.drawPixel(77, 13, SSD1306_WHITE);
  displayTela.drawPixel(78, 13, SSD1306_WHITE);
  displayTela.drawPixel(78, 12, SSD1306_WHITE);
  displayTela.drawPixel(78, 11, SSD1306_WHITE);
  displayTela.drawPixel(79, 10, SSD1306_WHITE);
  displayTela.drawPixel(79, 9, SSD1306_WHITE);
  displayTela.drawPixel(80, 9, SSD1306_WHITE);
  displayTela.drawPixel(81, 9, SSD1306_WHITE);
  displayTela.drawPixel(81, 10, SSD1306_WHITE);
  displayTela.drawPixel(82, 10, SSD1306_WHITE);
  displayTela.drawPixel(82, 11, SSD1306_WHITE);
  displayTela.drawPixel(82, 12, SSD1306_WHITE);
  displayTela.drawPixel(83, 12, SSD1306_WHITE);
  displayTela.drawPixel(84, 12, SSD1306_WHITE);
  displayTela.drawPixel(85, 12, SSD1306_WHITE);
  displayTela.drawPixel(86, 12, SSD1306_WHITE);
  displayTela.drawPixel(86, 11, SSD1306_WHITE);
  displayTela.drawPixel(86, 10, SSD1306_WHITE);
  displayTela.drawPixel(85, 10, SSD1306_WHITE);
  displayTela.drawPixel(84, 10, SSD1306_WHITE);
  displayTela.drawPixel(84, 11, SSD1306_WHITE);
  displayTela.drawPixel(84, 13, SSD1306_WHITE);
  displayTela.drawPixel(85, 14, SSD1306_WHITE);
  displayTela.drawPixel(85, 15, SSD1306_WHITE);
  displayTela.drawPixel(84, 15, SSD1306_WHITE);
  displayTela.drawPixel(84, 16, SSD1306_WHITE);
  displayTela.drawPixel(83, 16, SSD1306_WHITE);
  displayTela.drawPixel(83, 15, SSD1306_WHITE);
  displayTela.drawPixel(83, 14, SSD1306_WHITE);
  displayTela.drawPixel(83, 13, SSD1306_WHITE);
  //id: 31 pixel 334 
  displayTela.drawPixel(86, 15, SSD1306_BLACK);
  displayTela.drawPixel(86, 14, SSD1306_BLACK);
  displayTela.drawPixel(86, 13, SSD1306_BLACK);
  displayTela.drawPixel(86, 12, SSD1306_BLACK);
  displayTela.drawPixel(86, 11, SSD1306_BLACK);
  displayTela.drawPixel(86, 10, SSD1306_BLACK);
  displayTela.drawPixel(85, 11, SSD1306_BLACK);
  displayTela.drawPixel(85, 12, SSD1306_BLACK);
  displayTela.drawPixel(85, 13, SSD1306_BLACK);
  displayTela.drawPixel(85, 14, SSD1306_BLACK);
  displayTela.drawPixel(84, 15, SSD1306_BLACK);
  displayTela.drawPixel(84, 16, SSD1306_BLACK);
  displayTela.drawPixel(84, 14, SSD1306_BLACK);
  displayTela.drawPixel(85, 10, SSD1306_BLACK);
  displayTela.drawPixel(85, 9, SSD1306_BLACK);
  displayTela.drawPixel(84, 11, SSD1306_BLACK);
  displayTela.drawPixel(84, 12, SSD1306_BLACK);
  displayTela.drawPixel(84, 13, SSD1306_BLACK);
  displayTela.drawPixel(84, 10, SSD1306_BLACK);
  displayTela.drawPixel(84, 9, SSD1306_BLACK);
  displayTela.drawPixel(83, 16, SSD1306_BLACK);
  displayTela.drawPixel(83, 17, SSD1306_BLACK);
  displayTela.drawPixel(83, 18, SSD1306_BLACK);
  displayTela.drawPixel(83, 15, SSD1306_BLACK);
  displayTela.drawPixel(83, 14, SSD1306_BLACK);
  displayTela.drawPixel(85, 15, SSD1306_BLACK);
  //id: 32 pixel 335 
  displayTela.drawPixel(84, 12, SSD1306_WHITE);
  displayTela.drawPixel(84, 11, SSD1306_WHITE);
  displayTela.drawPixel(85, 11, SSD1306_WHITE);
  displayTela.drawPixel(86, 11, SSD1306_WHITE);
  displayTela.drawPixel(86, 10, SSD1306_WHITE);
  //id: 33 pixel 336 
  displayTela.drawPixel(84, 12, SSD1306_WHITE);
  displayTela.drawPixel(83, 12, SSD1306_WHITE);
  displayTela.drawPixel(82, 12, SSD1306_WHITE);
  displayTela.drawPixel(82, 11, SSD1306_WHITE);
  displayTela.drawPixel(81, 11, SSD1306_WHITE);
  displayTela.drawPixel(80, 11, SSD1306_WHITE);
  displayTela.drawPixel(80, 12, SSD1306_WHITE);
  displayTela.drawPixel(80, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 14, SSD1306_WHITE);
  //id: 34 pixel 337 
  displayTela.drawPixel(79, 15, SSD1306_WHITE);
  displayTela.drawPixel(78, 15, SSD1306_WHITE);
  displayTela.drawPixel(78, 16, SSD1306_WHITE);
  displayTela.drawPixel(78, 17, SSD1306_WHITE);
  displayTela.drawPixel(79, 17, SSD1306_WHITE);
  displayTela.drawPixel(79, 18, SSD1306_WHITE);
  displayTela.drawPixel(78, 14, SSD1306_WHITE);
  displayTela.drawPixel(78, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 12, SSD1306_WHITE);
  displayTela.drawPixel(79, 11, SSD1306_WHITE);
  displayTela.drawPixel(80, 11, SSD1306_WHITE);
  displayTela.drawPixel(80, 10, SSD1306_WHITE);
  displayTela.drawPixel(80, 9, SSD1306_WHITE);
  displayTela.drawPixel(79, 10, SSD1306_WHITE);
  displayTela.drawPixel(78, 12, SSD1306_WHITE);
  //id: 35 pixel 338 
  displayTela.drawPixel(79, 43, SSD1306_WHITE);
  displayTela.drawPixel(79, 44, SSD1306_WHITE);
  displayTela.drawPixel(80, 44, SSD1306_WHITE);
  displayTela.drawPixel(81, 44, SSD1306_WHITE);
  displayTela.drawPixel(81, 43, SSD1306_WHITE);
  displayTela.drawPixel(82, 43, SSD1306_WHITE);
  displayTela.drawPixel(83, 43, SSD1306_WHITE);
  displayTela.drawPixel(84, 43, SSD1306_WHITE);
  displayTela.drawPixel(85, 43, SSD1306_WHITE);
  displayTela.drawPixel(86, 43, SSD1306_WHITE);
  displayTela.drawPixel(86, 44, SSD1306_WHITE);
  displayTela.drawPixel(87, 44, SSD1306_WHITE);
  displayTela.drawPixel(88, 44, SSD1306_WHITE);
  displayTela.drawPixel(88, 45, SSD1306_WHITE);
  displayTela.drawPixel(89, 45, SSD1306_WHITE);
  displayTela.drawPixel(89, 46, SSD1306_WHITE);
  displayTela.drawPixel(89, 47, SSD1306_WHITE);
  displayTela.drawPixel(88, 46, SSD1306_WHITE);
  displayTela.drawPixel(87, 46, SSD1306_WHITE);
  displayTela.drawPixel(87, 45, SSD1306_WHITE);
  displayTela.drawPixel(86, 45, SSD1306_WHITE);
  displayTela.drawPixel(85, 45, SSD1306_WHITE);
  displayTela.drawPixel(84, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 45, SSD1306_WHITE);
  displayTela.drawPixel(82, 45, SSD1306_WHITE);
  displayTela.drawPixel(82, 44, SSD1306_WHITE);
  displayTela.drawPixel(80, 43, SSD1306_WHITE);
  displayTela.drawPixel(79, 45, SSD1306_WHITE);
  displayTela.drawPixel(80, 42, SSD1306_WHITE);
  displayTela.drawPixel(78, 43, SSD1306_WHITE);
  displayTela.drawPixel(78, 44, SSD1306_WHITE);
  displayTela.drawPixel(77, 44, SSD1306_WHITE);
  displayTela.drawPixel(79, 42, SSD1306_WHITE);
  displayTela.drawPixel(77, 45, SSD1306_WHITE);
  displayTela.drawPixel(78, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 44, SSD1306_WHITE);
  displayTela.drawPixel(84, 46, SSD1306_WHITE);
  displayTela.drawPixel(85, 46, SSD1306_WHITE);
  displayTela.drawPixel(85, 47, SSD1306_WHITE);
  displayTela.drawPixel(86, 47, SSD1306_WHITE);
  displayTela.drawPixel(86, 48, SSD1306_WHITE);
  displayTela.drawPixel(87, 49, SSD1306_WHITE);
  displayTela.drawPixel(88, 49, SSD1306_WHITE);
  displayTela.drawPixel(88, 48, SSD1306_WHITE);
  displayTela.drawPixel(88, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 44, SSD1306_WHITE);
  displayTela.drawPixel(84, 44, SSD1306_WHITE);
  displayTela.drawPixel(89, 48, SSD1306_WHITE);
  displayTela.drawPixel(89, 49, SSD1306_WHITE);
  displayTela.drawPixel(89, 50, SSD1306_WHITE);
  displayTela.drawPixel(87, 48, SSD1306_WHITE);
  displayTela.drawPixel(87, 47, SSD1306_WHITE);
  displayTela.drawPixel(86, 46, SSD1306_WHITE);
  //id: 36 pixel 339 
  displayTela.drawPixel(90, 46, SSD1306_BLACK);
  displayTela.drawPixel(89, 45, SSD1306_BLACK);
  displayTela.drawPixel(88, 45, SSD1306_BLACK);
  displayTela.drawPixel(88, 44, SSD1306_BLACK);
  displayTela.drawPixel(87, 44, SSD1306_BLACK);
  displayTela.drawPixel(86, 44, SSD1306_BLACK);
  displayTela.drawPixel(86, 43, SSD1306_BLACK);
  displayTela.drawPixel(85, 43, SSD1306_BLACK);
  displayTela.drawPixel(85, 42, SSD1306_BLACK);
  displayTela.drawPixel(86, 42, SSD1306_BLACK);
  displayTela.drawPixel(87, 42, SSD1306_BLACK);
  displayTela.drawPixel(88, 43, SSD1306_BLACK);
  displayTela.drawPixel(89, 43, SSD1306_BLACK);
  displayTela.drawPixel(90, 43, SSD1306_BLACK);
  displayTela.drawPixel(90, 44, SSD1306_BLACK);
  displayTela.drawPixel(91, 44, SSD1306_BLACK);
  displayTela.drawPixel(91, 45, SSD1306_BLACK);
  displayTela.drawPixel(91, 46, SSD1306_BLACK);
  displayTela.drawPixel(89, 46, SSD1306_BLACK);
  displayTela.drawPixel(88, 46, SSD1306_BLACK);
  displayTela.drawPixel(87, 43, SSD1306_BLACK);
  displayTela.drawPixel(88, 42, SSD1306_BLACK);
  displayTela.drawPixel(89, 44, SSD1306_BLACK);
  displayTela.drawPixel(89, 47, SSD1306_BLACK);
  displayTela.drawPixel(89, 48, SSD1306_BLACK);
  displayTela.drawPixel(89, 49, SSD1306_BLACK);
  displayTela.drawPixel(90, 49, SSD1306_BLACK);
  displayTela.drawPixel(90, 50, SSD1306_BLACK);
  displayTela.drawPixel(90, 51, SSD1306_BLACK);
  displayTela.drawPixel(91, 49, SSD1306_BLACK);
  displayTela.drawPixel(91, 48, SSD1306_BLACK);
  displayTela.drawPixel(91, 47, SSD1306_BLACK);
  //id: 37 pixel 340 
  displayTela.drawPixel(91, 46, SSD1306_BLACK);
  //id: 38 pixel 341 
  displayTela.drawPixel(48, 15, SSD1306_WHITE);
  displayTela.drawPixel(47, 15, SSD1306_WHITE);
  displayTela.drawPixel(46, 15, SSD1306_WHITE);
  displayTela.drawPixel(46, 14, SSD1306_WHITE);
  displayTela.drawPixel(45, 14, SSD1306_WHITE);
  displayTela.drawPixel(45, 13, SSD1306_WHITE);
  displayTela.drawPixel(45, 12, SSD1306_WHITE);
  displayTela.drawPixel(45, 11, SSD1306_WHITE);
  displayTela.drawPixel(46, 11, SSD1306_WHITE);
  displayTela.drawPixel(46, 10, SSD1306_WHITE);
  displayTela.drawPixel(46, 9, SSD1306_WHITE);
  displayTela.drawPixel(45, 9, SSD1306_WHITE);
  displayTela.drawPixel(45, 8, SSD1306_WHITE);
  displayTela.drawPixel(44, 8, SSD1306_WHITE);
  displayTela.drawPixel(43, 7, SSD1306_WHITE);
  displayTela.drawPixel(42, 7, SSD1306_WHITE);
  displayTela.drawPixel(42, 6, SSD1306_WHITE);
  displayTela.drawPixel(43, 6, SSD1306_WHITE);
  displayTela.drawPixel(44, 7, SSD1306_WHITE);
  displayTela.drawPixel(45, 10, SSD1306_WHITE);
  displayTela.drawPixel(46, 12, SSD1306_WHITE);
  displayTela.drawPixel(46, 13, SSD1306_WHITE);
  displayTela.drawPixel(47, 14, SSD1306_WHITE);
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  displayTela.drawPixel(49, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 16, SSD1306_WHITE);
  displayTela.drawPixel(47, 16, SSD1306_WHITE);
  displayTela.drawPixel(47, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 12, SSD1306_WHITE);
  displayTela.drawPixel(48, 11, SSD1306_WHITE);
  displayTela.drawPixel(47, 11, SSD1306_WHITE);
  displayTela.drawPixel(47, 10, SSD1306_WHITE);
  displayTela.drawPixel(47, 9, SSD1306_WHITE);
  displayTela.drawPixel(47, 8, SSD1306_WHITE);
  displayTela.drawPixel(46, 8, SSD1306_WHITE);
  displayTela.drawPixel(46, 7, SSD1306_WHITE);
  displayTela.drawPixel(45, 7, SSD1306_WHITE);
  displayTela.drawPixel(45, 6, SSD1306_WHITE);
  displayTela.drawPixel(44, 6, SSD1306_WHITE);
  displayTela.drawPixel(46, 6, SSD1306_WHITE);
  displayTela.drawPixel(47, 12, SSD1306_WHITE);
  displayTela.drawPixel(48, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 14, SSD1306_WHITE);
  displayTela.drawPixel(49, 14, SSD1306_WHITE);
  displayTela.drawPixel(50, 15, SSD1306_WHITE);
  displayTela.drawPixel(42, 5, SSD1306_WHITE);
  //id: 39 pixel 342 
  displayTela.drawPixel(46, 5, SSD1306_BLACK);
  displayTela.drawPixel(46, 6, SSD1306_BLACK);
  displayTela.drawPixel(47, 6, SSD1306_BLACK);
  displayTela.drawPixel(47, 7, SSD1306_BLACK);
  displayTela.drawPixel(48, 7, SSD1306_BLACK);
  displayTela.drawPixel(48, 8, SSD1306_BLACK);
  displayTela.drawPixel(49, 8, SSD1306_BLACK);
  displayTela.drawPixel(47, 8, SSD1306_BLACK);
  displayTela.drawPixel(46, 8, SSD1306_BLACK);
  displayTela.drawPixel(46, 7, SSD1306_BLACK);
  //id: 40 pixel 343 
  displayTela.drawPixel(48, 8, SSD1306_BLACK);
  //id: 41 pixel 344 
  displayTela.drawPixel(45, 8, SSD1306_WHITE);
  //id: 42 pixel 345 
  displayTela.drawPixel(46, 8, SSD1306_WHITE);
  //id: 43 pixel 346 
  displayTela.drawPixel(49, 15, SSD1306_WHITE);
  //id: 44 pixel 347 
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  displayTela.drawPixel(48, 17, SSD1306_WHITE);
  displayTela.drawPixel(49, 17, SSD1306_WHITE);
  displayTela.drawPixel(49, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 15, SSD1306_WHITE);
  displayTela.drawPixel(51, 15, SSD1306_WHITE);
  //id: 45 pixel 348 
  displayTela.drawPixel(51, 15, SSD1306_WHITE);
  displayTela.drawPixel(51, 14, SSD1306_WHITE);
  //id: 46 pixel 349 
  displayTela.drawPixel(51, 14, SSD1306_WHITE);
  //id: 47 pixel 350 
  displayTela.drawPixel(51, 14, SSD1306_WHITE);
  displayTela.drawPixel(51, 15, SSD1306_WHITE);
  displayTela.drawPixel(50, 15, SSD1306_WHITE);
  //id: 48 pixel 351 
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  //id: 49 pixel 352 
  displayTela.drawPixel(48, 17, SSD1306_WHITE);
  displayTela.drawPixel(47, 17, SSD1306_WHITE);
  //id: 50 pixel 353 
  displayTela.drawPixel(47, 18, SSD1306_WHITE);
  //id: 51 pixel 354 
  displayTela.drawPixel(62, 10, SSD1306_WHITE);
  displayTela.drawPixel(61, 10, SSD1306_WHITE);
  displayTela.drawPixel(60, 10, SSD1306_WHITE);
  displayTela.drawPixel(59, 10, SSD1306_WHITE);
  displayTela.drawPixel(58, 10, SSD1306_WHITE);
  displayTela.drawPixel(59, 9, SSD1306_WHITE);
  displayTela.drawPixel(60, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 9, SSD1306_WHITE);
  //id: 52 pixel 355 
  displayTela.drawPixel(65, 9, SSD1306_WHITE);
  displayTela.drawPixel(66, 9, SSD1306_WHITE);
  displayTela.drawPixel(66, 10, SSD1306_WHITE);
  displayTela.drawPixel(67, 10, SSD1306_WHITE);
  //id: 53 pixel 356 
  displayTela.drawPixel(67, 10, SSD1306_WHITE);
  //id: 54 pixel 357 
  displayTela.drawPixel(67, 10, SSD1306_WHITE);
  //id: 55 pixel 358 
  displayTela.drawPixel(77, 16, SSD1306_WHITE);
  //id: 56 pixel 359 
  displayTela.drawPixel(77, 16, SSD1306_WHITE);
  //id: 57 pixel 360 
  displayTela.drawPixel(76, 15, SSD1306_WHITE);
  //id: 58 pixel 361 
  displayTela.drawPixel(75, 15, SSD1306_WHITE);
  displayTela.drawPixel(75, 14, SSD1306_WHITE);
  //id: 59 pixel 362 
  displayTela.drawPixel(75, 14, SSD1306_WHITE);
  displayTela.drawPixel(74, 14, SSD1306_WHITE);
  //id: 60 pixel 363 
  displayTela.drawPixel(74, 14, SSD1306_WHITE);
  //id: 61 pixel 364 
  displayTela.drawPixel(83, 27, SSD1306_WHITE);
  displayTela.drawPixel(83, 26, SSD1306_WHITE);
  displayTela.drawPixel(82, 26, SSD1306_WHITE);
  //id: 62 pixel 365 
  displayTela.drawPixel(83, 33, SSD1306_WHITE);
  displayTela.drawPixel(83, 34, SSD1306_WHITE);
  displayTela.drawPixel(84, 33, SSD1306_WHITE);
  //id: 63 pixel 366 
  displayTela.drawPixel(84, 33, SSD1306_WHITE);
  displayTela.drawPixel(84, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 31, SSD1306_WHITE);
  displayTela.drawPixel(84, 30, SSD1306_WHITE);
  displayTela.drawPixel(84, 29, SSD1306_WHITE);
  displayTela.drawPixel(84, 28, SSD1306_WHITE);
  displayTela.drawPixel(84, 27, SSD1306_WHITE);
  //id: 64 pixel 367 
  displayTela.drawPixel(84, 27, SSD1306_WHITE);
  //id: 65 pixel 368 
  displayTela.drawPixel(82, 26, SSD1306_BLACK);
  //id: 66 pixel 369 
  displayTela.drawPixel(82, 25, SSD1306_WHITE);
  displayTela.drawPixel(83, 25, SSD1306_WHITE);
  //id: 67 pixel 370 
  displayTela.drawPixel(44, 25, SSD1306_WHITE);
  displayTela.drawPixel(43, 25, SSD1306_WHITE);
  displayTela.drawPixel(43, 26, SSD1306_WHITE);
  displayTela.drawPixel(42, 26, SSD1306_WHITE);
  displayTela.drawPixel(42, 27, SSD1306_WHITE);
  displayTela.drawPixel(43, 27, SSD1306_WHITE);
  //id: 68 pixel 371 
  displayTela.drawPixel(44, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 33, SSD1306_WHITE);
  //id: 69 pixel 372 
  displayTela.drawPixel(43, 33, SSD1306_WHITE);
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  //id: 70 pixel 373 
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  displayTela.drawPixel(43, 33, SSD1306_WHITE);
  displayTela.drawPixel(43, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 35, SSD1306_WHITE);
  displayTela.drawPixel(44, 35, SSD1306_WHITE);
  displayTela.drawPixel(42, 34, SSD1306_WHITE);
  displayTela.drawPixel(42, 33, SSD1306_WHITE);
  displayTela.drawPixel(42, 32, SSD1306_WHITE);
  displayTela.drawPixel(42, 31, SSD1306_WHITE);
  displayTela.drawPixel(42, 30, SSD1306_WHITE);
  //id: 71 pixel 374 
  displayTela.drawPixel(44, 34, SSD1306_BLACK);
  displayTela.drawPixel(44, 35, SSD1306_BLACK);
  displayTela.drawPixel(43, 35, SSD1306_BLACK);
  displayTela.drawPixel(43, 34, SSD1306_BLACK);
  displayTela.drawPixel(43, 33, SSD1306_BLACK);
  displayTela.drawPixel(42, 35, SSD1306_BLACK);
  displayTela.drawPixel(42, 34, SSD1306_BLACK);
  displayTela.drawPixel(42, 33, SSD1306_BLACK);
  //id: 72 pixel 375 
  displayTela.drawPixel(48, 43, SSD1306_WHITE);
  displayTela.drawPixel(49, 43, SSD1306_WHITE);
  displayTela.drawPixel(49, 44, SSD1306_WHITE);
  displayTela.drawPixel(50, 44, SSD1306_WHITE);
  //id: 73 pixel 376 
  displayTela.drawPixel(50, 45, SSD1306_WHITE);
  //id: 74 pixel 377 
  displayTela.drawPixel(51, 45, SSD1306_WHITE);
  //id: 75 pixel 378 
  displayTela.drawPixel(62, 50, SSD1306_WHITE);
  //id: 76 pixel 379 
  displayTela.drawPixel(61, 50, SSD1306_WHITE);
  //id: 77 pixel 380 
  displayTela.drawPixel(80, 41, SSD1306_WHITE);
  //id: 78 pixel 381 
  displayTela.drawPixel(80, 41, SSD1306_WHITE);
  //id: 79 pixel 382 
  displayTela.drawPixel(81, 41, SSD1306_WHITE);
  //id: 80 pixel 383 
  displayTela.drawPixel(81, 42, SSD1306_WHITE);
  //id: 81 pixel 384 
  displayTela.drawPixel(85, 48, SSD1306_WHITE);
  //id: 82 pixel 385 
  displayTela.drawPixel(85, 48, SSD1306_WHITE);
  displayTela.drawPixel(84, 48, SSD1306_WHITE);
  displayTela.drawPixel(83, 48, SSD1306_WHITE);
  displayTela.drawPixel(83, 47, SSD1306_WHITE);
  displayTela.drawPixel(83, 46, SSD1306_WHITE);
  displayTela.drawPixel(83, 45, SSD1306_WHITE);
  //id: 83 pixel 386 
  displayTela.drawPixel(83, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 46, SSD1306_WHITE);
  displayTela.drawPixel(84, 46, SSD1306_WHITE);
  displayTela.drawPixel(84, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 48, SSD1306_WHITE);
  displayTela.drawPixel(86, 48, SSD1306_WHITE);
  displayTela.drawPixel(86, 49, SSD1306_WHITE);
  displayTela.drawPixel(87, 49, SSD1306_WHITE);
  displayTela.drawPixel(87, 50, SSD1306_WHITE);
  displayTela.drawPixel(87, 51, SSD1306_WHITE);
  displayTela.drawPixel(87, 48, SSD1306_WHITE);
  displayTela.drawPixel(88, 50, SSD1306_WHITE);
  displayTela.drawPixel(88, 51, SSD1306_WHITE);
  displayTela.drawPixel(88, 49, SSD1306_WHITE);
  //id: 84 pixel 387 
  displayTela.drawPixel(86, 45, SSD1306_WHITE);
  displayTela.drawPixel(87, 45, SSD1306_WHITE);
  displayTela.drawPixel(87, 46, SSD1306_WHITE);
  displayTela.drawPixel(88, 46, SSD1306_WHITE);
  displayTela.drawPixel(88, 47, SSD1306_WHITE);
  displayTela.drawPixel(89, 47, SSD1306_WHITE);
  displayTela.drawPixel(89, 48, SSD1306_WHITE);
  displayTela.drawPixel(90, 49, SSD1306_WHITE);
  displayTela.drawPixel(90, 48, SSD1306_WHITE);
  //id: 85 pixel 388 
  displayTela.drawPixel(90, 48, SSD1306_BLACK);
  displayTela.drawPixel(89, 48, SSD1306_BLACK);
  //id: 86 pixel 389 
  displayTela.drawPixel(89, 47, SSD1306_BLACK);
  displayTela.drawPixel(90, 47, SSD1306_BLACK);
  displayTela.drawPixel(90, 48, SSD1306_BLACK);
  displayTela.drawPixel(91, 48, SSD1306_BLACK);
  displayTela.drawPixel(91, 49, SSD1306_BLACK);
  displayTela.drawPixel(91, 50, SSD1306_BLACK);
  displayTela.drawPixel(89, 48, SSD1306_BLACK);
  displayTela.drawPixel(88, 46, SSD1306_BLACK);
  displayTela.drawPixel(88, 45, SSD1306_BLACK);
  displayTela.drawPixel(87, 45, SSD1306_BLACK);
  displayTela.drawPixel(87, 44, SSD1306_BLACK);
  displayTela.drawPixel(86, 44, SSD1306_BLACK);
  displayTela.drawPixel(86, 43, SSD1306_BLACK);
  displayTela.drawPixel(85, 43, SSD1306_BLACK);
  displayTela.drawPixel(85, 42, SSD1306_BLACK);
  displayTela.drawPixel(86, 42, SSD1306_BLACK);
  displayTela.drawPixel(87, 43, SSD1306_BLACK);
  displayTela.drawPixel(88, 44, SSD1306_BLACK);
  displayTela.drawPixel(89, 45, SSD1306_BLACK);
  displayTela.drawPixel(89, 46, SSD1306_BLACK);
  displayTela.drawPixel(90, 49, SSD1306_BLACK);
  //id: 87 pixel 390 
  displayTela.drawPixel(83, 47, SSD1306_BLACK);
  displayTela.drawPixel(83, 48, SSD1306_BLACK);
  displayTela.drawPixel(84, 48, SSD1306_BLACK);
  displayTela.drawPixel(85, 48, SSD1306_BLACK);
  displayTela.drawPixel(85, 47, SSD1306_BLACK);
  displayTela.drawPixel(84, 49, SSD1306_BLACK);
  displayTela.drawPixel(84, 47, SSD1306_BLACK);
  displayTela.drawPixel(84, 46, SSD1306_BLACK);
  displayTela.drawPixel(83, 46, SSD1306_BLACK);
  //id: 88 pixel 391 
  displayTela.drawPixel(84, 47, SSD1306_BLACK);
  displayTela.drawPixel(84, 48, SSD1306_BLACK);
  //id: 89 pixel 392 
  displayTela.drawPixel(84, 48, SSD1306_BLACK);
  displayTela.drawPixel(84, 47, SSD1306_BLACK);
  displayTela.drawPixel(84, 46, SSD1306_BLACK);
  displayTela.drawPixel(84, 45, SSD1306_BLACK);
  displayTela.drawPixel(83, 47, SSD1306_BLACK);
  displayTela.drawPixel(83, 48, SSD1306_BLACK);
  displayTela.drawPixel(83, 46, SSD1306_BLACK);
  displayTela.drawPixel(82, 46, SSD1306_BLACK);
  //id: 90 pixel 393 
  displayTela.drawPixel(83, 47, SSD1306_BLACK);
  //id: 91 pixel 394 
  displayTela.drawPixel(79, 44, SSD1306_WHITE);
  displayTela.drawPixel(80, 44, SSD1306_WHITE);
  displayTela.drawPixel(81, 44, SSD1306_WHITE);
  displayTela.drawPixel(82, 44, SSD1306_WHITE);
  displayTela.drawPixel(82, 45, SSD1306_WHITE);
  displayTela.drawPixel(83, 45, SSD1306_WHITE);
  displayTela.drawPixel(84, 45, SSD1306_WHITE);
  displayTela.drawPixel(84, 46, SSD1306_WHITE);
  displayTela.drawPixel(84, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 48, SSD1306_WHITE);
  displayTela.drawPixel(85, 49, SSD1306_WHITE);
  displayTela.drawPixel(86, 50, SSD1306_WHITE);
  displayTela.drawPixel(86, 51, SSD1306_WHITE);
  displayTela.drawPixel(87, 51, SSD1306_WHITE);
  displayTela.drawPixel(87, 52, SSD1306_WHITE);
  displayTela.drawPixel(88, 52, SSD1306_WHITE);
  displayTela.drawPixel(89, 52, SSD1306_WHITE);
  displayTela.drawPixel(90, 52, SSD1306_WHITE);
  displayTela.drawPixel(91, 52, SSD1306_WHITE);
  displayTela.drawPixel(90, 51, SSD1306_WHITE);
  displayTela.drawPixel(91, 51, SSD1306_WHITE);
  displayTela.drawPixel(89, 51, SSD1306_WHITE);
  displayTela.drawPixel(88, 51, SSD1306_WHITE);
  //id: 92 pixel 395 
  displayTela.drawPixel(87, 46, SSD1306_BLACK);
  displayTela.drawPixel(88, 46, SSD1306_BLACK);
  displayTela.drawPixel(88, 47, SSD1306_BLACK);
  displayTela.drawPixel(89, 47, SSD1306_BLACK);
  displayTela.drawPixel(90, 47, SSD1306_BLACK);
  displayTela.drawPixel(90, 48, SSD1306_BLACK);
  displayTela.drawPixel(91, 48, SSD1306_BLACK);
  displayTela.drawPixel(91, 49, SSD1306_BLACK);
  displayTela.drawPixel(90, 49, SSD1306_BLACK);
  displayTela.drawPixel(89, 49, SSD1306_BLACK);
  displayTela.drawPixel(89, 48, SSD1306_BLACK);
  //id: 93 pixel 396 
  displayTela.drawPixel(88, 52, SSD1306_BLACK);
  displayTela.drawPixel(87, 52, SSD1306_BLACK);
  displayTela.drawPixel(87, 51, SSD1306_BLACK);
  displayTela.drawPixel(86, 51, SSD1306_BLACK);
  displayTela.drawPixel(86, 50, SSD1306_BLACK);
  displayTela.drawPixel(87, 53, SSD1306_BLACK);
  displayTela.drawPixel(86, 53, SSD1306_BLACK);
  displayTela.drawPixel(86, 52, SSD1306_BLACK);
  displayTela.drawPixel(85, 52, SSD1306_BLACK);
  displayTela.drawPixel(85, 51, SSD1306_BLACK);
  displayTela.drawPixel(85, 50, SSD1306_BLACK);
  displayTela.drawPixel(85, 49, SSD1306_BLACK);
  displayTela.drawPixel(84, 49, SSD1306_BLACK);
  displayTela.drawPixel(84, 48, SSD1306_BLACK);
  displayTela.drawPixel(83, 48, SSD1306_BLACK);
  displayTela.drawPixel(82, 48, SSD1306_BLACK);
  displayTela.drawPixel(82, 49, SSD1306_BLACK);
  //id: 94 pixel 397 
  displayTela.drawPixel(45, 20, SSD1306_WHITE);
  displayTela.drawPixel(45, 21, SSD1306_WHITE);
  displayTela.drawPixel(44, 21, SSD1306_WHITE);
  displayTela.drawPixel(43, 21, SSD1306_WHITE);
  displayTela.drawPixel(43, 20, SSD1306_WHITE);
  displayTela.drawPixel(42, 20, SSD1306_WHITE);
  displayTela.drawPixel(42, 19, SSD1306_WHITE);
  displayTela.drawPixel(41, 19, SSD1306_WHITE);
  displayTela.drawPixel(41, 18, SSD1306_WHITE);
  displayTela.drawPixel(40, 18, SSD1306_WHITE);
  displayTela.drawPixel(39, 18, SSD1306_WHITE);
  displayTela.drawPixel(39, 17, SSD1306_WHITE);
  displayTela.drawPixel(38, 17, SSD1306_WHITE);
  displayTela.drawPixel(39, 16, SSD1306_WHITE);
  displayTela.drawPixel(39, 15, SSD1306_WHITE);
  displayTela.drawPixel(39, 14, SSD1306_WHITE);
  displayTela.drawPixel(38, 14, SSD1306_WHITE);
  displayTela.drawPixel(37, 14, SSD1306_WHITE);
  displayTela.drawPixel(38, 15, SSD1306_WHITE);
  displayTela.drawPixel(40, 17, SSD1306_WHITE);
  displayTela.drawPixel(41, 17, SSD1306_WHITE);
  displayTela.drawPixel(42, 18, SSD1306_WHITE);
  displayTela.drawPixel(44, 20, SSD1306_WHITE);
  displayTela.drawPixel(44, 22, SSD1306_WHITE);
  displayTela.drawPixel(45, 22, SSD1306_WHITE);
  displayTela.drawPixel(38, 16, SSD1306_WHITE);
  displayTela.drawPixel(37, 16, SSD1306_WHITE);
  displayTela.drawPixel(37, 15, SSD1306_WHITE);
  displayTela.drawPixel(36, 15, SSD1306_WHITE);
  displayTela.drawPixel(35, 15, SSD1306_WHITE);
  displayTela.drawPixel(40, 16, SSD1306_WHITE);
  displayTela.drawPixel(43, 19, SSD1306_WHITE);
  displayTela.drawPixel(44, 19, SSD1306_WHITE);
  displayTela.drawPixel(38, 18, SSD1306_WHITE);
  displayTela.drawPixel(36, 16, SSD1306_WHITE);
  displayTela.drawPixel(35, 16, SSD1306_WHITE);
  displayTela.drawPixel(36, 17, SSD1306_WHITE);
  displayTela.drawPixel(35, 17, SSD1306_WHITE);
  displayTela.drawPixel(34, 17, SSD1306_WHITE);
  displayTela.drawPixel(35, 18, SSD1306_WHITE);
  displayTela.drawPixel(36, 18, SSD1306_WHITE);
  displayTela.drawPixel(34, 18, SSD1306_WHITE);
  displayTela.drawPixel(37, 18, SSD1306_WHITE);
  displayTela.drawPixel(37, 17, SSD1306_WHITE);
  displayTela.drawPixel(34, 15, SSD1306_WHITE);
  displayTela.drawPixel(33, 15, SSD1306_WHITE);
  displayTela.drawPixel(33, 16, SSD1306_WHITE);
  displayTela.drawPixel(34, 16, SSD1306_WHITE);
  //id: 95 pixel 398 
  displayTela.drawPixel(37, 14, SSD1306_BLACK);
  displayTela.drawPixel(37, 13, SSD1306_BLACK);
  displayTela.drawPixel(38, 14, SSD1306_BLACK);
  displayTela.drawPixel(39, 14, SSD1306_BLACK);
  displayTela.drawPixel(39, 15, SSD1306_BLACK);
  displayTela.drawPixel(40, 15, SSD1306_BLACK);
  displayTela.drawPixel(41, 15, SSD1306_BLACK);
  displayTela.drawPixel(42, 15, SSD1306_BLACK);
  displayTela.drawPixel(43, 15, SSD1306_BLACK);
  displayTela.drawPixel(43, 16, SSD1306_BLACK);
  //id: 96 pixel 399 
  displayTela.drawPixel(37, 18, SSD1306_BLACK);
  displayTela.drawPixel(36, 18, SSD1306_BLACK);
  displayTela.drawPixel(35, 17, SSD1306_BLACK);
  displayTela.drawPixel(34, 17, SSD1306_BLACK);
  displayTela.drawPixel(34, 16, SSD1306_BLACK);
  displayTela.drawPixel(34, 15, SSD1306_BLACK);
  displayTela.drawPixel(34, 14, SSD1306_BLACK);
  displayTela.drawPixel(35, 14, SSD1306_BLACK);
  displayTela.drawPixel(35, 13, SSD1306_BLACK);
  displayTela.drawPixel(34, 13, SSD1306_BLACK);
  displayTela.drawPixel(34, 18, SSD1306_BLACK);
  displayTela.drawPixel(33, 17, SSD1306_BLACK);
  displayTela.drawPixel(33, 16, SSD1306_BLACK);
  displayTela.drawPixel(32, 16, SSD1306_BLACK);
  displayTela.drawPixel(32, 15, SSD1306_BLACK);
  displayTela.drawPixel(32, 14, SSD1306_BLACK);
  displayTela.drawPixel(31, 14, SSD1306_BLACK);
  displayTela.drawPixel(31, 13, SSD1306_BLACK);
  displayTela.drawPixel(32, 13, SSD1306_BLACK);
  displayTela.drawPixel(33, 15, SSD1306_BLACK);
  displayTela.drawPixel(35, 18, SSD1306_BLACK);
  displayTela.drawPixel(36, 19, SSD1306_BLACK);
  displayTela.drawPixel(36, 20, SSD1306_BLACK);
  displayTela.drawPixel(36, 21, SSD1306_BLACK);
  displayTela.drawPixel(36, 17, SSD1306_BLACK);
  displayTela.drawPixel(35, 16, SSD1306_BLACK);
  displayTela.drawPixel(35, 15, SSD1306_BLACK);
  displayTela.drawPixel(37, 19, SSD1306_BLACK);
  displayTela.drawPixel(37, 20, SSD1306_BLACK);
  displayTela.drawPixel(37, 17, SSD1306_BLACK);
  displayTela.drawPixel(37, 16, SSD1306_BLACK);
  displayTela.drawPixel(37, 15, SSD1306_BLACK);
  displayTela.drawPixel(36, 14, SSD1306_BLACK);
  displayTela.drawPixel(36, 13, SSD1306_BLACK);
  displayTela.drawPixel(36, 12, SSD1306_BLACK);
  displayTela.drawPixel(36, 15, SSD1306_BLACK);
  displayTela.drawPixel(36, 16, SSD1306_BLACK);
  displayTela.drawPixel(35, 12, SSD1306_BLACK);
  displayTela.drawPixel(35, 11, SSD1306_BLACK);
  displayTela.drawPixel(35, 10, SSD1306_BLACK);
  displayTela.drawPixel(35, 19, SSD1306_BLACK);
  displayTela.drawPixel(38, 20, SSD1306_BLACK);
  displayTela.drawPixel(38, 19, SSD1306_BLACK);
  displayTela.drawPixel(38, 18, SSD1306_BLACK);
  displayTela.drawPixel(37, 14, SSD1306_BLACK);
  displayTela.drawPixel(37, 13, SSD1306_BLACK);
  displayTela.drawPixel(38, 17, SSD1306_BLACK);
  displayTela.drawPixel(39, 19, SSD1306_BLACK);
  displayTela.drawPixel(39, 20, SSD1306_BLACK);
  displayTela.drawPixel(39, 18, SSD1306_BLACK);
  displayTela.drawPixel(39, 17, SSD1306_BLACK);
  displayTela.drawPixel(39, 16, SSD1306_BLACK);
  displayTela.drawPixel(38, 15, SSD1306_BLACK);
  displayTela.drawPixel(38, 14, SSD1306_BLACK);
  displayTela.drawPixel(38, 13, SSD1306_BLACK);
  displayTela.drawPixel(38, 16, SSD1306_BLACK);
  displayTela.drawPixel(37, 11, SSD1306_BLACK);
  displayTela.drawPixel(36, 10, SSD1306_BLACK);
  displayTela.drawPixel(36, 9, SSD1306_BLACK);
  displayTela.drawPixel(35, 9, SSD1306_BLACK);
  //id: 97 pixel 400 
  displayTela.drawPixel(45, 38, SSD1306_WHITE);
  displayTela.drawPixel(44, 38, SSD1306_WHITE);
  displayTela.drawPixel(43, 38, SSD1306_WHITE);
  displayTela.drawPixel(42, 38, SSD1306_WHITE);
  displayTela.drawPixel(41, 38, SSD1306_WHITE);
  displayTela.drawPixel(41, 39, SSD1306_WHITE);
  displayTela.drawPixel(41, 40, SSD1306_WHITE);
  displayTela.drawPixel(41, 41, SSD1306_WHITE);
  displayTela.drawPixel(40, 41, SSD1306_WHITE);
  displayTela.drawPixel(40, 42, SSD1306_WHITE);
  displayTela.drawPixel(39, 42, SSD1306_WHITE);
  displayTela.drawPixel(42, 40, SSD1306_WHITE);
  displayTela.drawPixel(42, 39, SSD1306_WHITE);
  displayTela.drawPixel(43, 39, SSD1306_WHITE);
  displayTela.drawPixel(44, 37, SSD1306_WHITE);
  displayTela.drawPixel(44, 36, SSD1306_WHITE);
  displayTela.drawPixel(45, 37, SSD1306_WHITE);
  displayTela.drawPixel(45, 39, SSD1306_WHITE);
  displayTela.drawPixel(46, 39, SSD1306_WHITE);
  displayTela.drawPixel(46, 40, SSD1306_WHITE);
  displayTela.drawPixel(45, 40, SSD1306_WHITE);
  displayTela.drawPixel(44, 39, SSD1306_WHITE);
  displayTela.drawPixel(44, 40, SSD1306_WHITE);
  displayTela.drawPixel(43, 40, SSD1306_WHITE);
  displayTela.drawPixel(42, 41, SSD1306_WHITE);
  displayTela.drawPixel(42, 37, SSD1306_WHITE);
  displayTela.drawPixel(43, 37, SSD1306_WHITE);
  //id: 98 pixel 401 
  displayTela.drawPixel(55, 48, SSD1306_WHITE);
  displayTela.drawPixel(55, 49, SSD1306_WHITE);
  displayTela.drawPixel(55, 50, SSD1306_WHITE);
  displayTela.drawPixel(55, 51, SSD1306_WHITE);
  displayTela.drawPixel(55, 52, SSD1306_WHITE);
  displayTela.drawPixel(55, 53, SSD1306_WHITE);
  displayTela.drawPixel(55, 54, SSD1306_WHITE);
  displayTela.drawPixel(55, 55, SSD1306_WHITE);
  displayTela.drawPixel(54, 55, SSD1306_WHITE);
  displayTela.drawPixel(54, 56, SSD1306_WHITE);
  displayTela.drawPixel(54, 57, SSD1306_WHITE);
  displayTela.drawPixel(56, 54, SSD1306_WHITE);
  displayTela.drawPixel(56, 53, SSD1306_WHITE);
  displayTela.drawPixel(56, 52, SSD1306_WHITE);
  displayTela.drawPixel(56, 49, SSD1306_WHITE);
  displayTela.drawPixel(56, 48, SSD1306_WHITE);
  displayTela.drawPixel(57, 48, SSD1306_WHITE);
  displayTela.drawPixel(57, 49, SSD1306_WHITE);
  displayTela.drawPixel(57, 50, SSD1306_WHITE);
  displayTela.drawPixel(57, 51, SSD1306_WHITE);
  displayTela.drawPixel(56, 51, SSD1306_WHITE);
  displayTela.drawPixel(56, 50, SSD1306_WHITE);
  displayTela.drawPixel(54, 48, SSD1306_WHITE);
  displayTela.drawPixel(54, 47, SSD1306_WHITE);
  displayTela.drawPixel(54, 49, SSD1306_WHITE);
  displayTela.drawPixel(54, 50, SSD1306_WHITE);
  displayTela.drawPixel(58, 50, SSD1306_WHITE);
  displayTela.drawPixel(58, 49, SSD1306_WHITE);
  //id: 99 pixel 402 
  displayTela.drawPixel(72, 48, SSD1306_WHITE);
  displayTela.drawPixel(73, 48, SSD1306_WHITE);
  displayTela.drawPixel(73, 49, SSD1306_WHITE);
  displayTela.drawPixel(74, 49, SSD1306_WHITE);
  displayTela.drawPixel(75, 49, SSD1306_WHITE);
  displayTela.drawPixel(75, 50, SSD1306_WHITE);
  displayTela.drawPixel(76, 50, SSD1306_WHITE);
  displayTela.drawPixel(76, 51, SSD1306_WHITE);
  displayTela.drawPixel(77, 52, SSD1306_WHITE);
  displayTela.drawPixel(77, 53, SSD1306_WHITE);
  displayTela.drawPixel(78, 53, SSD1306_WHITE);
  displayTela.drawPixel(78, 54, SSD1306_WHITE);
  displayTela.drawPixel(79, 55, SSD1306_WHITE);
  displayTela.drawPixel(78, 55, SSD1306_WHITE);
  displayTela.drawPixel(77, 55, SSD1306_WHITE);
  displayTela.drawPixel(77, 54, SSD1306_WHITE);
  displayTela.drawPixel(76, 53, SSD1306_WHITE);
  displayTela.drawPixel(76, 52, SSD1306_WHITE);
  displayTela.drawPixel(75, 52, SSD1306_WHITE);
  displayTela.drawPixel(75, 51, SSD1306_WHITE);
  displayTela.drawPixel(74, 51, SSD1306_WHITE);
  displayTela.drawPixel(73, 51, SSD1306_WHITE);
  displayTela.drawPixel(72, 51, SSD1306_WHITE);
  displayTela.drawPixel(72, 50, SSD1306_WHITE);
  displayTela.drawPixel(71, 50, SSD1306_WHITE);
  displayTela.drawPixel(72, 49, SSD1306_WHITE);
  displayTela.drawPixel(74, 50, SSD1306_WHITE);
  displayTela.drawPixel(77, 51, SSD1306_WHITE);
  displayTela.drawPixel(76, 49, SSD1306_WHITE);
  displayTela.drawPixel(76, 48, SSD1306_WHITE);
  displayTela.drawPixel(75, 48, SSD1306_WHITE);
  displayTela.drawPixel(75, 47, SSD1306_WHITE);
  displayTela.drawPixel(74, 47, SSD1306_WHITE);
  displayTela.drawPixel(74, 48, SSD1306_WHITE);
  displayTela.drawPixel(73, 50, SSD1306_WHITE);
  //id: 100 pixel 403 
  displayTela.drawPixel(83, 38, SSD1306_WHITE);
  displayTela.drawPixel(84, 38, SSD1306_WHITE);
  displayTela.drawPixel(85, 38, SSD1306_WHITE);
  displayTela.drawPixel(86, 38, SSD1306_WHITE);
  displayTela.drawPixel(87, 38, SSD1306_WHITE);
  displayTela.drawPixel(88, 38, SSD1306_WHITE);
  displayTela.drawPixel(89, 38, SSD1306_WHITE);
  displayTela.drawPixel(89, 39, SSD1306_WHITE);
  displayTela.drawPixel(90, 39, SSD1306_WHITE);
  displayTela.drawPixel(91, 39, SSD1306_WHITE);
  displayTela.drawPixel(92, 39, SSD1306_WHITE);
  displayTela.drawPixel(90, 38, SSD1306_WHITE);
  displayTela.drawPixel(87, 37, SSD1306_WHITE);
  displayTela.drawPixel(86, 37, SSD1306_WHITE);
  displayTela.drawPixel(85, 37, SSD1306_WHITE);
  displayTela.drawPixel(85, 36, SSD1306_WHITE);
  displayTela.drawPixel(84, 36, SSD1306_WHITE);
  displayTela.drawPixel(83, 36, SSD1306_WHITE);
  displayTela.drawPixel(83, 37, SSD1306_WHITE);
  displayTela.drawPixel(84, 37, SSD1306_WHITE);
  displayTela.drawPixel(88, 39, SSD1306_WHITE);
  displayTela.drawPixel(87, 39, SSD1306_WHITE);
  displayTela.drawPixel(86, 39, SSD1306_WHITE);
  displayTela.drawPixel(85, 39, SSD1306_WHITE);
  displayTela.drawPixel(84, 39, SSD1306_WHITE);
  displayTela.drawPixel(83, 39, SSD1306_WHITE);
  displayTela.drawPixel(82, 39, SSD1306_WHITE);
  displayTela.drawPixel(82, 38, SSD1306_WHITE);
  //id: 101 pixel 404 
  displayTela.drawPixel(83, 37, SSD1306_WHITE);
  //id: 102 pixel 405 
  displayTela.drawPixel(82, 37, SSD1306_WHITE);
  //id: 103 pixel 406 
  displayTela.drawPixel(88, 40, SSD1306_WHITE);
  displayTela.drawPixel(87, 40, SSD1306_WHITE);
  displayTela.drawPixel(86, 40, SSD1306_WHITE);
  displayTela.drawPixel(85, 40, SSD1306_WHITE);
  //id: 104 pixel 407 
  displayTela.drawPixel(81, 21, SSD1306_WHITE);
  displayTela.drawPixel(82, 21, SSD1306_WHITE);
  displayTela.drawPixel(83, 21, SSD1306_WHITE);
  displayTela.drawPixel(83, 20, SSD1306_WHITE);
  displayTela.drawPixel(84, 20, SSD1306_WHITE);
  displayTela.drawPixel(85, 20, SSD1306_WHITE);
  displayTela.drawPixel(85, 19, SSD1306_WHITE);
  displayTela.drawPixel(86, 19, SSD1306_WHITE);
  displayTela.drawPixel(87, 19, SSD1306_WHITE);
  displayTela.drawPixel(87, 18, SSD1306_WHITE);
  displayTela.drawPixel(88, 18, SSD1306_WHITE);
  displayTela.drawPixel(89, 18, SSD1306_WHITE);
  displayTela.drawPixel(86, 18, SSD1306_WHITE);
  displayTela.drawPixel(85, 18, SSD1306_WHITE);
  displayTela.drawPixel(84, 19, SSD1306_WHITE);
  displayTela.drawPixel(83, 19, SSD1306_WHITE);
  displayTela.drawPixel(82, 19, SSD1306_WHITE);
  displayTela.drawPixel(81, 19, SSD1306_WHITE);
  displayTela.drawPixel(81, 20, SSD1306_WHITE);
  displayTela.drawPixel(80, 20, SSD1306_WHITE);
  displayTela.drawPixel(82, 20, SSD1306_WHITE);
  //id: 105 pixel 408 
  displayTela.drawPixel(53, 11, SSD1306_WHITE);
  displayTela.drawPixel(53, 10, SSD1306_WHITE);
  displayTela.drawPixel(53, 9, SSD1306_WHITE);
  displayTela.drawPixel(53, 8, SSD1306_WHITE);
  displayTela.drawPixel(53, 7, SSD1306_WHITE);
  displayTela.drawPixel(53, 6, SSD1306_WHITE);
  displayTela.drawPixel(53, 5, SSD1306_WHITE);
  displayTela.drawPixel(54, 10, SSD1306_WHITE);
  displayTela.drawPixel(55, 10, SSD1306_WHITE);
  displayTela.drawPixel(55, 11, SSD1306_WHITE);
  displayTela.drawPixel(54, 11, SSD1306_WHITE);
  displayTela.drawPixel(53, 12, SSD1306_WHITE);
  displayTela.drawPixel(54, 12, SSD1306_WHITE);
  displayTela.drawPixel(56, 11, SSD1306_WHITE);
  displayTela.drawPixel(56, 10, SSD1306_WHITE);
  displayTela.drawPixel(52, 9, SSD1306_WHITE);
  displayTela.drawPixel(52, 8, SSD1306_WHITE);
  displayTela.drawPixel(52, 7, SSD1306_WHITE);
  displayTela.drawPixel(52, 6, SSD1306_WHITE);
  displayTela.drawPixel(52, 5, SSD1306_WHITE);
  displayTela.drawPixel(52, 4, SSD1306_WHITE);
  displayTela.drawPixel(51, 4, SSD1306_WHITE);
  displayTela.drawPixel(51, 3, SSD1306_WHITE);
  displayTela.drawPixel(52, 3, SSD1306_WHITE);
  displayTela.drawPixel(54, 7, SSD1306_WHITE);
  displayTela.drawPixel(54, 8, SSD1306_WHITE);
  displayTela.drawPixel(54, 9, SSD1306_WHITE);
  displayTela.drawPixel(55, 9, SSD1306_WHITE);
  //id: 106 pixel 434 
  displayTela.drawPixel(42, 56, SSD1306_WHITE);
  //id: 107 pixel 435 
  displayTela.drawPixel(44, 56, SSD1306_BLACK);
  displayTela.drawPixel(44, 55, SSD1306_BLACK);
  //id: 108 pixel 436 
  displayTela.drawPixel(92, 31, SSD1306_BLACK);
  displayTela.drawPixel(91, 31, SSD1306_BLACK);
  displayTela.drawPixel(90, 31, SSD1306_BLACK);
  displayTela.drawPixel(89, 31, SSD1306_BLACK);
  displayTela.drawPixel(88, 32, SSD1306_BLACK);
  displayTela.drawPixel(90, 30, SSD1306_BLACK);
  displayTela.drawPixel(91, 30, SSD1306_BLACK);
  displayTela.drawPixel(92, 30, SSD1306_BLACK);
  displayTela.drawPixel(88, 31, SSD1306_BLACK);
  displayTela.drawPixel(87, 32, SSD1306_BLACK);
  //id: 109 pixel 437 
  displayTela.drawPixel(92, 28, SSD1306_WHITE);
  displayTela.drawPixel(92, 27, SSD1306_WHITE);
  displayTela.drawPixel(93, 27, SSD1306_WHITE);
  //id: 110 pixel 438 
  displayTela.drawPixel(75, 15, SSD1306_BLACK);
  //id: 111 pixel 439 
  displayTela.drawPixel(74, 14, SSD1306_BLACK);
  //id: 112 pixel 440 
  displayTela.drawPixel(82, 25, SSD1306_BLACK);
  //id: 113 pixel 441 
  displayTela.drawPixel(51, 45, SSD1306_BLACK);
  displayTela.drawPixel(50, 45, SSD1306_BLACK);
  displayTela.drawPixel(50, 44, SSD1306_BLACK);
  displayTela.drawPixel(49, 44, SSD1306_BLACK);
  displayTela.drawPixel(49, 43, SSD1306_BLACK);
  displayTela.drawPixel(49, 42, SSD1306_BLACK);
  displayTela.drawPixel(48, 42, SSD1306_BLACK);
  displayTela.drawPixel(48, 41, SSD1306_BLACK);
  displayTela.drawPixel(48, 40, SSD1306_BLACK);
  displayTela.drawPixel(48, 39, SSD1306_BLACK);
  //id: 114 pixel 442 
  displayTela.drawPixel(47, 39, SSD1306_BLACK);
  displayTela.drawPixel(46, 39, SSD1306_BLACK);
  displayTela.drawPixel(46, 38, SSD1306_BLACK);
  displayTela.drawPixel(46, 37, SSD1306_BLACK);
  displayTela.drawPixel(45, 37, SSD1306_BLACK);
  displayTela.drawPixel(45, 36, SSD1306_BLACK);
  //id: 115 pixel 443 
  displayTela.drawPixel(59, 49, SSD1306_BLACK);
  displayTela.drawPixel(58, 49, SSD1306_BLACK);
  displayTela.drawPixel(58, 48, SSD1306_BLACK);
  displayTela.drawPixel(57, 48, SSD1306_BLACK);
  displayTela.drawPixel(56, 48, SSD1306_BLACK);
  displayTela.drawPixel(56, 47, SSD1306_BLACK);
  displayTela.drawPixel(55, 47, SSD1306_BLACK);
  displayTela.drawPixel(54, 47, SSD1306_BLACK);
  displayTela.drawPixel(54, 46, SSD1306_BLACK);
  //id: 116 pixel 452 
  displayTela.drawPixel(91, 6, SSD1306_BLACK);
  displayTela.drawPixel(91, 5, SSD1306_BLACK);
  displayTela.drawPixel(91, 4, SSD1306_BLACK);
  displayTela.drawPixel(92, 4, SSD1306_BLACK);
  displayTela.drawPixel(93, 2, SSD1306_BLACK);
  displayTela.drawPixel(94, 1, SSD1306_BLACK);
  displayTela.drawPixel(97, 0, SSD1306_BLACK);
  displayTela.drawPixel(102, 30, SSD1306_BLACK);
  displayTela.drawPixel(103, 30, SSD1306_BLACK);
  displayTela.drawPixel(106, 30, SSD1306_BLACK);
  displayTela.drawPixel(108, 30, SSD1306_BLACK);
  displayTela.drawPixel(110, 30, SSD1306_BLACK);
  displayTela.drawPixel(113, 30, SSD1306_BLACK);
  displayTela.drawPixel(116, 30, SSD1306_BLACK);
  displayTela.drawPixel(119, 30, SSD1306_BLACK);
  displayTela.drawPixel(122, 30, SSD1306_BLACK);
  displayTela.drawPixel(126, 30, SSD1306_BLACK);
  displayTela.drawPixel(102, 39, SSD1306_BLACK);
  displayTela.drawPixel(103, 40, SSD1306_BLACK);
  displayTela.drawPixel(104, 41, SSD1306_BLACK);
  displayTela.drawPixel(108, 43, SSD1306_BLACK);
  displayTela.drawPixel(111, 45, SSD1306_BLACK);
  displayTela.drawPixel(114, 47, SSD1306_BLACK);
  displayTela.drawPixel(122, 52, SSD1306_BLACK);
  displayTela.drawPixel(126, 54, SSD1306_BLACK);
  displayTela.drawPixel(98, 49, SSD1306_BLACK);
  displayTela.drawPixel(99, 50, SSD1306_BLACK);
  displayTela.drawPixel(99, 51, SSD1306_BLACK);
  displayTela.drawPixel(100, 52, SSD1306_BLACK);
  displayTela.drawPixel(103, 56, SSD1306_BLACK);
  displayTela.drawPixel(106, 59, SSD1306_BLACK);
  displayTela.drawPixel(108, 61, SSD1306_BLACK);
  displayTela.drawPixel(92, 60, SSD1306_BLACK);
  displayTela.drawPixel(93, 61, SSD1306_BLACK);
  displayTela.drawPixel(94, 63, SSD1306_BLACK);
  displayTela.drawPixel(83, 60, SSD1306_BLACK);
  displayTela.drawPixel(83, 61, SSD1306_BLACK);
  displayTela.drawPixel(83, 62, SSD1306_BLACK);
  displayTela.drawPixel(84, 63, SSD1306_BLACK);
  displayTela.drawPixel(77, 61, SSD1306_BLACK);
  displayTela.drawPixel(77, 61, SSD1306_BLACK);
  displayTela.drawPixel(77, 62, SSD1306_BLACK);
  displayTela.drawPixel(78, 62, SSD1306_BLACK);
  displayTela.drawPixel(78, 63, SSD1306_BLACK);
  displayTela.drawPixel(70, 61, SSD1306_BLACK);
  displayTela.drawPixel(70, 62, SSD1306_BLACK);
  displayTela.drawPixel(70, 63, SSD1306_BLACK);
  displayTela.drawPixel(61, 61, SSD1306_BLACK);
  displayTela.drawPixel(61, 62, SSD1306_BLACK);
  displayTela.drawPixel(61, 63, SSD1306_BLACK);
  displayTela.drawPixel(55, 60, SSD1306_BLACK);
  displayTela.drawPixel(55, 61, SSD1306_BLACK);
  displayTela.drawPixel(54, 61, SSD1306_BLACK);
  displayTela.drawPixel(54, 62, SSD1306_BLACK);
  displayTela.drawPixel(46, 61, SSD1306_BLACK);
  displayTela.drawPixel(45, 62, SSD1306_BLACK);
  displayTela.drawPixel(45, 63, SSD1306_BLACK);
  displayTela.drawPixel(39, 59, SSD1306_BLACK);
  displayTela.drawPixel(38, 59, SSD1306_BLACK);
  displayTela.drawPixel(37, 60, SSD1306_BLACK);
  displayTela.drawPixel(32, 61, SSD1306_BLACK);
  displayTela.drawPixel(28, 62, SSD1306_BLACK);
  displayTela.drawPixel(23, 63, SSD1306_BLACK);
  displayTela.drawPixel(36, 51, SSD1306_BLACK);
  displayTela.drawPixel(35, 51, SSD1306_BLACK);
  displayTela.drawPixel(34, 51, SSD1306_BLACK);
  displayTela.drawPixel(33, 52, SSD1306_BLACK);
  displayTela.drawPixel(28, 54, SSD1306_BLACK);
  displayTela.drawPixel(24, 56, SSD1306_BLACK);
  displayTela.drawPixel(19, 58, SSD1306_BLACK);
  displayTela.drawPixel(32, 42, SSD1306_BLACK);
  displayTela.drawPixel(30, 42, SSD1306_BLACK);
  displayTela.drawPixel(29, 43, SSD1306_BLACK);
  displayTela.drawPixel(26, 44, SSD1306_BLACK);
  displayTela.drawPixel(17, 48, SSD1306_BLACK);
  displayTela.drawPixel(3, 54, SSD1306_BLACK);
  displayTela.drawPixel(33, 36, SSD1306_BLACK);
  displayTela.drawPixel(32, 36, SSD1306_BLACK);
  displayTela.drawPixel(30, 35, SSD1306_BLACK);
  displayTela.drawPixel(28, 35, SSD1306_BLACK);
  displayTela.drawPixel(24, 34, SSD1306_BLACK);
  displayTela.drawPixel(17, 33, SSD1306_BLACK);
  displayTela.drawPixel(9, 33, SSD1306_BLACK);
  displayTela.drawPixel(28, 24, SSD1306_BLACK);
  displayTela.drawPixel(27, 23, SSD1306_BLACK);
  displayTela.drawPixel(26, 23, SSD1306_BLACK);
  displayTela.drawPixel(23, 22, SSD1306_BLACK);
  displayTela.drawPixel(13, 20, SSD1306_BLACK);
  displayTela.drawPixel(5, 18, SSD1306_BLACK);
  displayTela.drawPixel(33, 18, SSD1306_BLACK);
  displayTela.drawPixel(32, 17, SSD1306_BLACK);
  displayTela.drawPixel(29, 15, SSD1306_BLACK);
  displayTela.drawPixel(25, 14, SSD1306_BLACK);
  displayTela.drawPixel(21, 11, SSD1306_BLACK);
  displayTela.drawPixel(15, 8, SSD1306_BLACK);
  displayTela.drawPixel(8, 5, SSD1306_BLACK);
  displayTela.drawPixel(40, 11, SSD1306_BLACK);
  displayTela.drawPixel(40, 10, SSD1306_BLACK);
  displayTela.drawPixel(38, 9, SSD1306_BLACK);
  displayTela.drawPixel(37, 8, SSD1306_BLACK);
  displayTela.drawPixel(36, 7, SSD1306_BLACK);
  displayTela.drawPixel(35, 5, SSD1306_BLACK);
  displayTela.drawPixel(32, 2, SSD1306_BLACK);
  displayTela.drawPixel(31, 9, SSD1306_BLACK);
  displayTela.drawPixel(30, 8, SSD1306_BLACK);
  displayTela.drawPixel(29, 7, SSD1306_BLACK);
  displayTela.drawPixel(28, 6, SSD1306_BLACK);
  displayTela.drawPixel(24, 2, SSD1306_BLACK);
  displayTela.drawPixel(21, 0, SSD1306_BLACK);
  displayTela.drawPixel(82, 4, SSD1306_BLACK);
  displayTela.drawPixel(82, 3, SSD1306_BLACK);
  displayTela.drawPixel(83, 1, SSD1306_BLACK);
  displayTela.drawPixel(72, 2, SSD1306_BLACK);
  displayTela.drawPixel(72, 0, SSD1306_BLACK);
  displayTela.drawPixel(52, 15, SSD1306_BLACK);
  displayTela.drawPixel(51, 15, SSD1306_BLACK);
  displayTela.drawPixel(51, 16, SSD1306_BLACK);
  displayTela.drawPixel(50, 16, SSD1306_BLACK);
  displayTela.drawPixel(50, 17, SSD1306_BLACK);
  displayTela.drawPixel(49, 17, SSD1306_BLACK);
  displayTela.drawPixel(49, 18, SSD1306_BLACK);


}

// Ícone 4: TERMÔMETRO (quente) - espaço reservado
// Já definido acima como desenharTermometro() - nenhuma ação necessária
void desenharTermometro() { 
  

// Draw shapes

  //id: 0 fillCircle 168 
  displayTela.fillCircle(63, 47, 7, SSD1306_WHITE);
  //id: 1 line 169 
  displayTela.drawLine(66, 40, 66, 6, SSD1306_WHITE);
  //id: 2 line 170 
  displayTela.drawLine(60, 40, 60, 6, SSD1306_WHITE);
  //id: 3 line 171 
  displayTela.drawLine(61, 5, 61, 5, SSD1306_WHITE);
  //id: 4 line 172 
  displayTela.drawLine(62, 4, 64, 4, SSD1306_WHITE);
  //id: 5 line 173 
  displayTela.drawLine(65, 5, 65, 5, SSD1306_WHITE);
  //id: 6 pixel 174 
  displayTela.drawPixel(65, 38, SSD1306_WHITE);
  displayTela.drawPixel(65, 37, SSD1306_WHITE);
  displayTela.drawPixel(65, 36, SSD1306_WHITE);
  displayTela.drawPixel(65, 35, SSD1306_WHITE);
  displayTela.drawPixel(65, 34, SSD1306_WHITE);
  displayTela.drawPixel(65, 33, SSD1306_WHITE);
  displayTela.drawPixel(65, 32, SSD1306_WHITE);
  displayTela.drawPixel(66, 31, SSD1306_WHITE);
  displayTela.drawPixel(66, 30, SSD1306_WHITE);
  displayTela.drawPixel(66, 29, SSD1306_WHITE);
  displayTela.drawPixel(65, 30, SSD1306_WHITE);
  displayTela.drawPixel(65, 31, SSD1306_WHITE);
  displayTela.drawPixel(64, 30, SSD1306_WHITE);
  displayTela.drawPixel(63, 30, SSD1306_WHITE);
  displayTela.drawPixel(62, 30, SSD1306_WHITE);
  displayTela.drawPixel(61, 30, SSD1306_WHITE);
  displayTela.drawPixel(61, 31, SSD1306_WHITE);
  displayTela.drawPixel(61, 32, SSD1306_WHITE);
  displayTela.drawPixel(61, 33, SSD1306_WHITE);
  displayTela.drawPixel(61, 34, SSD1306_WHITE);
  displayTela.drawPixel(61, 35, SSD1306_WHITE);
  displayTela.drawPixel(61, 36, SSD1306_WHITE);
  displayTela.drawPixel(61, 37, SSD1306_WHITE);
  displayTela.drawPixel(61, 38, SSD1306_WHITE);
  displayTela.drawPixel(61, 39, SSD1306_WHITE);
  displayTela.drawPixel(62, 39, SSD1306_WHITE);
  displayTela.drawPixel(62, 38, SSD1306_WHITE);
  displayTela.drawPixel(62, 37, SSD1306_WHITE);
  displayTela.drawPixel(62, 36, SSD1306_WHITE);
  displayTela.drawPixel(63, 36, SSD1306_WHITE);
  displayTela.drawPixel(63, 35, SSD1306_WHITE);
  displayTela.drawPixel(63, 34, SSD1306_WHITE);
  displayTela.drawPixel(63, 33, SSD1306_WHITE);
  displayTela.drawPixel(63, 32, SSD1306_WHITE);
  displayTela.drawPixel(62, 32, SSD1306_WHITE);
  displayTela.drawPixel(62, 33, SSD1306_WHITE);
  displayTela.drawPixel(62, 31, SSD1306_WHITE);
  displayTela.drawPixel(63, 31, SSD1306_WHITE);
  displayTela.drawPixel(64, 31, SSD1306_WHITE);
  displayTela.drawPixel(64, 32, SSD1306_WHITE);
  displayTela.drawPixel(64, 33, SSD1306_WHITE);
  displayTela.drawPixel(64, 34, SSD1306_WHITE);
  displayTela.drawPixel(64, 35, SSD1306_WHITE);
  displayTela.drawPixel(64, 36, SSD1306_WHITE);
  displayTela.drawPixel(64, 37, SSD1306_WHITE);
  displayTela.drawPixel(64, 38, SSD1306_WHITE);
  displayTela.drawPixel(63, 37, SSD1306_WHITE);
  displayTela.drawPixel(62, 35, SSD1306_WHITE);
  displayTela.drawPixel(62, 34, SSD1306_WHITE);
  displayTela.drawPixel(63, 38, SSD1306_WHITE);
  displayTela.drawPixel(63, 39, SSD1306_WHITE);
  displayTela.drawPixel(63, 40, SSD1306_WHITE);
  displayTela.drawPixel(64, 39, SSD1306_WHITE);
  displayTela.drawPixel(65, 39, SSD1306_WHITE);
  displayTela.drawPixel(65, 40, SSD1306_WHITE);
  //id: 7 pixel 175 
  displayTela.drawPixel(65, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  //id: 8 pixel 176 
  displayTela.drawPixel(62, 3, SSD1306_WHITE);
  displayTela.drawPixel(63, 3, SSD1306_WHITE);
  displayTela.drawPixel(64, 3, SSD1306_WHITE);
  //id: 9 pixel 177 
  displayTela.drawPixel(61, 4, SSD1306_WHITE);
  //id: 10 pixel 178 
  displayTela.drawPixel(60, 5, SSD1306_WHITE);
  //id: 11 pixel 179 
  displayTela.drawPixel(59, 6, SSD1306_WHITE);
  displayTela.drawPixel(59, 7, SSD1306_WHITE);
  displayTela.drawPixel(59, 8, SSD1306_WHITE);
  displayTela.drawPixel(59, 9, SSD1306_WHITE);
  displayTela.drawPixel(59, 10, SSD1306_WHITE);
  displayTela.drawPixel(59, 11, SSD1306_WHITE);
  displayTela.drawPixel(59, 12, SSD1306_WHITE);
  displayTela.drawPixel(59, 13, SSD1306_WHITE);
  displayTela.drawPixel(59, 14, SSD1306_WHITE);
  displayTela.drawPixel(59, 15, SSD1306_WHITE);
  displayTela.drawPixel(59, 16, SSD1306_WHITE);
  displayTela.drawPixel(59, 17, SSD1306_WHITE);
  displayTela.drawPixel(59, 18, SSD1306_WHITE);
  displayTela.drawPixel(59, 19, SSD1306_WHITE);
  displayTela.drawPixel(59, 20, SSD1306_WHITE);
  displayTela.drawPixel(59, 21, SSD1306_WHITE);
  displayTela.drawPixel(59, 22, SSD1306_WHITE);
  displayTela.drawPixel(59, 23, SSD1306_WHITE);
  displayTela.drawPixel(59, 24, SSD1306_WHITE);
  displayTela.drawPixel(59, 25, SSD1306_WHITE);
  displayTela.drawPixel(59, 26, SSD1306_WHITE);
  displayTela.drawPixel(59, 27, SSD1306_WHITE);
  displayTela.drawPixel(59, 28, SSD1306_WHITE);
  displayTela.drawPixel(59, 29, SSD1306_WHITE);
  displayTela.drawPixel(59, 30, SSD1306_WHITE);
  displayTela.drawPixel(59, 31, SSD1306_WHITE);
  displayTela.drawPixel(59, 32, SSD1306_WHITE);
  displayTela.drawPixel(59, 33, SSD1306_WHITE);
  displayTela.drawPixel(59, 34, SSD1306_WHITE);
  displayTela.drawPixel(59, 35, SSD1306_WHITE);
  displayTela.drawPixel(59, 36, SSD1306_WHITE);
  displayTela.drawPixel(59, 37, SSD1306_WHITE);
  displayTela.drawPixel(59, 38, SSD1306_WHITE);
  displayTela.drawPixel(59, 39, SSD1306_WHITE);
  displayTela.drawPixel(59, 40, SSD1306_WHITE);
  displayTela.drawPixel(58, 40, SSD1306_WHITE);
  displayTela.drawPixel(58, 41, SSD1306_WHITE);
  //id: 12 pixel 180 
  displayTela.drawPixel(67, 40, SSD1306_WHITE);
  displayTela.drawPixel(67, 39, SSD1306_WHITE);
  displayTela.drawPixel(67, 38, SSD1306_WHITE);
  displayTela.drawPixel(67, 37, SSD1306_WHITE);
  displayTela.drawPixel(67, 36, SSD1306_WHITE);
  displayTela.drawPixel(67, 35, SSD1306_WHITE);
  displayTela.drawPixel(67, 34, SSD1306_WHITE);
  displayTela.drawPixel(67, 33, SSD1306_WHITE);
  displayTela.drawPixel(67, 32, SSD1306_WHITE);
  displayTela.drawPixel(67, 31, SSD1306_WHITE);
  displayTela.drawPixel(67, 30, SSD1306_WHITE);
  displayTela.drawPixel(67, 29, SSD1306_WHITE);
  displayTela.drawPixel(67, 28, SSD1306_WHITE);
  displayTela.drawPixel(67, 27, SSD1306_WHITE);
  displayTela.drawPixel(67, 26, SSD1306_WHITE);
  displayTela.drawPixel(67, 25, SSD1306_WHITE);
  displayTela.drawPixel(67, 24, SSD1306_WHITE);
  displayTela.drawPixel(67, 23, SSD1306_WHITE);
  displayTela.drawPixel(67, 22, SSD1306_WHITE);
  displayTela.drawPixel(67, 21, SSD1306_WHITE);
  displayTela.drawPixel(67, 20, SSD1306_WHITE);
  displayTela.drawPixel(67, 19, SSD1306_WHITE);
  displayTela.drawPixel(67, 18, SSD1306_WHITE);
  displayTela.drawPixel(67, 17, SSD1306_WHITE);
  displayTela.drawPixel(67, 16, SSD1306_WHITE);
  displayTela.drawPixel(67, 15, SSD1306_WHITE);
  displayTela.drawPixel(67, 14, SSD1306_WHITE);
  displayTela.drawPixel(67, 13, SSD1306_WHITE);
  displayTela.drawPixel(67, 12, SSD1306_WHITE);
  displayTela.drawPixel(67, 11, SSD1306_WHITE);
  displayTela.drawPixel(67, 10, SSD1306_WHITE);
  displayTela.drawPixel(67, 9, SSD1306_WHITE);
  displayTela.drawPixel(67, 8, SSD1306_WHITE);
  displayTela.drawPixel(67, 7, SSD1306_WHITE);
  displayTela.drawPixel(67, 6, SSD1306_WHITE);
  //id: 13 pixel 181 
  displayTela.drawPixel(66, 5, SSD1306_WHITE);
  //id: 14 pixel 182 
  displayTela.drawPixel(65, 4, SSD1306_WHITE);
  //id: 15 pixel 183 
  displayTela.drawPixel(65, 29, SSD1306_WHITE);
  displayTela.drawPixel(65, 32, SSD1306_WHITE);
  displayTela.drawPixel(64, 34, SSD1306_WHITE);
  displayTela.drawPixel(64, 36, SSD1306_WHITE);
  displayTela.drawPixel(64, 38, SSD1306_WHITE);
  displayTela.drawPixel(64, 40, SSD1306_WHITE);
  displayTela.drawPixel(64, 42, SSD1306_WHITE);
  displayTela.drawPixel(64, 43, SSD1306_WHITE);
  displayTela.drawPixel(64, 44, SSD1306_WHITE);
  displayTela.drawPixel(64, 45, SSD1306_WHITE);
  displayTela.drawPixel(64, 46, SSD1306_WHITE);
  //id: 16 pixel 184 
  displayTela.drawPixel(65, 29, SSD1306_BLACK);
  //id: 17 pixel 185 
  displayTela.drawPixel(68, 40, SSD1306_WHITE);
  displayTela.drawPixel(68, 41, SSD1306_WHITE);
  //id: 18 pixel 186 
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  displayTela.drawPixel(63, 8, SSD1306_WHITE);
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  displayTela.drawPixel(65, 8, SSD1306_WHITE);
  //id: 19 pixel 187 
  displayTela.drawPixel(63, 12, SSD1306_WHITE);
  displayTela.drawPixel(64, 12, SSD1306_WHITE);
  displayTela.drawPixel(65, 12, SSD1306_WHITE);
  displayTela.drawPixel(65, 13, SSD1306_WHITE);
  displayTela.drawPixel(64, 13, SSD1306_WHITE);
  displayTela.drawPixel(63, 13, SSD1306_WHITE);
  //id: 20 pixel 188 
  displayTela.drawPixel(63, 17, SSD1306_WHITE);
  displayTela.drawPixel(64, 17, SSD1306_WHITE);
  displayTela.drawPixel(65, 17, SSD1306_WHITE);
  displayTela.drawPixel(65, 18, SSD1306_WHITE);
  displayTela.drawPixel(64, 18, SSD1306_WHITE);
  displayTela.drawPixel(63, 18, SSD1306_WHITE);
  //id: 21 pixel 189 
  displayTela.drawPixel(63, 22, SSD1306_WHITE);
  displayTela.drawPixel(64, 22, SSD1306_WHITE);
  displayTela.drawPixel(65, 22, SSD1306_WHITE);
  displayTela.drawPixel(65, 23, SSD1306_WHITE);
  displayTela.drawPixel(64, 23, SSD1306_WHITE);
  displayTela.drawPixel(63, 23, SSD1306_WHITE);
  //id: 22 pixel 190 
  displayTela.drawPixel(63, 23, SSD1306_BLACK);
  displayTela.drawPixel(63, 22, SSD1306_BLACK);
  displayTela.drawPixel(63, 21, SSD1306_BLACK);
  displayTela.drawPixel(63, 20, SSD1306_BLACK);
  displayTela.drawPixel(64, 19, SSD1306_BLACK);
  displayTela.drawPixel(64, 18, SSD1306_BLACK);
  displayTela.drawPixel(64, 17, SSD1306_BLACK);
  displayTela.drawPixel(64, 16, SSD1306_BLACK);
  displayTela.drawPixel(65, 19, SSD1306_BLACK);
  displayTela.drawPixel(65, 20, SSD1306_BLACK);
  displayTela.drawPixel(65, 21, SSD1306_BLACK);
  displayTela.drawPixel(65, 22, SSD1306_BLACK);
  displayTela.drawPixel(65, 23, SSD1306_BLACK);
  displayTela.drawPixel(64, 23, SSD1306_BLACK);
  displayTela.drawPixel(64, 22, SSD1306_BLACK);
  displayTela.drawPixel(64, 21, SSD1306_BLACK);
  displayTela.drawPixel(64, 20, SSD1306_BLACK);
  displayTela.drawPixel(65, 18, SSD1306_BLACK);
  displayTela.drawPixel(65, 17, SSD1306_BLACK);
  displayTela.drawPixel(63, 17, SSD1306_BLACK);
  displayTela.drawPixel(63, 16, SSD1306_BLACK);
  displayTela.drawPixel(63, 15, SSD1306_BLACK);
  displayTela.drawPixel(64, 15, SSD1306_BLACK);
  displayTela.drawPixel(64, 14, SSD1306_BLACK);
  displayTela.drawPixel(64, 13, SSD1306_BLACK);
  displayTela.drawPixel(64, 12, SSD1306_BLACK);
  displayTela.drawPixel(64, 11, SSD1306_BLACK);
  displayTela.drawPixel(65, 12, SSD1306_BLACK);
  displayTela.drawPixel(63, 12, SSD1306_BLACK);
  displayTela.drawPixel(63, 13, SSD1306_BLACK);
  displayTela.drawPixel(63, 14, SSD1306_BLACK);
  displayTela.drawPixel(65, 13, SSD1306_BLACK);
  displayTela.drawPixel(63, 18, SSD1306_BLACK);
  displayTela.drawPixel(63, 19, SSD1306_BLACK);
  //id: 23 pixel 191 
  displayTela.drawPixel(63, 12, SSD1306_WHITE);
  displayTela.drawPixel(64, 12, SSD1306_WHITE);
  //id: 24 pixel 192 
  displayTela.drawPixel(65, 13, SSD1306_WHITE);
  displayTela.drawPixel(64, 13, SSD1306_WHITE);
  displayTela.drawPixel(63, 13, SSD1306_WHITE);
  displayTela.drawPixel(63, 14, SSD1306_WHITE);
  displayTela.drawPixel(64, 14, SSD1306_WHITE);
  displayTela.drawPixel(65, 14, SSD1306_WHITE);
  //id: 25 pixel 193 
  displayTela.drawPixel(63, 19, SSD1306_WHITE);
  displayTela.drawPixel(64, 19, SSD1306_WHITE);
  displayTela.drawPixel(65, 19, SSD1306_WHITE);
  displayTela.drawPixel(65, 20, SSD1306_WHITE);
  displayTela.drawPixel(64, 20, SSD1306_WHITE);
  displayTela.drawPixel(63, 20, SSD1306_WHITE);
  displayTela.drawPixel(62, 20, SSD1306_WHITE);
  //id: 26 pixel 194 
  displayTela.drawPixel(63, 25, SSD1306_WHITE);
  displayTela.drawPixel(64, 24, SSD1306_WHITE);
  displayTela.drawPixel(65, 24, SSD1306_WHITE);
  displayTela.drawPixel(65, 25, SSD1306_WHITE);
  displayTela.drawPixel(64, 25, SSD1306_WHITE);
  //id: 27 pixel 195 
  displayTela.drawPixel(63, 26, SSD1306_WHITE);
  displayTela.drawPixel(64, 26, SSD1306_WHITE);
  displayTela.drawPixel(65, 26, SSD1306_WHITE);
  //id: 28 pixel 196 
  displayTela.drawPixel(62, 20, SSD1306_BLACK);
  //id: 29 pixel 197 
  displayTela.drawPixel(64, 24, SSD1306_BLACK);
  displayTela.drawPixel(65, 24, SSD1306_BLACK);
  //id: 30 pixel 198 
  displayTela.drawPixel(64, 12, SSD1306_BLACK);
  displayTela.drawPixel(63, 12, SSD1306_BLACK);
  //id: 31 pixel 199 
  displayTela.drawPixel(65, 28, SSD1306_WHITE);
  displayTela.drawPixel(64, 28, SSD1306_WHITE);
  displayTela.drawPixel(63, 28, SSD1306_WHITE);
  displayTela.drawPixel(62, 28, SSD1306_WHITE);
  displayTela.drawPixel(62, 29, SSD1306_WHITE);
  displayTela.drawPixel(61, 28, SSD1306_WHITE);
  displayTela.drawPixel(61, 29, SSD1306_WHITE);
  displayTela.drawPixel(61, 30, SSD1306_WHITE);
  displayTela.drawPixel(62, 30, SSD1306_WHITE);
  displayTela.drawPixel(63, 30, SSD1306_WHITE);
  displayTela.drawPixel(63, 29, SSD1306_WHITE);
  displayTela.drawPixel(64, 29, SSD1306_WHITE);
  displayTela.drawPixel(65, 29, SSD1306_WHITE);
  //id: 32 pixel 200 
  displayTela.drawPixel(65, 27, SSD1306_WHITE);
  displayTela.drawPixel(64, 27, SSD1306_WHITE);
  displayTela.drawPixel(63, 27, SSD1306_WHITE);
  displayTela.drawPixel(62, 27, SSD1306_WHITE);
  displayTela.drawPixel(61, 27, SSD1306_WHITE);
  //id: 33 pixel 201 
  displayTela.drawPixel(63, 31, SSD1306_BLACK);
  displayTela.drawPixel(64, 31, SSD1306_BLACK);
  displayTela.drawPixel(65, 31, SSD1306_BLACK);
  displayTela.drawPixel(65, 32, SSD1306_BLACK);
  displayTela.drawPixel(64, 32, SSD1306_BLACK);
  displayTela.drawPixel(63, 32, SSD1306_BLACK);
  //id: 34 pixel 202 
  displayTela.drawPixel(63, 37, SSD1306_BLACK);
  displayTela.drawPixel(64, 37, SSD1306_BLACK);
  displayTela.drawPixel(65, 37, SSD1306_BLACK);
  displayTela.drawPixel(65, 38, SSD1306_BLACK);
  displayTela.drawPixel(64, 38, SSD1306_BLACK);
  displayTela.drawPixel(63, 38, SSD1306_BLACK);
  //id: 35 pixel 203 
  displayTela.drawPixel(65, 27, SSD1306_BLACK);
  displayTela.drawPixel(64, 27, SSD1306_BLACK);
  displayTela.drawPixel(63, 27, SSD1306_BLACK);
  displayTela.drawPixel(62, 27, SSD1306_BLACK);
  displayTela.drawPixel(61, 27, SSD1306_BLACK);



}
// Ícone 5: FLOCO DE NEVE (frio/neve) - espaço reservado
// Já definido acima como desenharNeve() - nenhuma ação necessária
void desenharNeve() { 
  

// Draw shapes

  //id: 0 fillRect 207 
  displayTela.fillRect(64, 3, 1, 3, SSD1306_WHITE);
  //id: 1 fillRect 208 
  displayTela.fillRect(63, 3, 3, 3, SSD1306_WHITE);
  //id: 2 fillRect 209 
  displayTela.fillRect(60, 6, 9, 6, SSD1306_WHITE);
  //id: 3 fillRect 210 
  displayTela.fillRect(63, 11, 3, 4, SSD1306_WHITE);
  //id: 4 fillRect 211 
  displayTela.fillRect(60, 15, 9, 3, SSD1306_WHITE);
  //id: 5 fillRect 212 
  displayTela.fillRect(63, 18, 3, 3, SSD1306_WHITE);
  //id: 6 fillRect 213 
  displayTela.fillRect(60, 21, 9, 3, SSD1306_WHITE);
  //id: 7 fillRect 214 
  displayTela.fillRect(57, 24, 15, 3, SSD1306_WHITE);
  //id: 8 fillRect 215 
  displayTela.fillRect(54, 27, 21, 3, SSD1306_WHITE);
  //id: 9 fillRect 216 
  displayTela.fillRect(51, 15, 3, 12, SSD1306_WHITE);
  //id: 10 fillRect 217 
  displayTela.fillRect(54, 30, 21, 6, SSD1306_WHITE);
  //id: 11 fillRect 218 
  displayTela.fillRect(57, 36, 15, 3, SSD1306_WHITE);
  //id: 12 fillRect 219 
  displayTela.fillRect(60, 39, 9, 3, SSD1306_WHITE);
  //id: 13 fillRect 220 
  displayTela.fillRect(63, 42, 3, 3, SSD1306_WHITE);
  //id: 14 fillRect 221 
  displayTela.fillRect(75, 30, 3, 3, SSD1306_WHITE);
  //id: 15 fillRect 222 
  displayTela.fillRect(78, 27, 3, 8, SSD1306_WHITE);
  //id: 16 fillRect 223 
  displayTela.fillRect(78, 35, 3, 1, SSD1306_WHITE);
  //id: 17 fillRect 224 
  displayTela.fillRect(81, 30, 3, 3, SSD1306_WHITE);
  //id: 18 fillRect 225 
  displayTela.fillRect(84, 27, 6, 9, SSD1306_WHITE);
  //id: 19 fillRect 226 
  displayTela.fillRect(90, 30, 3, 3, SSD1306_WHITE);
  //id: 20 fillRect 227 
  displayTela.fillRect(37, 30, 17, 3, SSD1306_WHITE);
  //id: 21 fillRect 228 
  displayTela.fillRect(51, 15, 3, 12, SSD1306_BLACK);
  //id: 22 fillRect 229 
  displayTela.fillRect(54, 15, 3, 9, SSD1306_WHITE);
  //id: 23 fillRect 230 
  displayTela.fillRect(48, 27, 3, 9, SSD1306_WHITE);
  //id: 24 fillRect 231 
  displayTela.fillRect(48, 21, 6, 3, SSD1306_WHITE);
  //id: 25 fillRect 232 
  displayTela.fillRect(51, 18, 3, 3, SSD1306_WHITE);
  //id: 26 fillRect 233 
  displayTela.fillRect(45, 12, 6, 6, SSD1306_WHITE);
  //id: 27 fillRect 234 
  displayTela.fillRect(42, 9, 3, 3, SSD1306_WHITE);
  //id: 28 fillRect 235 
  displayTela.fillRect(72, 15, 3, 9, SSD1306_WHITE);
  //id: 29 fillRect 236 
  displayTela.fillRect(75, 21, 6, 3, SSD1306_WHITE);
  //id: 30 fillRect 237 
  displayTela.fillRect(75, 18, 3, 3, SSD1306_WHITE);
  //id: 31 fillRect 238 
  displayTela.fillRect(78, 12, 6, 6, SSD1306_WHITE);
  //id: 32 fillRect 239 
  displayTela.fillRect(84, 9, 3, 3, SSD1306_WHITE);
  //id: 33 fillRect 240 
  displayTela.fillRect(60, 45, 9, 3, SSD1306_WHITE);
  //id: 34 fillRect 241 
  displayTela.fillRect(63, 48, 3, 3, SSD1306_WHITE);
  //id: 35 fillRect 242 
  displayTela.fillRect(60, 51, 9, 6, SSD1306_WHITE);
  //id: 36 fillRect 243 
  displayTela.fillRect(63, 57, 3, 3, SSD1306_WHITE);
  //id: 37 fillRect 244 
  displayTela.fillRect(39, 27, 6, 9, SSD1306_WHITE);
  //id: 38 fillRect 245 
  displayTela.fillRect(36, 30, 1, 3, SSD1306_WHITE);
  //id: 39 fillRect 246 
  displayTela.fillRect(72, 39, 9, 3, SSD1306_WHITE);
  //id: 40 fillRect 247 
  displayTela.fillRect(72, 42, 3, 6, SSD1306_WHITE);
  //id: 41 fillRect 248 
  displayTela.fillRect(75, 42, 3, 3, SSD1306_WHITE);
  //id: 42 fillRect 249 
  displayTela.fillRect(78, 45, 6, 6, SSD1306_WHITE);
  //id: 43 fillRect 250 
  displayTela.fillRect(84, 51, 3, 3, SSD1306_WHITE);
  //id: 44 fillRect 251 
  displayTela.fillRect(48, 39, 9, 3, SSD1306_WHITE);
  //id: 45 fillRect 252 
  displayTela.fillRect(54, 42, 3, 6, SSD1306_WHITE);
  //id: 46 fillRect 253 
  displayTela.fillRect(51, 42, 3, 3, SSD1306_WHITE);
  //id: 47 fillRect 254 
  displayTela.fillRect(48, 45, 3, 3, SSD1306_WHITE);
  //id: 48 fillRect 255 
  displayTela.fillRect(45, 48, 3, 3, SSD1306_WHITE);
  //id: 49 fillRect 256 
  displayTela.fillRect(45, 45, 6, 6, SSD1306_WHITE);
  //id: 50 fillRect 257 
  displayTela.fillRect(42, 51, 3, 3, SSD1306_WHITE);
  //id: 51 fillRect 259 
  displayTela.fillRect(94, 11, 3, 3, SSD1306_WHITE);
  //id: 52 fillRect 260 
  displayTela.fillRect(97, 8, 3, 9, SSD1306_WHITE);
  //id: 53 fillRect 261 
  displayTela.fillRect(100, 11, 3, 3, SSD1306_WHITE);
  //id: 54 fillRect 262 
  displayTela.fillRect(96, 47, 3, 3, SSD1306_WHITE);
  //id: 55 fillRect 263 
  displayTela.fillRect(99, 44, 3, 3, SSD1306_WHITE);
  //id: 56 fillRect 264 
  displayTela.fillRect(99, 47, 3, 6, SSD1306_WHITE);
  //id: 57 fillRect 265 
  displayTela.fillRect(102, 47, 3, 3, SSD1306_WHITE);
  //id: 58 fillRect 266 
  displayTela.fillRect(26, 42, 3, 3, SSD1306_WHITE);
  //id: 59 fillRect 267 
  displayTela.fillRect(23, 39, 3, 9, SSD1306_WHITE);
  //id: 60 fillRect 268 
  displayTela.fillRect(20, 42, 3, 3, SSD1306_WHITE);
  //id: 61 fillRect 269 
  displayTela.fillRect(31, 14, 3, 3, SSD1306_WHITE);
  //id: 62 fillRect 270 
  displayTela.fillRect(28, 11, 3, 9, SSD1306_WHITE);
  //id: 63 fillRect 271 
  displayTela.fillRect(25, 14, 3, 3, SSD1306_WHITE);
  //id: 64 fillRect 272 
  displayTela.fillRect(6, 2, 3, 3, SSD1306_WHITE);
  //id: 65 fillRect 273 
  displayTela.fillRect(12, 55, 3, 3, SSD1306_WHITE);
  //id: 66 fillRect 274 
  displayTela.fillRect(16, 22, 3, 3, SSD1306_WHITE);
  //id: 67 fillRect 275 
  displayTela.fillRect(7, 33, 3, 3, SSD1306_WHITE);
  //id: 68 fillRect 276 
  displayTela.fillRect(26, 29, 3, 3, SSD1306_WHITE);
  //id: 69 fillRect 277 
  displayTela.fillRect(118, 4, 3, 3, SSD1306_WHITE);
  //id: 70 fillRect 278 
  displayTela.fillRect(122, 59, 3, 3, SSD1306_WHITE);
  //id: 71 fillRect 279 
  displayTela.fillRect(117, 30, 3, 3, SSD1306_WHITE);
  //id: 72 fillRect 280 
  displayTela.fillRect(114, 16, 3, 3, SSD1306_WHITE);
  //id: 73 fillRect 281 
  displayTela.fillRect(101, 28, 3, 3, SSD1306_WHITE);
  //id: 74 fillRect 282 
  displayTela.fillRect(113, 48, 3, 3, SSD1306_WHITE);
  //id: 75 fillRect 283 
  displayTela.fillRect(121, 36, 3, 3, SSD1306_WHITE);
  //id: 76 fillRect 284 
  displayTela.fillRect(118, 39, 9, 3, SSD1306_WHITE);
  //id: 77 fillRect 285 
  displayTela.fillRect(121, 42, 3, 3, SSD1306_WHITE);
  //id: 78 fillRect 286 
  displayTela.fillRect(35, 53, 3, 3, SSD1306_WHITE);
  //id: 79 fillRect 287 
  displayTela.fillRect(32, 56, 9, 3, SSD1306_WHITE);
  //id: 80 fillRect 288 
  displayTela.fillRect(35, 59, 3, 3, SSD1306_WHITE);
  //id: 81 fillRect 289 
  displayTela.fillRect(21, 3, 3, 3, SSD1306_WHITE);
  //id: 82 fillRect 290 
  displayTela.fillRect(88, 62, 3, 2, SSD1306_WHITE);
  //id: 83 fillRect 291 
  displayTela.fillRect(9, 62, 3, 2, SSD1306_WHITE);
  //id: 84 fillRect 292 
  displayTela.fillRect(1, 18, 3, 3, SSD1306_WHITE);

  

}
// ============================================================================

void desenharSino() {


// Draw shapes

  //id: 0 pixel 0 
  displayTela.drawPixel(63, 4, SSD1306_WHITE);
  displayTela.drawPixel(63, 5, SSD1306_WHITE);
  displayTela.drawPixel(63, 6, SSD1306_WHITE);
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  displayTela.drawPixel(63, 8, SSD1306_WHITE);
  displayTela.drawPixel(63, 9, SSD1306_WHITE);
  displayTela.drawPixel(63, 10, SSD1306_WHITE);
  displayTela.drawPixel(63, 11, SSD1306_WHITE);
  displayTela.drawPixel(63, 12, SSD1306_WHITE);
  displayTela.drawPixel(63, 13, SSD1306_WHITE);
  displayTela.drawPixel(63, 14, SSD1306_WHITE);
  displayTela.drawPixel(63, 15, SSD1306_WHITE);
  displayTela.drawPixel(63, 16, SSD1306_WHITE);
  displayTela.drawPixel(63, 17, SSD1306_WHITE);
  displayTela.drawPixel(63, 18, SSD1306_WHITE);
  displayTela.drawPixel(63, 19, SSD1306_WHITE);
  displayTela.drawPixel(63, 20, SSD1306_WHITE);
  displayTela.drawPixel(63, 21, SSD1306_WHITE);
  displayTela.drawPixel(63, 22, SSD1306_WHITE);
  displayTela.drawPixel(63, 23, SSD1306_WHITE);
  displayTela.drawPixel(63, 24, SSD1306_WHITE);
  displayTela.drawPixel(63, 25, SSD1306_WHITE);
  displayTela.drawPixel(63, 26, SSD1306_WHITE);
  displayTela.drawPixel(63, 27, SSD1306_WHITE);
  displayTela.drawPixel(63, 28, SSD1306_WHITE);
  displayTela.drawPixel(63, 29, SSD1306_WHITE);
  displayTela.drawPixel(63, 30, SSD1306_WHITE);
  displayTela.drawPixel(63, 31, SSD1306_WHITE);
  displayTela.drawPixel(63, 32, SSD1306_WHITE);
  displayTela.drawPixel(63, 33, SSD1306_WHITE);
  displayTela.drawPixel(63, 34, SSD1306_WHITE);
  displayTela.drawPixel(63, 35, SSD1306_WHITE);
  displayTela.drawPixel(63, 36, SSD1306_WHITE);
  displayTela.drawPixel(63, 37, SSD1306_WHITE);
  displayTela.drawPixel(63, 38, SSD1306_WHITE);
  displayTela.drawPixel(63, 39, SSD1306_WHITE);
  displayTela.drawPixel(63, 40, SSD1306_WHITE);
  displayTela.drawPixel(63, 41, SSD1306_WHITE);
  displayTela.drawPixel(63, 42, SSD1306_WHITE);
  displayTela.drawPixel(63, 43, SSD1306_WHITE);
  displayTela.drawPixel(63, 44, SSD1306_WHITE);
  displayTela.drawPixel(63, 45, SSD1306_WHITE);
  displayTela.drawPixel(63, 46, SSD1306_WHITE);
  displayTela.drawPixel(63, 47, SSD1306_WHITE);
  displayTela.drawPixel(63, 48, SSD1306_WHITE);
  displayTela.drawPixel(63, 49, SSD1306_WHITE);
  displayTela.drawPixel(63, 50, SSD1306_WHITE);
  displayTela.drawPixel(63, 51, SSD1306_WHITE);
  displayTela.drawPixel(63, 52, SSD1306_WHITE);
  displayTela.drawPixel(63, 53, SSD1306_WHITE);
  displayTela.drawPixel(63, 54, SSD1306_WHITE);
  displayTela.drawPixel(63, 55, SSD1306_WHITE);
  displayTela.drawPixel(63, 56, SSD1306_WHITE);
  displayTela.drawPixel(63, 57, SSD1306_WHITE);
  displayTela.drawPixel(63, 58, SSD1306_WHITE);
  //id: 1 pixel 1 
  displayTela.drawPixel(63, 58, SSD1306_WHITE);
  displayTela.drawPixel(64, 58, SSD1306_WHITE);
  displayTela.drawPixel(64, 57, SSD1306_WHITE);
  displayTela.drawPixel(64, 56, SSD1306_WHITE);
  displayTela.drawPixel(64, 55, SSD1306_WHITE);
  displayTela.drawPixel(64, 54, SSD1306_WHITE);
  displayTela.drawPixel(64, 53, SSD1306_WHITE);
  displayTela.drawPixel(64, 52, SSD1306_WHITE);
  displayTela.drawPixel(64, 51, SSD1306_WHITE);
  displayTela.drawPixel(64, 50, SSD1306_WHITE);
  displayTela.drawPixel(64, 49, SSD1306_WHITE);
  displayTela.drawPixel(64, 48, SSD1306_WHITE);
  displayTela.drawPixel(64, 47, SSD1306_WHITE);
  displayTela.drawPixel(64, 46, SSD1306_WHITE);
  displayTela.drawPixel(64, 45, SSD1306_WHITE);
  displayTela.drawPixel(64, 44, SSD1306_WHITE);
  displayTela.drawPixel(64, 43, SSD1306_WHITE);
  displayTela.drawPixel(64, 42, SSD1306_WHITE);
  displayTela.drawPixel(64, 41, SSD1306_WHITE);
  displayTela.drawPixel(64, 40, SSD1306_WHITE);
  displayTela.drawPixel(64, 39, SSD1306_WHITE);
  displayTela.drawPixel(64, 38, SSD1306_WHITE);
  displayTela.drawPixel(64, 37, SSD1306_WHITE);
  displayTela.drawPixel(64, 36, SSD1306_WHITE);
  displayTela.drawPixel(64, 35, SSD1306_WHITE);
  displayTela.drawPixel(64, 34, SSD1306_WHITE);
  displayTela.drawPixel(64, 33, SSD1306_WHITE);
  displayTela.drawPixel(64, 32, SSD1306_WHITE);
  displayTela.drawPixel(64, 31, SSD1306_WHITE);
  displayTela.drawPixel(64, 30, SSD1306_WHITE);
  displayTela.drawPixel(64, 29, SSD1306_WHITE);
  displayTela.drawPixel(64, 28, SSD1306_WHITE);
  displayTela.drawPixel(64, 27, SSD1306_WHITE);
  displayTela.drawPixel(64, 26, SSD1306_WHITE);
  displayTela.drawPixel(64, 25, SSD1306_WHITE);
  displayTela.drawPixel(64, 24, SSD1306_WHITE);
  displayTela.drawPixel(64, 23, SSD1306_WHITE);
  displayTela.drawPixel(64, 22, SSD1306_WHITE);
  displayTela.drawPixel(64, 21, SSD1306_WHITE);
  displayTela.drawPixel(64, 20, SSD1306_WHITE);
  displayTela.drawPixel(64, 19, SSD1306_WHITE);
  displayTela.drawPixel(64, 18, SSD1306_WHITE);
  displayTela.drawPixel(64, 17, SSD1306_WHITE);
  displayTela.drawPixel(64, 16, SSD1306_WHITE);
  displayTela.drawPixel(64, 15, SSD1306_WHITE);
  displayTela.drawPixel(64, 14, SSD1306_WHITE);
  displayTela.drawPixel(64, 13, SSD1306_WHITE);
  displayTela.drawPixel(64, 12, SSD1306_WHITE);
  displayTela.drawPixel(64, 11, SSD1306_WHITE);
  displayTela.drawPixel(64, 10, SSD1306_WHITE);
  displayTela.drawPixel(64, 9, SSD1306_WHITE);
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 6, SSD1306_WHITE);
  displayTela.drawPixel(64, 5, SSD1306_WHITE);
  displayTela.drawPixel(64, 4, SSD1306_WHITE);
  //id: 2 pixel 2 
  displayTela.drawPixel(65, 58, SSD1306_WHITE);
  //id: 3 pixel 3 
  displayTela.drawPixel(62, 58, SSD1306_WHITE);
  //id: 4 pixel 4 
  displayTela.drawPixel(61, 57, SSD1306_WHITE);
  displayTela.drawPixel(60, 57, SSD1306_WHITE);
  //id: 5 pixel 5 
  displayTela.drawPixel(65, 57, SSD1306_WHITE);
  displayTela.drawPixel(66, 57, SSD1306_WHITE);
  displayTela.drawPixel(67, 57, SSD1306_WHITE);
  displayTela.drawPixel(64, 57, SSD1306_WHITE);
  displayTela.drawPixel(63, 57, SSD1306_WHITE);
  displayTela.drawPixel(62, 57, SSD1306_WHITE);
  displayTela.drawPixel(61, 57, SSD1306_WHITE);
  displayTela.drawPixel(61, 56, SSD1306_WHITE);
  displayTela.drawPixel(60, 56, SSD1306_WHITE);
  displayTela.drawPixel(59, 56, SSD1306_WHITE);
  displayTela.drawPixel(60, 55, SSD1306_WHITE);
  displayTela.drawPixel(61, 55, SSD1306_WHITE);
  displayTela.drawPixel(62, 55, SSD1306_WHITE);
  displayTela.drawPixel(63, 55, SSD1306_WHITE);
  displayTela.drawPixel(64, 55, SSD1306_WHITE);
  displayTela.drawPixel(62, 56, SSD1306_WHITE);
  displayTela.drawPixel(63, 56, SSD1306_WHITE);
  displayTela.drawPixel(64, 56, SSD1306_WHITE);
  displayTela.drawPixel(65, 56, SSD1306_WHITE);
  displayTela.drawPixel(66, 56, SSD1306_WHITE);
  displayTela.drawPixel(67, 56, SSD1306_WHITE);
  displayTela.drawPixel(68, 56, SSD1306_WHITE);
  //id: 6 pixel 6 
  displayTela.drawPixel(56, 55, SSD1306_WHITE);
  displayTela.drawPixel(57, 55, SSD1306_WHITE);
  displayTela.drawPixel(58, 55, SSD1306_WHITE);
  displayTela.drawPixel(59, 55, SSD1306_WHITE);
  displayTela.drawPixel(60, 55, SSD1306_WHITE);
  displayTela.drawPixel(61, 55, SSD1306_WHITE);
  displayTela.drawPixel(62, 55, SSD1306_WHITE);
  displayTela.drawPixel(63, 55, SSD1306_WHITE);
  displayTela.drawPixel(64, 55, SSD1306_WHITE);
  displayTela.drawPixel(65, 55, SSD1306_WHITE);
  displayTela.drawPixel(66, 55, SSD1306_WHITE);
  displayTela.drawPixel(67, 55, SSD1306_WHITE);
  displayTela.drawPixel(68, 55, SSD1306_WHITE);
  displayTela.drawPixel(69, 55, SSD1306_WHITE);
  displayTela.drawPixel(69, 54, SSD1306_WHITE);
  displayTela.drawPixel(70, 54, SSD1306_WHITE);
  displayTela.drawPixel(71, 54, SSD1306_WHITE);
  displayTela.drawPixel(72, 54, SSD1306_WHITE);
  displayTela.drawPixel(73, 54, SSD1306_WHITE);
  displayTela.drawPixel(74, 54, SSD1306_WHITE);
  //id: 7 pixel 7 
  displayTela.drawPixel(70, 55, SSD1306_WHITE);
  displayTela.drawPixel(71, 55, SSD1306_WHITE);
  displayTela.drawPixel(72, 55, SSD1306_WHITE);
  displayTela.drawPixel(73, 55, SSD1306_WHITE);
  displayTela.drawPixel(74, 55, SSD1306_WHITE);
  displayTela.drawPixel(74, 54, SSD1306_WHITE);
  displayTela.drawPixel(75, 54, SSD1306_WHITE);
  displayTela.drawPixel(76, 54, SSD1306_WHITE);
  displayTela.drawPixel(77, 54, SSD1306_WHITE);
  displayTela.drawPixel(78, 54, SSD1306_WHITE);
  displayTela.drawPixel(76, 55, SSD1306_WHITE);
  displayTela.drawPixel(75, 55, SSD1306_WHITE);
  displayTela.drawPixel(71, 56, SSD1306_WHITE);
  displayTela.drawPixel(70, 56, SSD1306_WHITE);
  displayTela.drawPixel(69, 56, SSD1306_WHITE);
  displayTela.drawPixel(68, 56, SSD1306_WHITE);
  displayTela.drawPixel(67, 56, SSD1306_WHITE);
  displayTela.drawPixel(66, 56, SSD1306_WHITE);
  displayTela.drawPixel(65, 56, SSD1306_WHITE);
  displayTela.drawPixel(64, 56, SSD1306_WHITE);
  displayTela.drawPixel(63, 56, SSD1306_WHITE);
  displayTela.drawPixel(62, 56, SSD1306_WHITE);
  displayTela.drawPixel(61, 56, SSD1306_WHITE);
  displayTela.drawPixel(60, 56, SSD1306_WHITE);
  displayTela.drawPixel(59, 56, SSD1306_WHITE);
  displayTela.drawPixel(58, 56, SSD1306_WHITE);
  displayTela.drawPixel(57, 56, SSD1306_WHITE);
  displayTela.drawPixel(56, 56, SSD1306_WHITE);
  displayTela.drawPixel(55, 56, SSD1306_WHITE);
  displayTela.drawPixel(54, 56, SSD1306_WHITE);
  //id: 8 pixel 8 
  displayTela.drawPixel(58, 56, SSD1306_BLACK);
  displayTela.drawPixel(57, 56, SSD1306_BLACK);
  displayTela.drawPixel(56, 56, SSD1306_BLACK);
  displayTela.drawPixel(55, 56, SSD1306_BLACK);
  displayTela.drawPixel(54, 56, SSD1306_BLACK);
  displayTela.drawPixel(53, 56, SSD1306_BLACK);
  displayTela.drawPixel(52, 56, SSD1306_BLACK);
  //id: 9 pixel 9 
  displayTela.drawPixel(69, 56, SSD1306_BLACK);
  displayTela.drawPixel(70, 56, SSD1306_BLACK);
  displayTela.drawPixel(71, 56, SSD1306_BLACK);
  displayTela.drawPixel(72, 56, SSD1306_BLACK);
  displayTela.drawPixel(73, 56, SSD1306_BLACK);
  displayTela.drawPixel(73, 57, SSD1306_BLACK);
  //id: 10 line 10 
  displayTela.drawLine(58, 55, 41, 55, SSD1306_WHITE);
  //id: 11 line 11 
  displayTela.drawLine(73, 55, 89, 55, SSD1306_WHITE);
  //id: 12 line 12 
  displayTela.drawLine(40, 55, 38, 55, SSD1306_WHITE);
  //id: 13 line 13 
  displayTela.drawLine(62, 3, 65, 3, SSD1306_WHITE);
  //id: 14 line 14 
  displayTela.drawLine(66, 4, 66, 4, SSD1306_WHITE);
  //id: 15 line 15 
  displayTela.drawLine(67, 4, 67, 4, SSD1306_WHITE);
  //id: 16 line 16 
  displayTela.drawLine(68, 5, 68, 5, SSD1306_WHITE);
  //id: 17 line 17 
  displayTela.drawLine(61, 3, 60, 4, SSD1306_WHITE);
  //id: 18 line 18 
  displayTela.drawLine(61, 4, 61, 4, SSD1306_WHITE);
  //id: 19 line 19 
  displayTela.drawLine(59, 5, 59, 5, SSD1306_WHITE);
  //id: 20 line 20 
  displayTela.drawLine(61, 3, 61, 3, SSD1306_WHITE);
  //id: 21 line 21 
  displayTela.drawLine(55, 5, 55, 5, SSD1306_WHITE);
  //id: 22 line 22 
  displayTela.drawLine(55, 5, 55, 5, SSD1306_BLACK);
  //id: 23 line 23 
  displayTela.drawLine(61, 3, 61, 3, SSD1306_BLACK);
  //id: 24 line 24 
  displayTela.drawLine(57, 6, 70, 6, SSD1306_WHITE);
  //id: 25 line 25 
  displayTela.drawLine(49, 25, 49, 25, SSD1306_WHITE);
  //id: 26 line 26 
  displayTela.drawLine(74, 25, 74, 25, SSD1306_WHITE);
  //id: 27 line 27 
  displayTela.drawLine(76, 25, 76, 25, SSD1306_WHITE);
  //id: 28 line 28 
  displayTela.drawLine(50, 24, 50, 22, SSD1306_WHITE);
  //id: 29 line 29 
  displayTela.drawLine(51, 21, 51, 17, SSD1306_WHITE);
  //id: 30 line 30 
  displayTela.drawLine(52, 16, 52, 15, SSD1306_WHITE);
  //id: 31 line 31 
  displayTela.drawLine(53, 14, 53, 12, SSD1306_WHITE);
  //id: 32 line 32 
  displayTela.drawLine(54, 11, 54, 10, SSD1306_WHITE);
  //id: 33 line 33 
  displayTela.drawLine(56, 7, 55, 7, SSD1306_WHITE);
  //id: 34 line 34 
  displayTela.drawLine(55, 8, 55, 8, SSD1306_WHITE);
  //id: 35 line 35 
  displayTela.drawLine(54, 9, 54, 9, SSD1306_WHITE);
  //id: 36 line 36 
  displayTela.drawLine(55, 7, 55, 7, SSD1306_BLACK);
  //id: 37 line 37 
  displayTela.drawLine(71, 7, 71, 7, SSD1306_WHITE);
  //id: 38 line 38 
  displayTela.drawLine(72, 8, 72, 8, SSD1306_WHITE);
  //id: 39 line 39 
  displayTela.drawLine(73, 9, 73, 11, SSD1306_WHITE);
  //id: 40 line 40 
  displayTela.drawLine(74, 12, 74, 14, SSD1306_WHITE);
  //id: 41 line 41 
  displayTela.drawLine(74, 14, 74, 14, SSD1306_WHITE);
  //id: 42 line 42 
  displayTela.drawLine(75, 15, 75, 16, SSD1306_WHITE);
  //id: 43 line 43 
  displayTela.drawLine(76, 17, 76, 21, SSD1306_WHITE);
  //id: 44 line 44 
  displayTela.drawLine(77, 22, 77, 24, SSD1306_WHITE);
  //id: 45 line 45 
  displayTela.drawLine(78, 25, 78, 25, SSD1306_WHITE);
  //id: 46 line 46 
  displayTela.drawLine(73, 24, 76, 25, SSD1306_BLACK);
  //id: 47 line 47 
  displayTela.drawLine(73, 25, 73, 25, SSD1306_BLACK);
  //id: 48 line 48 
  displayTela.drawLine(74, 25, 74, 25, SSD1306_BLACK);
  //id: 49 line 49 
  displayTela.drawLine(63, 4, 64, 4, SSD1306_BLACK);
  //id: 50 line 50 
  displayTela.drawLine(63, 5, 64, 5, SSD1306_BLACK);
  //id: 51 line 51 
  displayTela.drawLine(35, 55, 37, 55, SSD1306_WHITE);
  //id: 52 line 52 
  displayTela.drawLine(36, 54, 37, 52, SSD1306_WHITE);
  //id: 53 line 53 
  displayTela.drawLine(37, 52, 37, 47, SSD1306_WHITE);
  //id: 54 line 54 
  displayTela.drawLine(38, 46, 38, 40, SSD1306_WHITE);
  //id: 55 line 55 
  displayTela.drawLine(39, 40, 40, 37, SSD1306_WHITE);
  //id: 56 line 56 
  displayTela.drawLine(39, 40, 40, 37, SSD1306_BLACK);
  //id: 57 line 57 
  displayTela.drawLine(39, 39, 39, 37, SSD1306_WHITE);
  //id: 58 line 58 
  displayTela.drawLine(40, 36, 41, 33, SSD1306_WHITE);
  //id: 59 line 59 
  displayTela.drawLine(42, 32, 43, 31, SSD1306_WHITE);
  //id: 60 line 60 
  displayTela.drawLine(44, 30, 45, 29, SSD1306_WHITE);
  //id: 61 line 61 
  displayTela.drawLine(46, 29, 46, 29, SSD1306_WHITE);
  //id: 62 line 62 
  displayTela.drawLine(47, 28, 47, 28, SSD1306_WHITE);
  //id: 63 line 63 
  displayTela.drawLine(48, 26, 47, 27, SSD1306_WHITE);
  //id: 64 line 64 
  displayTela.drawLine(47, 27, 47, 27, SSD1306_WHITE);
  //id: 65 line 65 
  displayTela.drawLine(79, 26, 79, 26, SSD1306_WHITE);
  //id: 66 line 66 
  displayTela.drawLine(80, 27, 80, 28, SSD1306_WHITE);
  //id: 67 line 67 
  displayTela.drawLine(81, 29, 82, 29, SSD1306_WHITE);
  //id: 68 line 68 
  displayTela.drawLine(83, 30, 83, 30, SSD1306_WHITE);
  //id: 69 line 69 
  displayTela.drawLine(84, 31, 84, 31, SSD1306_WHITE);
  //id: 70 line 70 
  displayTela.drawLine(85, 32, 85, 32, SSD1306_WHITE);
  //id: 71 line 71 
  displayTela.drawLine(86, 33, 86, 34, SSD1306_WHITE);
  //id: 72 line 72 
  displayTela.drawLine(87, 35, 87, 36, SSD1306_WHITE);
  //id: 73 line 73 
  displayTela.drawLine(88, 37, 88, 39, SSD1306_WHITE);
  //id: 74 line 74 
  displayTela.drawLine(89, 40, 89, 46, SSD1306_WHITE);
  //id: 75 line 75 
  displayTela.drawLine(90, 47, 90, 52, SSD1306_WHITE);
  //id: 76 line 76 
  displayTela.drawLine(91, 53, 91, 54, SSD1306_WHITE);
  //id: 77 line 77 
  displayTela.drawLine(89, 55, 92, 55, SSD1306_WHITE);
  //id: 78 fillCircle 78 
  displayTela.fillCircle(64, 13, 0, SSD1306_WHITE);
  //id: 79 fillCircle 79 
  displayTela.fillCircle(62, 19, 9, SSD1306_WHITE);
  //id: 80 fillCircle 80 
  displayTela.fillCircle(51, 44, 11, SSD1306_WHITE);
  //id: 81 fillCircle 81 
  displayTela.fillCircle(77, 44, 11, SSD1306_WHITE);
  //id: 82 fillCircle 83 
  displayTela.fillCircle(64, 34, 15, SSD1306_WHITE);
  //id: 83 pixel 84 
  displayTela.drawPixel(61, 51, SSD1306_WHITE);
  displayTela.drawPixel(60, 51, SSD1306_WHITE);
  displayTela.drawPixel(60, 52, SSD1306_WHITE);
  displayTela.drawPixel(61, 52, SSD1306_WHITE);
  displayTela.drawPixel(62, 51, SSD1306_WHITE);
  displayTela.drawPixel(64, 51, SSD1306_WHITE);
  displayTela.drawPixel(65, 51, SSD1306_WHITE);
  displayTela.drawPixel(66, 51, SSD1306_WHITE);
  displayTela.drawPixel(68, 50, SSD1306_WHITE);
  displayTela.drawPixel(69, 50, SSD1306_WHITE);
  displayTela.drawPixel(70, 50, SSD1306_WHITE);
  displayTela.drawPixel(71, 50, SSD1306_WHITE);
  displayTela.drawPixel(71, 49, SSD1306_WHITE);
  displayTela.drawPixel(66, 50, SSD1306_WHITE);
  displayTela.drawPixel(65, 50, SSD1306_WHITE);
  displayTela.drawPixel(63, 51, SSD1306_WHITE);
  displayTela.drawPixel(59, 51, SSD1306_WHITE);
  displayTela.drawPixel(61, 50, SSD1306_WHITE);
  displayTela.drawPixel(62, 50, SSD1306_WHITE);
  displayTela.drawPixel(63, 50, SSD1306_WHITE);
  displayTela.drawPixel(64, 50, SSD1306_WHITE);
  displayTela.drawPixel(64, 49, SSD1306_WHITE);
  displayTela.drawPixel(65, 49, SSD1306_WHITE);
  displayTela.drawPixel(63, 49, SSD1306_WHITE);
  displayTela.drawPixel(60, 50, SSD1306_WHITE);
  displayTela.drawPixel(66, 49, SSD1306_WHITE);
  displayTela.drawPixel(67, 49, SSD1306_WHITE);
  displayTela.drawPixel(68, 48, SSD1306_WHITE);
  displayTela.drawPixel(69, 48, SSD1306_WHITE);
  displayTela.drawPixel(69, 49, SSD1306_WHITE);
  displayTela.drawPixel(68, 49, SSD1306_WHITE);
  displayTela.drawPixel(62, 52, SSD1306_WHITE);
  displayTela.drawPixel(63, 52, SSD1306_WHITE);
  displayTela.drawPixel(64, 52, SSD1306_WHITE);
  displayTela.drawPixel(67, 51, SSD1306_WHITE);
  displayTela.drawPixel(59, 52, SSD1306_WHITE);
  displayTela.drawPixel(59, 53, SSD1306_WHITE);
  displayTela.drawPixel(58, 53, SSD1306_WHITE);
  displayTela.drawPixel(60, 53, SSD1306_WHITE);
  displayTela.drawPixel(65, 52, SSD1306_WHITE);
  displayTela.drawPixel(67, 52, SSD1306_WHITE);
  displayTela.drawPixel(68, 52, SSD1306_WHITE);
  displayTela.drawPixel(69, 52, SSD1306_WHITE);
  displayTela.drawPixel(70, 52, SSD1306_WHITE);
  displayTela.drawPixel(71, 52, SSD1306_WHITE);
  displayTela.drawPixel(72, 53, SSD1306_WHITE);
  displayTela.drawPixel(72, 54, SSD1306_WHITE);
  displayTela.drawPixel(71, 54, SSD1306_WHITE);
  displayTela.drawPixel(70, 54, SSD1306_WHITE);
  displayTela.drawPixel(69, 54, SSD1306_WHITE);
  displayTela.drawPixel(68, 54, SSD1306_WHITE);
  displayTela.drawPixel(67, 54, SSD1306_WHITE);
  displayTela.drawPixel(65, 54, SSD1306_WHITE);
  displayTela.drawPixel(63, 54, SSD1306_WHITE);
  displayTela.drawPixel(61, 54, SSD1306_WHITE);
  displayTela.drawPixel(60, 54, SSD1306_WHITE);
  displayTela.drawPixel(59, 54, SSD1306_WHITE);
  displayTela.drawPixel(58, 54, SSD1306_WHITE);
  displayTela.drawPixel(57, 54, SSD1306_WHITE);
  displayTela.drawPixel(56, 54, SSD1306_WHITE);
  displayTela.drawPixel(62, 54, SSD1306_WHITE);
  displayTela.drawPixel(64, 54, SSD1306_WHITE);
  displayTela.drawPixel(66, 54, SSD1306_WHITE);
  displayTela.drawPixel(57, 53, SSD1306_WHITE);
  displayTela.drawPixel(67, 50, SSD1306_WHITE);
  displayTela.drawPixel(68, 51, SSD1306_WHITE);
  displayTela.drawPixel(68, 53, SSD1306_WHITE);
  displayTela.drawPixel(70, 53, SSD1306_WHITE);
  displayTela.drawPixel(71, 53, SSD1306_WHITE);
  displayTela.drawPixel(73, 53, SSD1306_WHITE);
  displayTela.drawPixel(74, 53, SSD1306_WHITE);
  displayTela.drawPixel(75, 53, SSD1306_WHITE);
  displayTela.drawPixel(76, 53, SSD1306_WHITE);
  displayTela.drawPixel(69, 53, SSD1306_WHITE);
  displayTela.drawPixel(66, 53, SSD1306_WHITE);
  displayTela.drawPixel(64, 53, SSD1306_WHITE);
  displayTela.drawPixel(61, 53, SSD1306_WHITE);
  displayTela.drawPixel(56, 53, SSD1306_WHITE);
  displayTela.drawPixel(57, 52, SSD1306_WHITE);
  displayTela.drawPixel(58, 52, SSD1306_WHITE);
  displayTela.drawPixel(66, 52, SSD1306_WHITE);
  displayTela.drawPixel(72, 52, SSD1306_WHITE);
  displayTela.drawPixel(73, 52, SSD1306_WHITE);
  displayTela.drawPixel(62, 53, SSD1306_WHITE);
  displayTela.drawPixel(63, 53, SSD1306_WHITE);
  displayTela.drawPixel(65, 53, SSD1306_WHITE);
  displayTela.drawPixel(67, 53, SSD1306_WHITE);
  //id: 84 pixel 85 
  displayTela.drawPixel(46, 54, SSD1306_WHITE);
  displayTela.drawPixel(45, 54, SSD1306_WHITE);
  displayTela.drawPixel(44, 54, SSD1306_WHITE);
  displayTela.drawPixel(43, 54, SSD1306_WHITE);
  displayTela.drawPixel(42, 54, SSD1306_WHITE);
  displayTela.drawPixel(41, 54, SSD1306_WHITE);
  displayTela.drawPixel(40, 54, SSD1306_WHITE);
  displayTela.drawPixel(39, 54, SSD1306_WHITE);
  displayTela.drawPixel(38, 54, SSD1306_WHITE);
  displayTela.drawPixel(37, 54, SSD1306_WHITE);
  displayTela.drawPixel(37, 53, SSD1306_WHITE);
  displayTela.drawPixel(38, 53, SSD1306_WHITE);
  displayTela.drawPixel(39, 53, SSD1306_WHITE);
  displayTela.drawPixel(40, 53, SSD1306_WHITE);
  displayTela.drawPixel(40, 52, SSD1306_WHITE);
  displayTela.drawPixel(41, 52, SSD1306_WHITE);
  displayTela.drawPixel(42, 52, SSD1306_WHITE);
  displayTela.drawPixel(43, 52, SSD1306_WHITE);
  displayTela.drawPixel(43, 53, SSD1306_WHITE);
  displayTela.drawPixel(42, 53, SSD1306_WHITE);
  displayTela.drawPixel(41, 53, SSD1306_WHITE);
  displayTela.drawPixel(40, 51, SSD1306_WHITE);
  displayTela.drawPixel(41, 51, SSD1306_WHITE);
  displayTela.drawPixel(39, 51, SSD1306_WHITE);
  displayTela.drawPixel(38, 51, SSD1306_WHITE);
  displayTela.drawPixel(38, 52, SSD1306_WHITE);
  displayTela.drawPixel(37, 52, SSD1306_WHITE);
  //id: 85 pixel 86 
  displayTela.drawPixel(37, 52, SSD1306_WHITE);
  displayTela.drawPixel(38, 52, SSD1306_WHITE);
  displayTela.drawPixel(39, 52, SSD1306_WHITE);
  displayTela.drawPixel(39, 51, SSD1306_WHITE);
  displayTela.drawPixel(40, 51, SSD1306_WHITE);
  displayTela.drawPixel(40, 50, SSD1306_WHITE);
  displayTela.drawPixel(41, 50, SSD1306_WHITE);
  displayTela.drawPixel(39, 50, SSD1306_WHITE);
  displayTela.drawPixel(38, 50, SSD1306_WHITE);
  displayTela.drawPixel(38, 49, SSD1306_WHITE);
  displayTela.drawPixel(38, 48, SSD1306_WHITE);
  displayTela.drawPixel(38, 47, SSD1306_WHITE);
  displayTela.drawPixel(38, 46, SSD1306_WHITE);
  displayTela.drawPixel(39, 46, SSD1306_WHITE);
  displayTela.drawPixel(39, 47, SSD1306_WHITE);
  displayTela.drawPixel(39, 48, SSD1306_WHITE);
  displayTela.drawPixel(39, 49, SSD1306_WHITE);
  displayTela.drawPixel(40, 47, SSD1306_WHITE);
  displayTela.drawPixel(40, 48, SSD1306_WHITE);
  displayTela.drawPixel(40, 46, SSD1306_WHITE);
  displayTela.drawPixel(40, 45, SSD1306_WHITE);
  displayTela.drawPixel(40, 44, SSD1306_WHITE);
  displayTela.drawPixel(40, 49, SSD1306_WHITE);
  displayTela.drawPixel(38, 45, SSD1306_WHITE);
  displayTela.drawPixel(39, 44, SSD1306_WHITE);
  displayTela.drawPixel(39, 43, SSD1306_WHITE);
  displayTela.drawPixel(39, 42, SSD1306_WHITE);
  displayTela.drawPixel(39, 41, SSD1306_WHITE);
  displayTela.drawPixel(39, 40, SSD1306_WHITE);
  displayTela.drawPixel(40, 40, SSD1306_WHITE);
  displayTela.drawPixel(40, 39, SSD1306_WHITE);
  displayTela.drawPixel(40, 38, SSD1306_WHITE);
  displayTela.drawPixel(40, 37, SSD1306_WHITE);
  displayTela.drawPixel(39, 39, SSD1306_WHITE);
  displayTela.drawPixel(38, 42, SSD1306_WHITE);
  displayTela.drawPixel(38, 43, SSD1306_WHITE);
  displayTela.drawPixel(38, 44, SSD1306_WHITE);
  displayTela.drawPixel(39, 45, SSD1306_WHITE);
  displayTela.drawPixel(40, 43, SSD1306_WHITE);
  displayTela.drawPixel(40, 42, SSD1306_WHITE);
  displayTela.drawPixel(40, 41, SSD1306_WHITE);
  displayTela.drawPixel(41, 40, SSD1306_WHITE);
  displayTela.drawPixel(41, 39, SSD1306_WHITE);
  displayTela.drawPixel(41, 38, SSD1306_WHITE);
  displayTela.drawPixel(41, 37, SSD1306_WHITE);
  displayTela.drawPixel(41, 36, SSD1306_WHITE);
  displayTela.drawPixel(42, 36, SSD1306_WHITE);
  displayTela.drawPixel(42, 35, SSD1306_WHITE);
  displayTela.drawPixel(42, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 34, SSD1306_WHITE);
  displayTela.drawPixel(43, 33, SSD1306_WHITE);
  displayTela.drawPixel(44, 33, SSD1306_WHITE);
  displayTela.drawPixel(44, 32, SSD1306_WHITE);
  displayTela.drawPixel(45, 31, SSD1306_WHITE);
  displayTela.drawPixel(46, 31, SSD1306_WHITE);
  displayTela.drawPixel(47, 31, SSD1306_WHITE);
  displayTela.drawPixel(47, 30, SSD1306_WHITE);
  displayTela.drawPixel(48, 30, SSD1306_WHITE);
  displayTela.drawPixel(49, 30, SSD1306_WHITE);
  displayTela.drawPixel(50, 30, SSD1306_WHITE);
  displayTela.drawPixel(48, 31, SSD1306_WHITE);
  displayTela.drawPixel(46, 32, SSD1306_WHITE);
  displayTela.drawPixel(45, 32, SSD1306_WHITE);
  displayTela.drawPixel(45, 33, SSD1306_WHITE);
  displayTela.drawPixel(43, 36, SSD1306_WHITE);
  displayTela.drawPixel(43, 35, SSD1306_WHITE);
  displayTela.drawPixel(44, 35, SSD1306_WHITE);
  displayTela.drawPixel(45, 35, SSD1306_WHITE);
  displayTela.drawPixel(45, 34, SSD1306_WHITE);
  displayTela.drawPixel(46, 34, SSD1306_WHITE);
  displayTela.drawPixel(47, 34, SSD1306_WHITE);
  displayTela.drawPixel(47, 33, SSD1306_WHITE);
  displayTela.drawPixel(48, 33, SSD1306_WHITE);
  displayTela.drawPixel(49, 33, SSD1306_WHITE);
  displayTela.drawPixel(49, 32, SSD1306_WHITE);
  displayTela.drawPixel(48, 32, SSD1306_WHITE);
  displayTela.drawPixel(46, 33, SSD1306_WHITE);
  displayTela.drawPixel(44, 34, SSD1306_WHITE);
  displayTela.drawPixel(47, 32, SSD1306_WHITE);
  displayTela.drawPixel(41, 34, SSD1306_WHITE);
  displayTela.drawPixel(41, 35, SSD1306_WHITE);
  displayTela.drawPixel(42, 33, SSD1306_WHITE);
  displayTela.drawPixel(42, 32, SSD1306_WHITE);
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  displayTela.drawPixel(44, 31, SSD1306_WHITE);
  displayTela.drawPixel(45, 30, SSD1306_WHITE);
  displayTela.drawPixel(46, 30, SSD1306_WHITE);
  displayTela.drawPixel(46, 29, SSD1306_WHITE);
  displayTela.drawPixel(47, 29, SSD1306_WHITE);
  displayTela.drawPixel(48, 29, SSD1306_WHITE);
  displayTela.drawPixel(48, 28, SSD1306_WHITE);
  displayTela.drawPixel(49, 28, SSD1306_WHITE);
  displayTela.drawPixel(49, 27, SSD1306_WHITE);
  displayTela.drawPixel(49, 26, SSD1306_WHITE);
  displayTela.drawPixel(50, 26, SSD1306_WHITE);
  displayTela.drawPixel(50, 25, SSD1306_WHITE);
  displayTela.drawPixel(50, 24, SSD1306_WHITE);
  displayTela.drawPixel(51, 24, SSD1306_WHITE);
  displayTela.drawPixel(52, 24, SSD1306_WHITE);
  displayTela.drawPixel(52, 23, SSD1306_WHITE);
  displayTela.drawPixel(53, 23, SSD1306_WHITE);
  displayTela.drawPixel(53, 22, SSD1306_WHITE);
  displayTela.drawPixel(54, 22, SSD1306_WHITE);
  displayTela.drawPixel(52, 25, SSD1306_WHITE);
  displayTela.drawPixel(51, 25, SSD1306_WHITE);
  displayTela.drawPixel(51, 26, SSD1306_WHITE);
  displayTela.drawPixel(50, 27, SSD1306_WHITE);
  displayTela.drawPixel(50, 28, SSD1306_WHITE);
  displayTela.drawPixel(50, 29, SSD1306_WHITE);
  displayTela.drawPixel(48, 27, SSD1306_WHITE);
  displayTela.drawPixel(49, 29, SSD1306_WHITE);
  displayTela.drawPixel(51, 23, SSD1306_WHITE);
  displayTela.drawPixel(52, 22, SSD1306_WHITE);
  displayTela.drawPixel(52, 21, SSD1306_WHITE);
  displayTela.drawPixel(53, 21, SSD1306_WHITE);
  displayTela.drawPixel(53, 20, SSD1306_WHITE);
  displayTela.drawPixel(53, 19, SSD1306_WHITE);
  displayTela.drawPixel(53, 18, SSD1306_WHITE);
  displayTela.drawPixel(53, 17, SSD1306_WHITE);
  displayTela.drawPixel(53, 16, SSD1306_WHITE);
  displayTela.drawPixel(53, 15, SSD1306_WHITE);
  displayTela.drawPixel(52, 17, SSD1306_WHITE);
  displayTela.drawPixel(52, 18, SSD1306_WHITE);
  displayTela.drawPixel(52, 19, SSD1306_WHITE);
  displayTela.drawPixel(52, 20, SSD1306_WHITE);
  displayTela.drawPixel(51, 22, SSD1306_WHITE);
  displayTela.drawPixel(55, 22, SSD1306_WHITE);
  displayTela.drawPixel(56, 22, SSD1306_WHITE);
  displayTela.drawPixel(58, 22, SSD1306_WHITE);
  displayTela.drawPixel(59, 22, SSD1306_WHITE);
  displayTela.drawPixel(60, 23, SSD1306_WHITE);
  displayTela.drawPixel(61, 23, SSD1306_WHITE);
  displayTela.drawPixel(62, 23, SSD1306_WHITE);
  displayTela.drawPixel(63, 23, SSD1306_WHITE);
  displayTela.drawPixel(64, 23, SSD1306_WHITE);
  displayTela.drawPixel(65, 23, SSD1306_WHITE);
  displayTela.drawPixel(65, 24, SSD1306_WHITE);
  displayTela.drawPixel(66, 24, SSD1306_WHITE);
  displayTela.drawPixel(67, 24, SSD1306_WHITE);
  displayTela.drawPixel(68, 25, SSD1306_WHITE);
  displayTela.drawPixel(69, 25, SSD1306_WHITE);
  displayTela.drawPixel(70, 25, SSD1306_WHITE);
  displayTela.drawPixel(71, 25, SSD1306_WHITE);
  displayTela.drawPixel(72, 25, SSD1306_WHITE);
  displayTela.drawPixel(72, 26, SSD1306_WHITE);
  displayTela.drawPixel(73, 26, SSD1306_WHITE);
  displayTela.drawPixel(74, 26, SSD1306_WHITE);
  displayTela.drawPixel(75, 27, SSD1306_WHITE);
  displayTela.drawPixel(76, 27, SSD1306_WHITE);
  displayTela.drawPixel(76, 28, SSD1306_WHITE);
  displayTela.drawPixel(77, 28, SSD1306_WHITE);
  displayTela.drawPixel(78, 28, SSD1306_WHITE);
  displayTela.drawPixel(78, 29, SSD1306_WHITE);
  displayTela.drawPixel(79, 29, SSD1306_WHITE);
  displayTela.drawPixel(79, 30, SSD1306_WHITE);
  displayTela.drawPixel(80, 30, SSD1306_WHITE);
  displayTela.drawPixel(80, 31, SSD1306_WHITE);
  displayTela.drawPixel(80, 32, SSD1306_WHITE);
  displayTela.drawPixel(80, 33, SSD1306_WHITE);
  displayTela.drawPixel(81, 33, SSD1306_WHITE);
  displayTela.drawPixel(81, 34, SSD1306_WHITE);
  displayTela.drawPixel(81, 35, SSD1306_WHITE);
  displayTela.drawPixel(81, 36, SSD1306_WHITE);
  displayTela.drawPixel(82, 36, SSD1306_WHITE);
  displayTela.drawPixel(82, 37, SSD1306_WHITE);
  displayTela.drawPixel(83, 37, SSD1306_WHITE);
  displayTela.drawPixel(83, 38, SSD1306_WHITE);
  displayTela.drawPixel(84, 38, SSD1306_WHITE);
  displayTela.drawPixel(85, 38, SSD1306_WHITE);
  displayTela.drawPixel(86, 38, SSD1306_WHITE);
  displayTela.drawPixel(87, 38, SSD1306_WHITE);
  displayTela.drawPixel(87, 37, SSD1306_WHITE);
  displayTela.drawPixel(86, 36, SSD1306_WHITE);
  displayTela.drawPixel(85, 36, SSD1306_WHITE);
  displayTela.drawPixel(85, 35, SSD1306_WHITE);
  displayTela.drawPixel(85, 34, SSD1306_WHITE);
  displayTela.drawPixel(84, 34, SSD1306_WHITE);
  displayTela.drawPixel(83, 34, SSD1306_WHITE);
  displayTela.drawPixel(82, 33, SSD1306_WHITE);
  displayTela.drawPixel(81, 30, SSD1306_WHITE);
  displayTela.drawPixel(81, 31, SSD1306_WHITE);
  displayTela.drawPixel(82, 31, SSD1306_WHITE);
  displayTela.drawPixel(83, 31, SSD1306_WHITE);
  displayTela.drawPixel(83, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 33, SSD1306_WHITE);
  displayTela.drawPixel(84, 35, SSD1306_WHITE);
  displayTela.drawPixel(83, 35, SSD1306_WHITE);
  displayTela.drawPixel(82, 34, SSD1306_WHITE);
  displayTela.drawPixel(81, 32, SSD1306_WHITE);
  displayTela.drawPixel(82, 32, SSD1306_WHITE);
  displayTela.drawPixel(83, 33, SSD1306_WHITE);
  displayTela.drawPixel(82, 30, SSD1306_WHITE);
  displayTela.drawPixel(85, 33, SSD1306_WHITE);
  displayTela.drawPixel(86, 35, SSD1306_WHITE);
  displayTela.drawPixel(86, 37, SSD1306_WHITE);
  displayTela.drawPixel(87, 39, SSD1306_WHITE);
  displayTela.drawPixel(88, 39, SSD1306_WHITE);
  displayTela.drawPixel(88, 40, SSD1306_WHITE);
  displayTela.drawPixel(88, 41, SSD1306_WHITE);
  displayTela.drawPixel(88, 42, SSD1306_WHITE);
  displayTela.drawPixel(88, 43, SSD1306_WHITE);
  displayTela.drawPixel(88, 44, SSD1306_WHITE);
  displayTela.drawPixel(88, 45, SSD1306_WHITE);
  displayTela.drawPixel(88, 46, SSD1306_WHITE);
  displayTela.drawPixel(89, 46, SSD1306_WHITE);
  displayTela.drawPixel(89, 47, SSD1306_WHITE);
  displayTela.drawPixel(89, 48, SSD1306_WHITE);
  displayTela.drawPixel(89, 49, SSD1306_WHITE);
  displayTela.drawPixel(89, 50, SSD1306_WHITE);
  displayTela.drawPixel(89, 51, SSD1306_WHITE);
  displayTela.drawPixel(89, 52, SSD1306_WHITE);
  displayTela.drawPixel(88, 52, SSD1306_WHITE);
  displayTela.drawPixel(88, 53, SSD1306_WHITE);
  displayTela.drawPixel(87, 53, SSD1306_WHITE);
  displayTela.drawPixel(86, 53, SSD1306_WHITE);
  displayTela.drawPixel(86, 54, SSD1306_WHITE);
  displayTela.drawPixel(85, 54, SSD1306_WHITE);
  displayTela.drawPixel(84, 54, SSD1306_WHITE);
  displayTela.drawPixel(83, 54, SSD1306_WHITE);
  displayTela.drawPixel(82, 54, SSD1306_WHITE);
  displayTela.drawPixel(87, 54, SSD1306_WHITE);
  displayTela.drawPixel(88, 54, SSD1306_WHITE);
  displayTela.drawPixel(89, 54, SSD1306_WHITE);
  displayTela.drawPixel(90, 54, SSD1306_WHITE);
  displayTela.drawPixel(90, 53, SSD1306_WHITE);
  displayTela.drawPixel(89, 53, SSD1306_WHITE);
  displayTela.drawPixel(86, 52, SSD1306_WHITE);
  displayTela.drawPixel(85, 52, SSD1306_WHITE);
  displayTela.drawPixel(84, 52, SSD1306_WHITE);
  displayTela.drawPixel(85, 51, SSD1306_WHITE);
  displayTela.drawPixel(86, 51, SSD1306_WHITE);
  displayTela.drawPixel(86, 50, SSD1306_WHITE);
  displayTela.drawPixel(86, 49, SSD1306_WHITE);
  displayTela.drawPixel(87, 49, SSD1306_WHITE);
  displayTela.drawPixel(87, 48, SSD1306_WHITE);
  displayTela.drawPixel(87, 50, SSD1306_WHITE);
  displayTela.drawPixel(87, 51, SSD1306_WHITE);
  displayTela.drawPixel(87, 52, SSD1306_WHITE);
  displayTela.drawPixel(88, 49, SSD1306_WHITE);
  displayTela.drawPixel(88, 50, SSD1306_WHITE);
  displayTela.drawPixel(88, 51, SSD1306_WHITE);
  displayTela.drawPixel(88, 48, SSD1306_WHITE);
  displayTela.drawPixel(88, 47, SSD1306_WHITE);
  displayTela.drawPixel(85, 53, SSD1306_WHITE);
  displayTela.drawPixel(84, 51, SSD1306_WHITE);
  displayTela.drawPixel(84, 50, SSD1306_WHITE);
  displayTela.drawPixel(84, 49, SSD1306_WHITE);
  displayTela.drawPixel(83, 48, SSD1306_WHITE);
  displayTela.drawPixel(83, 47, SSD1306_WHITE);
  displayTela.drawPixel(82, 47, SSD1306_WHITE);
  displayTela.drawPixel(82, 45, SSD1306_WHITE);
  displayTela.drawPixel(82, 44, SSD1306_WHITE);
  displayTela.drawPixel(81, 43, SSD1306_WHITE);
  displayTela.drawPixel(81, 42, SSD1306_WHITE);
  displayTela.drawPixel(81, 41, SSD1306_WHITE);
  displayTela.drawPixel(80, 40, SSD1306_WHITE);
  displayTela.drawPixel(80, 39, SSD1306_WHITE);
  displayTela.drawPixel(80, 38, SSD1306_WHITE);
  displayTela.drawPixel(79, 37, SSD1306_WHITE);
  displayTela.drawPixel(79, 36, SSD1306_WHITE);
  displayTela.drawPixel(79, 35, SSD1306_WHITE);
  displayTela.drawPixel(78, 35, SSD1306_WHITE);
  displayTela.drawPixel(78, 34, SSD1306_WHITE);
  displayTela.drawPixel(78, 33, SSD1306_WHITE);
  displayTela.drawPixel(78, 32, SSD1306_WHITE);
  displayTela.drawPixel(78, 31, SSD1306_WHITE);
  displayTela.drawPixel(78, 30, SSD1306_WHITE);
  displayTela.drawPixel(80, 29, SSD1306_WHITE);
  displayTela.drawPixel(80, 28, SSD1306_WHITE);
  displayTela.drawPixel(79, 28, SSD1306_WHITE);
  displayTela.drawPixel(78, 27, SSD1306_WHITE);
  displayTela.drawPixel(79, 27, SSD1306_WHITE);
  displayTela.drawPixel(78, 26, SSD1306_WHITE);
  displayTela.drawPixel(77, 26, SSD1306_WHITE);
  displayTela.drawPixel(77, 25, SSD1306_WHITE);
  displayTela.drawPixel(76, 25, SSD1306_WHITE);
  displayTela.drawPixel(76, 24, SSD1306_WHITE);
  displayTela.drawPixel(75, 24, SSD1306_WHITE);
  displayTela.drawPixel(75, 23, SSD1306_WHITE);
  displayTela.drawPixel(74, 22, SSD1306_WHITE);
  displayTela.drawPixel(74, 21, SSD1306_WHITE);
  displayTela.drawPixel(73, 21, SSD1306_WHITE);
  displayTela.drawPixel(73, 20, SSD1306_WHITE);
  displayTela.drawPixel(72, 20, SSD1306_WHITE);
  displayTela.drawPixel(71, 20, SSD1306_WHITE);
  displayTela.drawPixel(71, 19, SSD1306_WHITE);
  displayTela.drawPixel(70, 19, SSD1306_WHITE);
  displayTela.drawPixel(70, 18, SSD1306_WHITE);
  displayTela.drawPixel(69, 18, SSD1306_WHITE);
  displayTela.drawPixel(69, 17, SSD1306_WHITE);
  displayTela.drawPixel(69, 16, SSD1306_WHITE);
  displayTela.drawPixel(68, 15, SSD1306_WHITE);
  displayTela.drawPixel(68, 14, SSD1306_WHITE);
  displayTela.drawPixel(68, 13, SSD1306_WHITE);
  displayTela.drawPixel(68, 12, SSD1306_WHITE);
  displayTela.drawPixel(68, 11, SSD1306_WHITE);
  displayTela.drawPixel(67, 11, SSD1306_WHITE);
  displayTela.drawPixel(69, 11, SSD1306_WHITE);
  displayTela.drawPixel(70, 12, SSD1306_WHITE);
  displayTela.drawPixel(70, 13, SSD1306_WHITE);
  displayTela.drawPixel(71, 13, SSD1306_WHITE);
  displayTela.drawPixel(71, 14, SSD1306_WHITE);
  displayTela.drawPixel(71, 15, SSD1306_WHITE);
  displayTela.drawPixel(71, 16, SSD1306_WHITE);
  displayTela.drawPixel(72, 17, SSD1306_WHITE);
  displayTela.drawPixel(72, 18, SSD1306_WHITE);
  displayTela.drawPixel(72, 19, SSD1306_WHITE);
  displayTela.drawPixel(73, 19, SSD1306_WHITE);
  displayTela.drawPixel(75, 22, SSD1306_WHITE);
  displayTela.drawPixel(76, 23, SSD1306_WHITE);
  displayTela.drawPixel(76, 22, SSD1306_WHITE);
  displayTela.drawPixel(76, 21, SSD1306_WHITE);
  displayTela.drawPixel(76, 20, SSD1306_WHITE);
  displayTela.drawPixel(75, 20, SSD1306_WHITE);
  displayTela.drawPixel(75, 19, SSD1306_WHITE);
  displayTela.drawPixel(75, 18, SSD1306_WHITE);
  displayTela.drawPixel(75, 17, SSD1306_WHITE);
  displayTela.drawPixel(74, 17, SSD1306_WHITE);
  displayTela.drawPixel(74, 16, SSD1306_WHITE);
  displayTela.drawPixel(74, 15, SSD1306_WHITE);
  displayTela.drawPixel(74, 18, SSD1306_WHITE);
  displayTela.drawPixel(74, 19, SSD1306_WHITE);
  displayTela.drawPixel(75, 21, SSD1306_WHITE);
  displayTela.drawPixel(77, 23, SSD1306_WHITE);
  displayTela.drawPixel(77, 24, SSD1306_WHITE);
  displayTela.drawPixel(78, 24, SSD1306_WHITE);
  displayTela.drawPixel(78, 25, SSD1306_WHITE);
  displayTela.drawPixel(77, 22, SSD1306_WHITE);
  displayTela.drawPixel(74, 20, SSD1306_WHITE);
  displayTela.drawPixel(73, 18, SSD1306_WHITE);
  displayTela.drawPixel(72, 16, SSD1306_WHITE);
  displayTela.drawPixel(70, 15, SSD1306_WHITE);
  displayTela.drawPixel(70, 14, SSD1306_WHITE);
  displayTela.drawPixel(71, 17, SSD1306_WHITE);
  displayTela.drawPixel(73, 17, SSD1306_WHITE);
  displayTela.drawPixel(73, 16, SSD1306_WHITE);
  displayTela.drawPixel(73, 15, SSD1306_WHITE);
  displayTela.drawPixel(73, 14, SSD1306_WHITE);
  displayTela.drawPixel(72, 14, SSD1306_WHITE);
  displayTela.drawPixel(72, 13, SSD1306_WHITE);
  displayTela.drawPixel(69, 13, SSD1306_WHITE);
  displayTela.drawPixel(69, 12, SSD1306_WHITE);
  displayTela.drawPixel(66, 11, SSD1306_WHITE);
  displayTela.drawPixel(65, 11, SSD1306_WHITE);
  displayTela.drawPixel(64, 11, SSD1306_WHITE);
  displayTela.drawPixel(63, 11, SSD1306_WHITE);
  displayTela.drawPixel(62, 11, SSD1306_WHITE);
  displayTela.drawPixel(61, 11, SSD1306_WHITE);
  displayTela.drawPixel(60, 11, SSD1306_WHITE);
  displayTela.drawPixel(59, 11, SSD1306_WHITE);
  displayTela.drawPixel(58, 11, SSD1306_WHITE);
  displayTela.drawPixel(57, 11, SSD1306_WHITE);
  displayTela.drawPixel(57, 12, SSD1306_WHITE);
  displayTela.drawPixel(56, 12, SSD1306_WHITE);
  displayTela.drawPixel(55, 12, SSD1306_WHITE);
  displayTela.drawPixel(54, 12, SSD1306_WHITE);
  displayTela.drawPixel(54, 13, SSD1306_WHITE);
  displayTela.drawPixel(55, 11, SSD1306_WHITE);
  displayTela.drawPixel(56, 11, SSD1306_WHITE);
  displayTela.drawPixel(56, 10, SSD1306_WHITE);
  displayTela.drawPixel(57, 10, SSD1306_WHITE);
  displayTela.drawPixel(58, 10, SSD1306_WHITE);
  displayTela.drawPixel(58, 9, SSD1306_WHITE);
  displayTela.drawPixel(59, 9, SSD1306_WHITE);
  displayTela.drawPixel(60, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 8, SSD1306_WHITE);
  displayTela.drawPixel(62, 8, SSD1306_WHITE);
  displayTela.drawPixel(63, 8, SSD1306_WHITE);
  displayTela.drawPixel(64, 8, SSD1306_WHITE);
  displayTela.drawPixel(65, 8, SSD1306_WHITE);
  displayTela.drawPixel(66, 8, SSD1306_WHITE);
  displayTela.drawPixel(61, 7, SSD1306_WHITE);
  displayTela.drawPixel(60, 7, SSD1306_WHITE);
  displayTela.drawPixel(59, 7, SSD1306_WHITE);
  displayTela.drawPixel(58, 7, SSD1306_WHITE);
  displayTela.drawPixel(57, 7, SSD1306_WHITE);
  displayTela.drawPixel(57, 8, SSD1306_WHITE);
  displayTela.drawPixel(56, 8, SSD1306_WHITE);
  displayTela.drawPixel(56, 9, SSD1306_WHITE);
  displayTela.drawPixel(55, 9, SSD1306_WHITE);
  displayTela.drawPixel(55, 10, SSD1306_WHITE);
  displayTela.drawPixel(57, 9, SSD1306_WHITE);
  displayTela.drawPixel(58, 8, SSD1306_WHITE);
  displayTela.drawPixel(59, 8, SSD1306_WHITE);
  displayTela.drawPixel(60, 8, SSD1306_WHITE);
  displayTela.drawPixel(62, 7, SSD1306_WHITE);
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(65, 7, SSD1306_WHITE);
  displayTela.drawPixel(66, 7, SSD1306_WHITE);
  displayTela.drawPixel(62, 9, SSD1306_WHITE);
  displayTela.drawPixel(63, 9, SSD1306_WHITE);
  displayTela.drawPixel(64, 9, SSD1306_WHITE);
  displayTela.drawPixel(67, 7, SSD1306_WHITE);
  displayTela.drawPixel(68, 7, SSD1306_WHITE);
  displayTela.drawPixel(69, 7, SSD1306_WHITE);
  displayTela.drawPixel(70, 8, SSD1306_WHITE);
  displayTela.drawPixel(71, 8, SSD1306_WHITE);
  displayTela.drawPixel(72, 8, SSD1306_WHITE);
  displayTela.drawPixel(72, 9, SSD1306_WHITE);
  displayTela.drawPixel(72, 10, SSD1306_WHITE);
  displayTela.drawPixel(72, 11, SSD1306_WHITE);
  displayTela.drawPixel(73, 11, SSD1306_WHITE);
  displayTela.drawPixel(73, 12, SSD1306_WHITE);
  displayTela.drawPixel(73, 13, SSD1306_WHITE);
  displayTela.drawPixel(71, 12, SSD1306_WHITE);
  displayTela.drawPixel(71, 11, SSD1306_WHITE);
  displayTela.drawPixel(70, 11, SSD1306_WHITE);
  displayTela.drawPixel(69, 10, SSD1306_WHITE);
  displayTela.drawPixel(69, 9, SSD1306_WHITE);
  displayTela.drawPixel(68, 9, SSD1306_WHITE);
  displayTela.drawPixel(67, 9, SSD1306_WHITE);
  displayTela.drawPixel(67, 8, SSD1306_WHITE);
  displayTela.drawPixel(62, 10, SSD1306_WHITE);
  displayTela.drawPixel(63, 10, SSD1306_WHITE);
  displayTela.drawPixel(66, 10, SSD1306_WHITE);
  displayTela.drawPixel(67, 10, SSD1306_WHITE);
  displayTela.drawPixel(69, 8, SSD1306_WHITE);
  displayTela.drawPixel(70, 7, SSD1306_WHITE);
  displayTela.drawPixel(70, 9, SSD1306_WHITE);
  displayTela.drawPixel(70, 10, SSD1306_WHITE);
  displayTela.drawPixel(68, 10, SSD1306_WHITE);
  displayTela.drawPixel(66, 9, SSD1306_WHITE);
  displayTela.drawPixel(65, 10, SSD1306_WHITE);
  displayTela.drawPixel(68, 8, SSD1306_WHITE);
  displayTela.drawPixel(71, 10, SSD1306_WHITE);
  displayTela.drawPixel(71, 9, SSD1306_WHITE);
  displayTela.drawPixel(72, 15, SSD1306_WHITE);
  displayTela.drawPixel(72, 12, SSD1306_WHITE);
  displayTela.drawPixel(64, 10, SSD1306_WHITE);
  displayTela.drawPixel(65, 9, SSD1306_WHITE);
  displayTela.drawPixel(65, 12, SSD1306_WHITE);
  //id: 86 pixel 87 
  displayTela.drawPixel(58, 5, SSD1306_WHITE);
  //id: 87 pixel 88 
  displayTela.drawPixel(59, 4, SSD1306_WHITE);
  //id: 88 pixel 89 
  displayTela.drawPixel(60, 3, SSD1306_WHITE);
  displayTela.drawPixel(61, 3, SSD1306_WHITE);
  //id: 89 pixel 90 
  displayTela.drawPixel(62, 2, SSD1306_WHITE);
  displayTela.drawPixel(63, 2, SSD1306_WHITE);
  displayTela.drawPixel(64, 2, SSD1306_WHITE);
  displayTela.drawPixel(65, 2, SSD1306_WHITE);
  //id: 90 pixel 91 
  displayTela.drawPixel(66, 3, SSD1306_WHITE);
  displayTela.drawPixel(67, 3, SSD1306_WHITE);
  //id: 91 pixel 92 
  displayTela.drawPixel(68, 4, SSD1306_WHITE);
  //id: 92 pixel 93 
  displayTela.drawPixel(69, 5, SSD1306_WHITE);
  displayTela.drawPixel(70, 5, SSD1306_WHITE);
  //id: 93 pixel 94 
  displayTela.drawPixel(52, 20, SSD1306_WHITE);
  displayTela.drawPixel(53, 20, SSD1306_WHITE);
  displayTela.drawPixel(52, 21, SSD1306_WHITE);
  displayTela.drawPixel(51, 21, SSD1306_WHITE);
  displayTela.drawPixel(51, 22, SSD1306_WHITE);
  displayTela.drawPixel(50, 22, SSD1306_WHITE);
  displayTela.drawPixel(50, 23, SSD1306_WHITE);
  displayTela.drawPixel(50, 24, SSD1306_WHITE);
  displayTela.drawPixel(49, 24, SSD1306_WHITE);
  displayTela.drawPixel(49, 25, SSD1306_WHITE);
  displayTela.drawPixel(48, 25, SSD1306_WHITE);
  displayTela.drawPixel(48, 26, SSD1306_WHITE);
  displayTela.drawPixel(47, 27, SSD1306_WHITE);
  displayTela.drawPixel(47, 28, SSD1306_WHITE);
  displayTela.drawPixel(46, 29, SSD1306_WHITE);
  displayTela.drawPixel(46, 30, SSD1306_WHITE);
  displayTela.drawPixel(46, 28, SSD1306_WHITE);
  displayTela.drawPixel(46, 27, SSD1306_WHITE);
  displayTela.drawPixel(47, 26, SSD1306_WHITE);
  displayTela.drawPixel(47, 25, SSD1306_WHITE);
  displayTela.drawPixel(48, 24, SSD1306_WHITE);
  displayTela.drawPixel(49, 23, SSD1306_WHITE);
  displayTela.drawPixel(52, 19, SSD1306_WHITE);
  displayTela.drawPixel(52, 18, SSD1306_WHITE);
  displayTela.drawPixel(53, 18, SSD1306_WHITE);
  displayTela.drawPixel(53, 17, SSD1306_WHITE);
  displayTela.drawPixel(53, 16, SSD1306_WHITE);
  displayTela.drawPixel(52, 17, SSD1306_WHITE);
  displayTela.drawPixel(51, 18, SSD1306_WHITE);
  displayTela.drawPixel(51, 19, SSD1306_WHITE);
  displayTela.drawPixel(51, 20, SSD1306_WHITE);
  displayTela.drawPixel(50, 21, SSD1306_WHITE);
  displayTela.drawPixel(50, 19, SSD1306_WHITE);
  displayTela.drawPixel(50, 20, SSD1306_WHITE);
  displayTela.drawPixel(49, 22, SSD1306_WHITE);
  displayTela.drawPixel(48, 23, SSD1306_WHITE);
  displayTela.drawPixel(48, 22, SSD1306_WHITE);
  displayTela.drawPixel(49, 21, SSD1306_WHITE);
  displayTela.drawPixel(53, 19, SSD1306_WHITE);
  displayTela.drawPixel(50, 18, SSD1306_WHITE);
  displayTela.drawPixel(49, 19, SSD1306_WHITE);
  displayTela.drawPixel(49, 20, SSD1306_WHITE);
  displayTela.drawPixel(49, 26, SSD1306_WHITE);
  displayTela.drawPixel(49, 27, SSD1306_WHITE);
  displayTela.drawPixel(49, 28, SSD1306_WHITE);
  displayTela.drawPixel(50, 28, SSD1306_WHITE);
  displayTela.drawPixel(50, 29, SSD1306_WHITE);
  displayTela.drawPixel(50, 30, SSD1306_WHITE);
  displayTela.drawPixel(51, 31, SSD1306_WHITE);
  displayTela.drawPixel(52, 32, SSD1306_WHITE);
  //id: 94 pixel 95 
  displayTela.drawPixel(35, 55, SSD1306_BLACK);
  //id: 95 pixel 96 
  displayTela.drawPixel(56, 6, SSD1306_WHITE);
  displayTela.drawPixel(55, 6, SSD1306_WHITE);
  //id: 96 pixel 97 
  displayTela.drawPixel(71, 6, SSD1306_WHITE);
  displayTela.drawPixel(72, 6, SSD1306_WHITE);
  displayTela.drawPixel(73, 6, SSD1306_WHITE);
  //id: 97 pixel 98 
  displayTela.drawPixel(74, 7, SSD1306_WHITE);
  displayTela.drawPixel(75, 7, SSD1306_WHITE);
  //id: 98 pixel 99 
  displayTela.drawPixel(76, 8, SSD1306_WHITE);
  displayTela.drawPixel(77, 8, SSD1306_WHITE);
  //id: 99 pixel 100 
  displayTela.drawPixel(78, 9, SSD1306_WHITE);
  //id: 100 pixel 101 
  displayTela.drawPixel(79, 10, SSD1306_WHITE);
  displayTela.drawPixel(80, 10, SSD1306_WHITE);
  //id: 101 pixel 102 
  displayTela.drawPixel(81, 11, SSD1306_WHITE);
  displayTela.drawPixel(81, 12, SSD1306_WHITE);
  //id: 102 pixel 103 
  displayTela.drawPixel(81, 11, SSD1306_BLACK);
  displayTela.drawPixel(81, 12, SSD1306_BLACK);
  displayTela.drawPixel(80, 12, SSD1306_BLACK);
  displayTela.drawPixel(80, 11, SSD1306_BLACK);
  displayTela.drawPixel(79, 11, SSD1306_BLACK);
  displayTela.drawPixel(79, 10, SSD1306_BLACK);
  displayTela.drawPixel(80, 10, SSD1306_BLACK);
  //id: 103 line 104 
  displayTela.drawLine(79, 9, 79, 9, SSD1306_WHITE);
  //id: 104 line 105 
  displayTela.drawLine(80, 10, 80, 10, SSD1306_WHITE);
  //id: 105 line 106 
  displayTela.drawLine(81, 11, 81, 11, SSD1306_WHITE);
  //id: 106 line 107 
  displayTela.drawLine(82, 12, 82, 13, SSD1306_WHITE);
  //id: 107 line 108 
  displayTela.drawLine(54, 7, 53, 7, SSD1306_WHITE);
  //id: 108 pixel 109 
  displayTela.drawPixel(52, 8, SSD1306_WHITE);
  displayTela.drawPixel(53, 8, SSD1306_WHITE);
  displayTela.drawPixel(54, 8, SSD1306_WHITE);
  displayTela.drawPixel(55, 8, SSD1306_WHITE);
  displayTela.drawPixel(55, 7, SSD1306_WHITE);
  displayTela.drawPixel(56, 7, SSD1306_WHITE);
  displayTela.drawPixel(57, 7, SSD1306_WHITE);
  displayTela.drawPixel(58, 7, SSD1306_WHITE);
  displayTela.drawPixel(59, 7, SSD1306_WHITE);
  displayTela.drawPixel(60, 7, SSD1306_WHITE);
  displayTela.drawPixel(61, 7, SSD1306_WHITE);
  displayTela.drawPixel(62, 7, SSD1306_WHITE);
  displayTela.drawPixel(63, 7, SSD1306_WHITE);
  displayTela.drawPixel(64, 7, SSD1306_WHITE);
  displayTela.drawPixel(66, 7, SSD1306_WHITE);
  displayTela.drawPixel(67, 7, SSD1306_WHITE);
  displayTela.drawPixel(68, 7, SSD1306_WHITE);
  displayTela.drawPixel(69, 7, SSD1306_WHITE);
  displayTela.drawPixel(70, 7, SSD1306_WHITE);
  displayTela.drawPixel(71, 7, SSD1306_WHITE);
  displayTela.drawPixel(71, 6, SSD1306_WHITE);
  displayTela.drawPixel(72, 6, SSD1306_WHITE);
  displayTela.drawPixel(72, 7, SSD1306_WHITE);
  displayTela.drawPixel(73, 7, SSD1306_WHITE);
  displayTela.drawPixel(73, 8, SSD1306_WHITE);
  displayTela.drawPixel(74, 8, SSD1306_WHITE);
  displayTela.drawPixel(73, 9, SSD1306_WHITE);
  displayTela.drawPixel(74, 9, SSD1306_WHITE);
  displayTela.drawPixel(75, 9, SSD1306_WHITE);
  displayTela.drawPixel(75, 8, SSD1306_WHITE);
  displayTela.drawPixel(74, 10, SSD1306_WHITE);
  displayTela.drawPixel(74, 11, SSD1306_WHITE);
  displayTela.drawPixel(75, 11, SSD1306_WHITE);
  displayTela.drawPixel(75, 10, SSD1306_WHITE);
  displayTela.drawPixel(76, 10, SSD1306_WHITE);
  displayTela.drawPixel(76, 9, SSD1306_WHITE);
  displayTela.drawPixel(75, 12, SSD1306_WHITE);
  displayTela.drawPixel(75, 13, SSD1306_WHITE);
  displayTela.drawPixel(76, 13, SSD1306_WHITE);
  displayTela.drawPixel(77, 13, SSD1306_WHITE);
  displayTela.drawPixel(77, 12, SSD1306_WHITE);
  displayTela.drawPixel(77, 11, SSD1306_WHITE);
  displayTela.drawPixel(74, 12, SSD1306_WHITE);
  displayTela.drawPixel(74, 13, SSD1306_WHITE);
  displayTela.drawPixel(75, 14, SSD1306_WHITE);
  displayTela.drawPixel(76, 14, SSD1306_WHITE);
  displayTela.drawPixel(77, 14, SSD1306_WHITE);
  displayTela.drawPixel(78, 14, SSD1306_WHITE);
  displayTela.drawPixel(79, 14, SSD1306_WHITE);
  displayTela.drawPixel(79, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 12, SSD1306_WHITE);
  displayTela.drawPixel(78, 12, SSD1306_WHITE);
  displayTela.drawPixel(76, 12, SSD1306_WHITE);
  displayTela.drawPixel(75, 15, SSD1306_WHITE);
  displayTela.drawPixel(76, 15, SSD1306_WHITE);
  displayTela.drawPixel(77, 15, SSD1306_WHITE);
  displayTela.drawPixel(76, 11, SSD1306_WHITE);
  //id: 109 pixel 110 
  displayTela.drawPixel(53, 9, SSD1306_WHITE);
  displayTela.drawPixel(52, 9, SSD1306_WHITE);
  displayTela.drawPixel(51, 9, SSD1306_WHITE);
  displayTela.drawPixel(50, 9, SSD1306_WHITE);
  //id: 110 pixel 111 
  displayTela.drawPixel(51, 8, SSD1306_WHITE);
  //id: 111 pixel 112 
  displayTela.drawPixel(49, 9, SSD1306_WHITE);
  //id: 112 pixel 113 
  displayTela.drawPixel(76, 9, SSD1306_WHITE);
  displayTela.drawPixel(77, 9, SSD1306_WHITE);
  displayTela.drawPixel(77, 10, SSD1306_WHITE);
  displayTela.drawPixel(77, 11, SSD1306_WHITE);
  displayTela.drawPixel(78, 11, SSD1306_WHITE);
  displayTela.drawPixel(78, 10, SSD1306_WHITE);
  displayTela.drawPixel(79, 11, SSD1306_WHITE);
  displayTela.drawPixel(79, 10, SSD1306_WHITE);
  displayTela.drawPixel(80, 11, SSD1306_WHITE);
  displayTela.drawPixel(80, 12, SSD1306_WHITE);
  displayTela.drawPixel(80, 13, SSD1306_WHITE);
  displayTela.drawPixel(79, 13, SSD1306_WHITE);
  displayTela.drawPixel(78, 13, SSD1306_WHITE);
  displayTela.drawPixel(80, 14, SSD1306_WHITE);
  displayTela.drawPixel(80, 15, SSD1306_WHITE);
  displayTela.drawPixel(80, 16, SSD1306_WHITE);
  displayTela.drawPixel(79, 16, SSD1306_WHITE);
  displayTela.drawPixel(78, 16, SSD1306_WHITE);
  displayTela.drawPixel(77, 16, SSD1306_WHITE);
  displayTela.drawPixel(76, 17, SSD1306_WHITE);
  displayTela.drawPixel(75, 17, SSD1306_WHITE);
  displayTela.drawPixel(75, 18, SSD1306_WHITE);
  displayTela.drawPixel(76, 18, SSD1306_WHITE);
  displayTela.drawPixel(77, 17, SSD1306_WHITE);
  displayTela.drawPixel(78, 15, SSD1306_WHITE);
  displayTela.drawPixel(77, 15, SSD1306_WHITE);
  displayTela.drawPixel(76, 16, SSD1306_WHITE);
  displayTela.drawPixel(75, 16, SSD1306_WHITE);
  displayTela.drawPixel(74, 17, SSD1306_WHITE);
  displayTela.drawPixel(74, 18, SSD1306_WHITE);
  displayTela.drawPixel(78, 14, SSD1306_WHITE);
  displayTela.drawPixel(77, 14, SSD1306_WHITE);
  displayTela.drawPixel(76, 15, SSD1306_WHITE);
  displayTela.drawPixel(75, 19, SSD1306_WHITE);
  displayTela.drawPixel(76, 19, SSD1306_WHITE);
  displayTela.drawPixel(77, 18, SSD1306_WHITE);
  displayTela.drawPixel(77, 19, SSD1306_WHITE);
  displayTela.drawPixel(78, 18, SSD1306_WHITE);
  displayTela.drawPixel(78, 17, SSD1306_WHITE);
  displayTela.drawPixel(79, 15, SSD1306_WHITE);
  displayTela.drawPixel(79, 14, SSD1306_WHITE);
  displayTela.drawPixel(78, 19, SSD1306_WHITE);
  displayTela.drawPixel(78, 20, SSD1306_WHITE);
  displayTela.drawPixel(78, 21, SSD1306_WHITE);
  displayTela.drawPixel(78, 22, SSD1306_WHITE);
  displayTela.drawPixel(78, 23, SSD1306_WHITE);
  displayTela.drawPixel(78, 24, SSD1306_WHITE);
  displayTela.drawPixel(77, 24, SSD1306_WHITE);
  displayTela.drawPixel(77, 23, SSD1306_WHITE);
  displayTela.drawPixel(77, 22, SSD1306_WHITE);
  displayTela.drawPixel(77, 21, SSD1306_WHITE);
  displayTela.drawPixel(77, 20, SSD1306_WHITE);
  displayTela.drawPixel(79, 17, SSD1306_WHITE);
  displayTela.drawPixel(79, 18, SSD1306_WHITE);
  displayTela.drawPixel(79, 19, SSD1306_WHITE);
  displayTela.drawPixel(79, 20, SSD1306_WHITE);
  displayTela.drawPixel(79, 22, SSD1306_WHITE);
  displayTela.drawPixel(80, 23, SSD1306_WHITE);
  displayTela.drawPixel(80, 25, SSD1306_WHITE);
  displayTela.drawPixel(80, 26, SSD1306_WHITE);
  displayTela.drawPixel(80, 24, SSD1306_WHITE);
  displayTela.drawPixel(80, 22, SSD1306_WHITE);
  displayTela.drawPixel(80, 21, SSD1306_WHITE);
  displayTela.drawPixel(80, 20, SSD1306_WHITE);
  displayTela.drawPixel(80, 19, SSD1306_WHITE);
  displayTela.drawPixel(80, 17, SSD1306_WHITE);
  displayTela.drawPixel(79, 21, SSD1306_WHITE);
  displayTela.drawPixel(79, 23, SSD1306_WHITE);
  displayTela.drawPixel(79, 24, SSD1306_WHITE);
  displayTela.drawPixel(78, 25, SSD1306_WHITE);
  displayTela.drawPixel(78, 26, SSD1306_WHITE);
  displayTela.drawPixel(78, 27, SSD1306_WHITE);
  displayTela.drawPixel(79, 25, SSD1306_WHITE);
  displayTela.drawPixel(79, 26, SSD1306_WHITE);
  displayTela.drawPixel(80, 18, SSD1306_WHITE);
  displayTela.drawPixel(73, 19, SSD1306_WHITE);
  displayTela.drawPixel(72, 19, SSD1306_WHITE);
  displayTela.drawPixel(71, 19, SSD1306_WHITE);
  displayTela.drawPixel(69, 19, SSD1306_WHITE);
  displayTela.drawPixel(68, 19, SSD1306_WHITE);
  displayTela.drawPixel(66, 19, SSD1306_WHITE);
  displayTela.drawPixel(64, 19, SSD1306_WHITE);
  displayTela.drawPixel(63, 19, SSD1306_WHITE);
  displayTela.drawPixel(61, 18, SSD1306_WHITE);
  displayTela.drawPixel(59, 18, SSD1306_WHITE);
  displayTela.drawPixel(58, 18, SSD1306_WHITE);
  displayTela.drawPixel(57, 17, SSD1306_WHITE);
  displayTela.drawPixel(56, 16, SSD1306_WHITE);
  displayTela.drawPixel(56, 15, SSD1306_WHITE);
  displayTela.drawPixel(56, 14, SSD1306_WHITE);
  displayTela.drawPixel(57, 14, SSD1306_WHITE);
  displayTela.drawPixel(58, 14, SSD1306_WHITE);
  displayTela.drawPixel(58, 13, SSD1306_WHITE);
  //id: 113 pixel 114 
  displayTela.drawPixel(48, 10, SSD1306_WHITE);
  //id: 114 pixel 115 
  displayTela.drawPixel(47, 10, SSD1306_WHITE);
  //id: 115 pixel 116 
  displayTela.drawPixel(47, 11, SSD1306_WHITE);
  //id: 116 pixel 117 
  displayTela.drawPixel(47, 10, SSD1306_WHITE);
  //id: 117 pixel 118 
  displayTela.drawPixel(47, 10, SSD1306_BLACK);
  //id: 118 pixel 119 
  displayTela.drawPixel(46, 12, SSD1306_WHITE);
  displayTela.drawPixel(46, 13, SSD1306_WHITE);
  displayTela.drawPixel(46, 14, SSD1306_WHITE);
  displayTela.drawPixel(46, 15, SSD1306_WHITE);
  displayTela.drawPixel(46, 16, SSD1306_WHITE);
  displayTela.drawPixel(46, 17, SSD1306_WHITE);
  displayTela.drawPixel(46, 18, SSD1306_WHITE);
  displayTela.drawPixel(46, 19, SSD1306_WHITE);
  displayTela.drawPixel(46, 20, SSD1306_WHITE);
  displayTela.drawPixel(46, 21, SSD1306_WHITE);
  displayTela.drawPixel(46, 22, SSD1306_WHITE);
  displayTela.drawPixel(46, 23, SSD1306_WHITE);
  displayTela.drawPixel(46, 24, SSD1306_WHITE);
  displayTela.drawPixel(46, 25, SSD1306_WHITE);
  displayTela.drawPixel(46, 26, SSD1306_WHITE);
  displayTela.drawPixel(46, 27, SSD1306_WHITE);
  displayTela.drawPixel(46, 28, SSD1306_WHITE);
  displayTela.drawPixel(46, 29, SSD1306_WHITE);
  //id: 119 pixel 120 
  displayTela.drawPixel(82, 13, SSD1306_WHITE);
  displayTela.drawPixel(82, 14, SSD1306_WHITE);
  displayTela.drawPixel(82, 15, SSD1306_WHITE);
  displayTela.drawPixel(82, 16, SSD1306_WHITE);
  displayTela.drawPixel(82, 17, SSD1306_WHITE);
  displayTela.drawPixel(82, 18, SSD1306_WHITE);
  displayTela.drawPixel(82, 19, SSD1306_WHITE);
  displayTela.drawPixel(82, 20, SSD1306_WHITE);
  displayTela.drawPixel(82, 21, SSD1306_WHITE);
  displayTela.drawPixel(82, 22, SSD1306_WHITE);
  displayTela.drawPixel(82, 23, SSD1306_WHITE);
  displayTela.drawPixel(82, 24, SSD1306_WHITE);
  displayTela.drawPixel(82, 25, SSD1306_WHITE);
  displayTela.drawPixel(82, 26, SSD1306_WHITE);
  displayTela.drawPixel(82, 27, SSD1306_WHITE);
  displayTela.drawPixel(82, 28, SSD1306_WHITE);
  displayTela.drawPixel(82, 29, SSD1306_WHITE);
  displayTela.drawPixel(82, 30, SSD1306_WHITE);
  displayTela.drawPixel(82, 31, SSD1306_WHITE);
  displayTela.drawPixel(81, 29, SSD1306_WHITE);
  displayTela.drawPixel(81, 28, SSD1306_WHITE);
  displayTela.drawPixel(81, 27, SSD1306_WHITE);
  displayTela.drawPixel(81, 26, SSD1306_WHITE);
  displayTela.drawPixel(81, 25, SSD1306_WHITE);
  displayTela.drawPixel(81, 24, SSD1306_WHITE);
  displayTela.drawPixel(81, 23, SSD1306_WHITE);
  displayTela.drawPixel(81, 22, SSD1306_WHITE);
  displayTela.drawPixel(81, 21, SSD1306_WHITE);
  displayTela.drawPixel(81, 20, SSD1306_WHITE);
  displayTela.drawPixel(81, 19, SSD1306_WHITE);
  displayTela.drawPixel(81, 18, SSD1306_WHITE);
  displayTela.drawPixel(81, 17, SSD1306_WHITE);
  displayTela.drawPixel(81, 16, SSD1306_WHITE);
  displayTela.drawPixel(81, 15, SSD1306_WHITE);
  displayTela.drawPixel(81, 14, SSD1306_WHITE);
  displayTela.drawPixel(81, 13, SSD1306_WHITE);
  displayTela.drawPixel(81, 12, SSD1306_WHITE);
  displayTela.drawPixel(80, 12, SSD1306_WHITE);
  displayTela.drawPixel(79, 12, SSD1306_WHITE);
  displayTela.drawPixel(79, 11, SSD1306_WHITE);
  displayTela.drawPixel(78, 11, SSD1306_WHITE);
  displayTela.drawPixel(77, 11, SSD1306_WHITE);
  displayTela.drawPixel(76, 11, SSD1306_WHITE);
  displayTela.drawPixel(75, 11, SSD1306_WHITE);
  displayTela.drawPixel(74, 11, SSD1306_WHITE);
  displayTela.drawPixel(73, 10, SSD1306_WHITE);
  displayTela.drawPixel(72, 10, SSD1306_WHITE);
  displayTela.drawPixel(71, 10, SSD1306_WHITE);
  displayTela.drawPixel(70, 10, SSD1306_WHITE);
  displayTela.drawPixel(69, 10, SSD1306_WHITE);
  displayTela.drawPixel(68, 9, SSD1306_WHITE);
  displayTela.drawPixel(67, 9, SSD1306_WHITE);
  displayTela.drawPixel(66, 9, SSD1306_WHITE);
  displayTela.drawPixel(65, 9, SSD1306_WHITE);
  displayTela.drawPixel(64, 9, SSD1306_WHITE);
  displayTela.drawPixel(63, 9, SSD1306_WHITE);
  displayTela.drawPixel(62, 9, SSD1306_WHITE);
  displayTela.drawPixel(61, 9, SSD1306_WHITE);
  displayTela.drawPixel(60, 9, SSD1306_WHITE);
  displayTela.drawPixel(59, 9, SSD1306_WHITE);
  displayTela.drawPixel(58, 10, SSD1306_WHITE);
  displayTela.drawPixel(57, 10, SSD1306_WHITE);
  displayTela.drawPixel(56, 10, SSD1306_WHITE);
  displayTela.drawPixel(55, 10, SSD1306_WHITE);
  displayTela.drawPixel(54, 10, SSD1306_WHITE);
  displayTela.drawPixel(53, 10, SSD1306_WHITE);
  displayTela.drawPixel(52, 10, SSD1306_WHITE);
  displayTela.drawPixel(52, 11, SSD1306_WHITE);
  displayTela.drawPixel(51, 11, SSD1306_WHITE);
  displayTela.drawPixel(50, 11, SSD1306_WHITE);
  displayTela.drawPixel(50, 10, SSD1306_WHITE);
  displayTela.drawPixel(51, 10, SSD1306_WHITE);
  displayTela.drawPixel(49, 10, SSD1306_WHITE);
  displayTela.drawPixel(48, 10, SSD1306_WHITE);
  displayTela.drawPixel(48, 11, SSD1306_WHITE);
  displayTela.drawPixel(49, 11, SSD1306_WHITE);
  displayTela.drawPixel(53, 11, SSD1306_WHITE);
  displayTela.drawPixel(50, 12, SSD1306_WHITE);
  displayTela.drawPixel(50, 13, SSD1306_WHITE);
  displayTela.drawPixel(49, 14, SSD1306_WHITE);
  displayTela.drawPixel(49, 15, SSD1306_WHITE);
  displayTela.drawPixel(49, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 14, SSD1306_WHITE);
  displayTela.drawPixel(49, 12, SSD1306_WHITE);
  displayTela.drawPixel(49, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 13, SSD1306_WHITE);
  displayTela.drawPixel(48, 14, SSD1306_WHITE);
  displayTela.drawPixel(48, 15, SSD1306_WHITE);
  displayTela.drawPixel(51, 14, SSD1306_WHITE);
  displayTela.drawPixel(51, 13, SSD1306_WHITE);
  displayTela.drawPixel(52, 13, SSD1306_WHITE);
  displayTela.drawPixel(50, 16, SSD1306_WHITE);
  displayTela.drawPixel(51, 16, SSD1306_WHITE);
  displayTela.drawPixel(51, 15, SSD1306_WHITE);
  displayTela.drawPixel(51, 12, SSD1306_WHITE);
  displayTela.drawPixel(48, 12, SSD1306_WHITE);
  displayTela.drawPixel(47, 12, SSD1306_WHITE);
  displayTela.drawPixel(47, 13, SSD1306_WHITE);
  displayTela.drawPixel(47, 14, SSD1306_WHITE);
  displayTela.drawPixel(47, 15, SSD1306_WHITE);
  displayTela.drawPixel(47, 16, SSD1306_WHITE);
  displayTela.drawPixel(47, 17, SSD1306_WHITE);
  displayTela.drawPixel(46, 18, SSD1306_WHITE);
  displayTela.drawPixel(46, 19, SSD1306_WHITE);
  displayTela.drawPixel(46, 20, SSD1306_WHITE);
  displayTela.drawPixel(46, 21, SSD1306_WHITE);
  displayTela.drawPixel(46, 22, SSD1306_WHITE);
  displayTela.drawPixel(46, 23, SSD1306_WHITE);
  displayTela.drawPixel(46, 24, SSD1306_WHITE);
  displayTela.drawPixel(46, 25, SSD1306_WHITE);
  displayTela.drawPixel(46, 17, SSD1306_WHITE);
  displayTela.drawPixel(46, 16, SSD1306_WHITE);
  displayTela.drawPixel(46, 15, SSD1306_WHITE);
  displayTela.drawPixel(46, 14, SSD1306_WHITE);
  displayTela.drawPixel(46, 26, SSD1306_WHITE);
  displayTela.drawPixel(47, 25, SSD1306_WHITE);
  displayTela.drawPixel(47, 24, SSD1306_WHITE);
  displayTela.drawPixel(47, 23, SSD1306_WHITE);
  displayTela.drawPixel(47, 22, SSD1306_WHITE);
  displayTela.drawPixel(47, 21, SSD1306_WHITE);
  displayTela.drawPixel(47, 20, SSD1306_WHITE);
  displayTela.drawPixel(47, 19, SSD1306_WHITE);
  displayTela.drawPixel(47, 18, SSD1306_WHITE);
  displayTela.drawPixel(48, 17, SSD1306_WHITE);
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  displayTela.drawPixel(52, 14, SSD1306_WHITE);
  displayTela.drawPixel(52, 15, SSD1306_WHITE);
  displayTela.drawPixel(52, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 15, SSD1306_WHITE);
  displayTela.drawPixel(49, 17, SSD1306_WHITE);
  displayTela.drawPixel(49, 18, SSD1306_WHITE);
  displayTela.drawPixel(49, 19, SSD1306_WHITE);
  displayTela.drawPixel(49, 20, SSD1306_WHITE);
  displayTela.drawPixel(49, 21, SSD1306_WHITE);
  displayTela.drawPixel(49, 22, SSD1306_WHITE);
  displayTela.drawPixel(49, 23, SSD1306_WHITE);
  displayTela.drawPixel(48, 23, SSD1306_WHITE);
  displayTela.drawPixel(48, 22, SSD1306_WHITE);
  displayTela.drawPixel(48, 21, SSD1306_WHITE);
  displayTela.drawPixel(48, 20, SSD1306_WHITE);
  displayTela.drawPixel(48, 19, SSD1306_WHITE);
  displayTela.drawPixel(48, 18, SSD1306_WHITE);
  displayTela.drawPixel(50, 17, SSD1306_WHITE);
  displayTela.drawPixel(52, 12, SSD1306_WHITE);
  displayTela.drawPixel(51, 17, SSD1306_WHITE);
  //id: 120 pixel 121 
  displayTela.drawPixel(45, 26, SSD1306_WHITE);
  displayTela.drawPixel(45, 27, SSD1306_WHITE);
  displayTela.drawPixel(45, 28, SSD1306_WHITE);
  //id: 121 pixel 122 
  displayTela.drawPixel(83, 26, SSD1306_WHITE);
  displayTela.drawPixel(83, 27, SSD1306_WHITE);
  displayTela.drawPixel(83, 28, SSD1306_WHITE);
  displayTela.drawPixel(83, 29, SSD1306_WHITE);
  displayTela.drawPixel(83, 30, SSD1306_WHITE);
  //id: 122 pixel 123 
  displayTela.drawPixel(91, 52, SSD1306_BLACK);
  displayTela.drawPixel(91, 53, SSD1306_BLACK);
  displayTela.drawPixel(91, 54, SSD1306_BLACK);
  displayTela.drawPixel(91, 55, SSD1306_BLACK);
  displayTela.drawPixel(91, 56, SSD1306_BLACK);
  displayTela.drawPixel(92, 56, SSD1306_BLACK);
  //id: 123 pixel 124 
  displayTela.drawPixel(92, 55, SSD1306_BLACK);
  //id: 124 pixel 125 
  displayTela.drawPixel(45, 16, SSD1306_WHITE);
  displayTela.drawPixel(45, 17, SSD1306_WHITE);
  displayTela.drawPixel(45, 18, SSD1306_WHITE);
  displayTela.drawPixel(45, 19, SSD1306_WHITE);
  displayTela.drawPixel(45, 20, SSD1306_WHITE);
  displayTela.drawPixel(45, 21, SSD1306_WHITE);
  displayTela.drawPixel(45, 22, SSD1306_WHITE);
  displayTela.drawPixel(45, 23, SSD1306_WHITE);
  displayTela.drawPixel(45, 24, SSD1306_WHITE);
  displayTela.drawPixel(45, 25, SSD1306_WHITE);
  displayTela.drawPixel(45, 26, SSD1306_WHITE);
  displayTela.drawPixel(45, 27, SSD1306_WHITE);
  displayTela.drawPixel(46, 27, SSD1306_WHITE);
  //id: 125 pixel 126 
  displayTela.drawPixel(48, 16, SSD1306_WHITE);
  displayTela.drawPixel(49, 16, SSD1306_WHITE);
  displayTela.drawPixel(50, 16, SSD1306_WHITE);
  displayTela.drawPixel(51, 16, SSD1306_WHITE);
  displayTela.drawPixel(52, 16, SSD1306_WHITE);
  displayTela.drawPixel(53, 16, SSD1306_WHITE);
  displayTela.drawPixel(54, 16, SSD1306_WHITE);
  displayTela.drawPixel(55, 16, SSD1306_WHITE);
  displayTela.drawPixel(56, 16, SSD1306_WHITE);
  displayTela.drawPixel(57, 16, SSD1306_WHITE);
  displayTela.drawPixel(58, 16, SSD1306_WHITE);
  displayTela.drawPixel(59, 16, SSD1306_WHITE);
  displayTela.drawPixel(60, 16, SSD1306_WHITE);
  displayTela.drawPixel(62, 16, SSD1306_WHITE);
  displayTela.drawPixel(64, 16, SSD1306_WHITE);
  displayTela.drawPixel(65, 16, SSD1306_WHITE);
  displayTela.drawPixel(66, 16, SSD1306_WHITE);
  displayTela.drawPixel(68, 16, SSD1306_WHITE);
  displayTela.drawPixel(69, 16, SSD1306_WHITE);
  displayTela.drawPixel(70, 16, SSD1306_WHITE);
  displayTela.drawPixel(72, 16, SSD1306_WHITE);
  displayTela.drawPixel(73, 16, SSD1306_WHITE);
  displayTela.drawPixel(75, 16, SSD1306_WHITE);
  displayTela.drawPixel(76, 16, SSD1306_WHITE);
  displayTela.drawPixel(77, 16, SSD1306_WHITE);
  displayTela.drawPixel(80, 16, SSD1306_WHITE);
  displayTela.drawPixel(81, 16, SSD1306_WHITE);
  displayTela.drawPixel(82, 16, SSD1306_WHITE);
  displayTela.drawPixel(83, 16, SSD1306_WHITE);
  //id: 126 pixel 127 
  displayTela.drawPixel(83, 16, SSD1306_WHITE);
  displayTela.drawPixel(83, 17, SSD1306_WHITE);
  displayTela.drawPixel(83, 18, SSD1306_WHITE);
  displayTela.drawPixel(83, 19, SSD1306_WHITE);
  displayTela.drawPixel(83, 20, SSD1306_WHITE);
  displayTela.drawPixel(83, 21, SSD1306_WHITE);
  displayTela.drawPixel(83, 22, SSD1306_WHITE);
  displayTela.drawPixel(83, 23, SSD1306_WHITE);
  displayTela.drawPixel(83, 24, SSD1306_WHITE);
  displayTela.drawPixel(83, 25, SSD1306_WHITE);
  displayTela.drawPixel(83, 26, SSD1306_WHITE);
  displayTela.drawPixel(83, 27, SSD1306_WHITE);
  displayTela.drawPixel(83, 28, SSD1306_WHITE);
  //id: 127 pixel 128 
  displayTela.drawPixel(120, 25, SSD1306_WHITE);
  //id: 128 pixel 129 
  displayTela.drawPixel(119, 25, SSD1306_BLACK);
  //id: 129 pixel 130 
  displayTela.drawPixel(119, 25, SSD1306_BLACK);
  displayTela.drawPixel(120, 25, SSD1306_BLACK);
  //id: 130 pixel 131 
  displayTela.drawPixel(84, 21, SSD1306_WHITE);
  displayTela.drawPixel(84, 22, SSD1306_WHITE);
  displayTela.drawPixel(84, 23, SSD1306_WHITE);
  displayTela.drawPixel(84, 24, SSD1306_WHITE);
  displayTela.drawPixel(84, 25, SSD1306_WHITE);
  displayTela.drawPixel(84, 26, SSD1306_WHITE);
  displayTela.drawPixel(84, 27, SSD1306_WHITE);
  displayTela.drawPixel(84, 28, SSD1306_WHITE);
  displayTela.drawPixel(84, 29, SSD1306_WHITE);
  displayTela.drawPixel(84, 30, SSD1306_WHITE);
  displayTela.drawPixel(84, 31, SSD1306_WHITE);
  displayTela.drawPixel(84, 32, SSD1306_WHITE);
  displayTela.drawPixel(84, 33, SSD1306_WHITE);
  //id: 131 pixel 132 
  displayTela.drawPixel(44, 21, SSD1306_WHITE);
  displayTela.drawPixel(44, 22, SSD1306_WHITE);
  displayTela.drawPixel(44, 23, SSD1306_WHITE);
  displayTela.drawPixel(44, 24, SSD1306_WHITE);
  displayTela.drawPixel(44, 25, SSD1306_WHITE);
  displayTela.drawPixel(43, 25, SSD1306_WHITE);
  displayTela.drawPixel(43, 26, SSD1306_WHITE);
  displayTela.drawPixel(43, 27, SSD1306_WHITE);
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  displayTela.drawPixel(43, 29, SSD1306_WHITE);
  displayTela.drawPixel(44, 29, SSD1306_WHITE);
  displayTela.drawPixel(44, 28, SSD1306_WHITE);
  displayTela.drawPixel(44, 27, SSD1306_WHITE);
  displayTela.drawPixel(44, 26, SSD1306_WHITE);
  displayTela.drawPixel(44, 30, SSD1306_WHITE);
  displayTela.drawPixel(44, 31, SSD1306_WHITE);
  //id: 132 pixel 133 
  displayTela.drawPixel(43, 28, SSD1306_WHITE);
  displayTela.drawPixel(43, 29, SSD1306_WHITE);
  displayTela.drawPixel(43, 30, SSD1306_WHITE);
  displayTela.drawPixel(43, 31, SSD1306_WHITE);
  displayTela.drawPixel(43, 32, SSD1306_WHITE);
  //id: 133 pixel 134 
  displayTela.drawPixel(85, 26, SSD1306_WHITE);
  displayTela.drawPixel(85, 27, SSD1306_WHITE);
  displayTela.drawPixel(85, 28, SSD1306_WHITE);
  displayTela.drawPixel(85, 29, SSD1306_WHITE);
  displayTela.drawPixel(85, 30, SSD1306_WHITE);
  displayTela.drawPixel(86, 31, SSD1306_WHITE);
  displayTela.drawPixel(86, 32, SSD1306_WHITE);
  displayTela.drawPixel(86, 33, SSD1306_WHITE);
  displayTela.drawPixel(86, 34, SSD1306_WHITE);
  displayTela.drawPixel(86, 35, SSD1306_WHITE);
  displayTela.drawPixel(86, 36, SSD1306_WHITE);
  displayTela.drawPixel(86, 37, SSD1306_WHITE);
  //id: 134 pixel 135 
  displayTela.drawPixel(85, 31, SSD1306_WHITE);
  displayTela.drawPixel(85, 32, SSD1306_WHITE);
  displayTela.drawPixel(85, 33, SSD1306_WHITE);
  displayTela.drawPixel(85, 34, SSD1306_WHITE);
  displayTela.drawPixel(85, 35, SSD1306_WHITE);
  displayTela.drawPixel(86, 35, SSD1306_WHITE);
  displayTela.drawPixel(86, 37, SSD1306_WHITE);
  displayTela.drawPixel(86, 38, SSD1306_WHITE);
  displayTela.drawPixel(86, 39, SSD1306_WHITE);
  displayTela.drawPixel(86, 40, SSD1306_WHITE);
  //id: 135 pixel 136 
  displayTela.drawPixel(87, 35, SSD1306_BLACK);
  //id: 136 pixel 137 
  displayTela.drawPixel(43, 25, SSD1306_BLACK);
  //id: 137 pixel 138 
  displayTela.drawPixel(42, 31, SSD1306_WHITE);
  //id: 138 pixel 139 
  displayTela.drawPixel(41, 33, SSD1306_BLACK);
  //id: 139 pixel 140 
  displayTela.drawPixel(41, 34, SSD1306_BLACK);
  //id: 140 pixel 141 
  displayTela.drawPixel(41, 35, SSD1306_BLACK);
  //id: 141 pixel 142 
  displayTela.drawPixel(40, 35, SSD1306_BLACK);
  //id: 142 pixel 143 
  displayTela.drawPixel(39, 36, SSD1306_BLACK);
  //id: 143 pixel 144 
  displayTela.drawPixel(39, 37, SSD1306_BLACK);
  displayTela.drawPixel(39, 38, SSD1306_BLACK);
  //id: 144 pixel 145 
  displayTela.drawPixel(40, 36, SSD1306_BLACK);
  //id: 145 pixel 146 
  displayTela.drawPixel(39, 39, SSD1306_BLACK);
  //id: 146 pixel 147 
  displayTela.drawPixel(38, 40, SSD1306_BLACK);
  displayTela.drawPixel(38, 41, SSD1306_BLACK);
  displayTela.drawPixel(38, 42, SSD1306_BLACK);
  displayTela.drawPixel(38, 43, SSD1306_BLACK);
  displayTela.drawPixel(38, 44, SSD1306_BLACK);
  //id: 147 pixel 148 
  displayTela.drawPixel(35, 50, SSD1306_BLACK);
  displayTela.drawPixel(35, 51, SSD1306_BLACK);
  displayTela.drawPixel(36, 51, SSD1306_BLACK);
  displayTela.drawPixel(36, 52, SSD1306_BLACK);
  displayTela.drawPixel(36, 53, SSD1306_BLACK);
  displayTela.drawPixel(36, 54, SSD1306_BLACK);
  displayTela.drawPixel(36, 55, SSD1306_BLACK);
  displayTela.drawPixel(36, 56, SSD1306_BLACK);
  displayTela.drawPixel(36, 57, SSD1306_BLACK);
  displayTela.drawPixel(35, 57, SSD1306_BLACK);
  //id: 148 pixel 149 
  displayTela.drawPixel(38, 45, SSD1306_BLACK);
  displayTela.drawPixel(38, 46, SSD1306_BLACK);
  //id: 149 pixel 150 
  displayTela.drawPixel(37, 46, SSD1306_BLACK);
  displayTela.drawPixel(37, 47, SSD1306_BLACK);
  displayTela.drawPixel(37, 48, SSD1306_BLACK);
  displayTela.drawPixel(37, 49, SSD1306_BLACK);
  displayTela.drawPixel(36, 49, SSD1306_BLACK);
  displayTela.drawPixel(36, 50, SSD1306_BLACK);
  displayTela.drawPixel(36, 51, SSD1306_BLACK);
  displayTela.drawPixel(36, 52, SSD1306_BLACK);
  displayTela.drawPixel(36, 53, SSD1306_BLACK);
  displayTela.drawPixel(36, 54, SSD1306_BLACK);
  displayTela.drawPixel(36, 55, SSD1306_BLACK);
  displayTela.drawPixel(37, 55, SSD1306_BLACK);
  //id: 150 pixel 151 
  displayTela.drawPixel(37, 54, SSD1306_BLACK);
  displayTela.drawPixel(37, 53, SSD1306_BLACK);
  displayTela.drawPixel(37, 52, SSD1306_BLACK);
  displayTela.drawPixel(37, 51, SSD1306_BLACK);
  displayTela.drawPixel(37, 50, SSD1306_BLACK);
  displayTela.drawPixel(37, 49, SSD1306_BLACK);
  //id: 151 fillRect 152 
  displayTela.fillRect(98, 16, 3, 3, SSD1306_WHITE);
  //id: 152 fillRect 153 
  displayTela.fillRect(101, 19, 3, 15, SSD1306_WHITE);
  //id: 153 fillRect 154 
  displayTela.fillRect(98, 34, 3, 3, SSD1306_WHITE);
  //id: 154 fillRect 155 
  displayTela.fillRect(104, 13, 3, 3, SSD1306_WHITE);
  //id: 155 fillRect 156 
  displayTela.fillRect(104, 37, 3, 3, SSD1306_WHITE);
  //id: 156 fillRect 157 
  displayTela.fillRect(101, 40, 3, 3, SSD1306_WHITE);
  //id: 157 fillRect 158 
  displayTela.fillRect(107, 16, 3, 21, SSD1306_WHITE);
  //id: 158 fillRect 159 
  displayTela.fillRect(101, 10, 3, 3, SSD1306_WHITE);
  //id: 159 fillRect 160 
  displayTela.fillRect(28, 34, 3, 3, SSD1306_WHITE);
  //id: 160 fillRect 161 
  displayTela.fillRect(28, 16, 3, 3, SSD1306_WHITE);
  //id: 161 fillRect 162 
  displayTela.fillRect(25, 19, 3, 15, SSD1306_WHITE);
  //id: 162 fillRect 163 
  displayTela.fillRect(22, 13, 3, 3, SSD1306_WHITE);
  //id: 163 fillRect 164 
  displayTela.fillRect(22, 37, 3, 3, SSD1306_WHITE);
  //id: 164 fillRect 165 
  displayTela.fillRect(25, 40, 3, 3, SSD1306_WHITE);
  //id: 165 fillRect 166 
  displayTela.fillRect(25, 10, 3, 3, SSD1306_WHITE);
  //id: 166 fillRect 167 
  displayTela.fillRect(19, 16, 3, 21, SSD1306_WHITE);



}
