#include "ChronosObsidian.h"

// ============================================================================
// TAREFA: TaskAudio (Core 1)
// ============================================================================
// Responsabilidade: Reprodução de melodias de forma não-bloqueante
// Comunicação: Lê estado global (melodiaAtualIdx, notaAtualIdx, alarmeDisparo)
// Executada em Core 1 com prioridade MÉDIA para não bloquear UI
// ============================================================================

void TaskAudio(void *pvParameters) {
  logPrintln("[TASK] TaskAudio iniciada (Core 1)");
  
  while (1) {
    // Executa reprodução de áudio a cada TASK_AUDIO_INTERVAL ms (~50ms)
    // Isso permite tocar notas individuais da melodia de forma suave
    
    uint64_t tAudio0 = esp_timer_get_time();
    
    // Verifica se há alarme disparado
    if (alarmeDisparo && melodiaAtualIdx >= 0 && melodiaAtualIdx < TOTAL_MELODIAS) {
      // Chama a função de reprodução passo-a-passo
      tocarMelodiaStep();
    }
    
    // [OPCIONAL] Log periódico de estado de áudio (comentado para não sobrecarregar logs)
    // if (alarmeDisparo) {
    //   logInfo("[TASK-AUDIO] Melodia %d: Nota %d\n", melodiaAtualIdx, notaAtualIdx);
    // }
    
    // Atraso mínimo para evitar timeout do watchdog (cedência de 50ms)
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ============================================================================
// ESTRUTURA DE DADOS: EstruturaAudio (para expansão futura)
// ============================================================================
// Pode ser usada para controlar volume, efeitos de fade, etc.
struct EstruturaAudio {
  uint8_t volume;          // 0-100%
  bool mute;               // True = mudo
  uint32_t tempoNotas;     // Tempo total tocado (ms)
  uint16_t notasProcessadas; // Contador de notas tocadas
};

// [TODO] Implementar suporte a:
// - Controle de volume dinâmico
// - Fade in/out
// - Fila de melodias (tocar várias em sequência)
// - Efeitos sonoros (beep curto, etc.)
