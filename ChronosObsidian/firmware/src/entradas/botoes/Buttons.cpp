#include "ChronosObsidian.h"

static void btn1_debounce_callback(void* arg) {
  // Lê o pino novamente para confirmar o pressionamento (software debounce)
  if (digitalRead(pinoButton1) == LOW) {
    // Pressionamento confirmado: cria evento
    ButtonEvent event = {
      .button_id = 1,
      .timestamp_ms = millis(),
      .debounce_state = 1  // Confirmado
    };
    // Enfileira evento (seguro a partir do contexto ISR)
    xQueueSendFromISR(buttonEventQueue, &event, NULL);
    logInfo("[BTN-ISR] Botão 1 confirmado\n");
  }
  btn1_pending = false;
}

// ============================================================================
// CALLBACK ISR: Timer de debounce do Botão 2 (dispara após 20ms)
// ============================================================================
static void btn2_debounce_callback(void* arg) {
  if (digitalRead(pinoButton2) == LOW) {
    ButtonEvent event = {
      .button_id = 2,
      .timestamp_ms = millis(),
      .debounce_state = 1
    };
    xQueueSendFromISR(buttonEventQueue, &event, NULL);
    logInfo("[BTN-ISR] Botão 2 confirmado\n");
  }
  btn2_pending = false;
}

// ============================================================================
// ISR GPIO: Detecção de pressionamento do Botão 1 (borda HIGH→LOW)
// ============================================================================
// Chamado imediatamente pela interrupção GPIO (manipulador rápido: < 1µs)
// Inicia o timer de debounce em vez de bloquear com delay()
void IRAM_ATTR btn1_isr() {
  // Apenas enfileira novo debounce se não houver um pendente
  if (!btn1_pending) {
    btn1_pending = true;
    btn1_bounce_time = millis();
    // Inicia timer de debounce de 20ms (chama btn1_debounce_callback após o atraso)
    esp_timer_start_once(btn1_debounce_timer, 20000);  // 20ms em microssegundos
  }
}

// ============================================================================
// ISR GPIO: Detecção de pressionamento do Botão 2 (borda HIGH→LOW)
// ============================================================================
void IRAM_ATTR btn2_isr() {
  if (!btn2_pending) {
    btn2_pending = true;
    btn2_bounce_time = millis();
    esp_timer_start_once(btn2_debounce_timer, 20000);
  }
}
