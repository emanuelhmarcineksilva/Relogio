#include "ChronosObsidian.h"

void iniciarI2S() {
  // Configura o periférico I2S para mandar áudio ao MAX98357A
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  cfg.sample_rate = SAMPLE_RATE;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;  // MAX98357A é mono
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  cfg.dma_buf_count = 8; // dma_buf_count = 8 é o número de buffers de áudio que vão ser enviados para o MAX98357A (a dma faz o envio)
  cfg.dma_buf_len = 64; // dma_buf_len = 64 é o tamanho do buffer de áudio que vai ser enviado para o MAX98357A (a dma faz o envio)
  cfg.use_apll = false;

  i2s_pin_config_t pins = {};
  pins.bck_io_num   = I2S_BCLK;
  pins.ws_io_num    = I2S_LRC;
  pins.data_out_num = I2S_DIN;
  pins.data_in_num  = I2S_PIN_NO_CHANGE;

  esp_err_t err = i2s_driver_install(I2S_PORT, &cfg, 0, NULL);
  if (err == ESP_OK) {
    i2s_set_pin(I2S_PORT, &pins);
    i2s_zero_dma_buffer(I2S_PORT);
    i2sIniciado = true;
    Serial.println("I2S OK (MAX98357A)");
  } else {
    Serial.println("I2S FALHOU!");
  }
}

// Toca um pedaço da melodia atual (não-bloqueante)
// Chamada a cada iteração do loop — gera ~128 samples (~5.8ms de áudio)
void tocarMelodiaStep() {
  // Se o driver de audio ainda nao iniciou, nao existe para onde enviar som.
  if (!i2sIniciado) return;

  // Quando nenhum alarme esta tocando, garantimos silencio no I2S.
  // Tambem resetamos os indices para a proxima execucao comecar do inicio.
  if (!alarmeDisparo || melodiaAtualIdx < 0 || melodiaAtualIdx >= TOTAL_MELODIAS) {
    if (notaAtualIdx != 0 || faseOnda != 0) {
      i2s_zero_dma_buffer(I2S_PORT);
      notaAtualIdx = 0;
      faseOnda = 0;
    }
    return;
  }

  // Le a nota atual da melodia armazenada na flash (PROGMEM).
  const Nota* mel = MELODIAS[melodiaAtualIdx];
  Nota nota;
  memcpy_P(&nota, &mel[notaAtualIdx], sizeof(Nota));

  // Se passou o tempo da nota atual, avanca para a proxima nota.
  if (millis() - notaInicioMs >= nota.durMs) {
    notaAtualIdx++;

    // Se encontrou marcador de fim ({0,0}), reinicia a melodia em loop.
    Nota proxima;
    memcpy_P(&proxima, &mel[notaAtualIdx], sizeof(Nota));
    if (proxima.freq == 0 && proxima.durMs == 0) {
      notaAtualIdx = 0;
    }

    // Salva novo instante de inicio da nota e zera a fase da senoide.
    // Zerando a fase evitamos acumulo numerico durante muito tempo de execucao.
    notaInicioMs = millis();
    memcpy_P(&nota, &mel[notaAtualIdx], sizeof(Nota));
    faseOnda = 0; // Reseta fase ao mudar de nota
  }

  // Gera 128 amostras PCM (16 bits) para este passo do audio.
  // Em 22050Hz, isso representa ~5.8ms de áudio.
  int16_t samples[128];

  if (nota.freq > 0) {
    float incremento = 2.0f * PI * nota.freq / (float)SAMPLE_RATE;
    for (int i = 0; i < 128; i++) {
      samples[i] = (int16_t)(sinf(faseOnda) * 8000); 
      faseOnda += incremento;
      if (faseOnda >= 2.0f * PI) faseOnda -= 2.0f * PI;
    }
  } else {
    memset(samples, 0, sizeof(samples));
  }

  // Envia o bloco para o periférico I2S com timeout longo (100ms).
  // Isso permite que a função bloqueie exatamente o tempo necessário para o hardware,
  // mantendo a reprodução em velocidade real (100%).
  size_t written;
  i2s_write(I2S_PORT, samples, sizeof(samples), &written, 100 / portTICK_PERIOD_MS);
}
