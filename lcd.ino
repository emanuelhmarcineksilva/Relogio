#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Wire.begin(21, 22);   // SDA=21, SCL=22 no ESP32

  lcd.begin();          // Se der erro, troque por lcd.init();

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("TESTE LCD");

  lcd.setCursor(0, 1);
  lcd.print("ESP32 OK");
}

void loop() {
}