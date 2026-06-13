#include "ChronosObsidian.h"

void oledMsg(const char* l1, const char* l2) {
  if (!oledOkBoot) return; // Se o OLED não estiver inicializado, não faz nada
  displayTela.clearDisplay(); // Limpa o buffer da tela
  displayTela.setTextColor(SSD1306_WHITE); // Define a cor do texto

  // Linha 1: Titulo em negrito (tamanho 1 = 6x8 pixels por char)
  displayTela.setTextSize(1);
  displayTela.setCursor(0, 0);
  displayTela.print(l1);

  // Separador horizontal
  displayTela.drawLine(0, 11, SCREEN_WIDTH - 1, 11, SSD1306_WHITE);

  // Linha 2: Status (se fornecido)
  if (l2 != nullptr) {
    displayTela.setCursor(0, 16);
    displayTela.setTextSize(1);
    displayTela.print(l2);
  }

  displayTela.display();
}
