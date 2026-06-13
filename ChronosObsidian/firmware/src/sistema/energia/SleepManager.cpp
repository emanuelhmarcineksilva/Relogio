#include "ChronosObsidian.h"

void enterLightSleep(uint32_t duration_ms) {
  if (sleep_mode_active) return;  // Ja esta dormindo
  
  logInfo("[SLEEP] Entrando em Light Sleep por %lu ms\n", duration_ms);
  
  // Configure wake sources
  esp_sleep_enable_timer_wakeup(duration_ms * 1000);  // Timer in microseconds
  esp_sleep_enable_ext0_wakeup((gpio_num_t)pinoButton1, 0);       // Button 1 (GPIO 4, LOW)
  esp_sleep_enable_ext0_wakeup((gpio_num_t)pinoButton2, 0);       // Button 2 (GPIO 18, LOW)
  
  // Enter Light Sleep
  sleep_mode_active = true;
  esp_light_sleep_start();
  sleep_mode_active = false;
  
  logPrintln("[SLEEP] Woke from Light Sleep");
}
