#include "servicos/alarmes/AlarmService.h"

void inicializarAlarmes() {
    logPrintln("[ALARMES] Sistema inicializado.");
}

void processarAlarmes(int hora, int minuto) {
    uint64_t t0 = esp_timer_get_time();

    for (int i = 0; i < NUM_ALARMES; i++) {
        // Reset da trava quando o minuto do alarme passa
        if (minuto != alarmes[i].minuto) {
            alarmes[i].jaTocou = false;
        }

        // Disparo: só no modo relógio, se ativo e hora/minuto batem
        if (estadoAtual == "RELOGIO" && alarmes[i].ativo &&
            hora == alarmes[i].hora && minuto == alarmes[i].minuto &&
            !alarmes[i].jaTocou && !alarmeDisparo) {
            
            alarmeDisparo = true;
            alarmeDisparoIdx = i;
            melodiaAtualIdx = alarmes[i].melodia;
            notaAtualIdx = 0;
            notaInicioMs = millis();
            faseOnda = 0;
            
            // Inicia exibição da mensagem de clima
            gerarMsgClima();
            mostrarMsgClima = true;
            inicioMsgClima = millis();
            
            logPrintf("*** ALARME %d DISPAROU! ***\n", i + 1);
        }
    }

    tempoLogicaAlarmeUs = (uint32_t)(esp_timer_get_time() - t0);
}

void silenciarAlarmeAtual() {
    if (alarmeDisparo) {
        alarmeDisparo = false;
        if (alarmeDisparoIdx >= 0) {
            alarmes[alarmeDisparoIdx].jaTocou = true;
        }
        logPrintln("[ALARME] Alarme silenciado pelo usuario.");
    }
}

void configurarAlarme(int idx, uint8_t h, uint8_t m, bool ativo, uint8_t melodia) {
    if (idx >= 0 && idx < NUM_ALARMES) {
        alarmes[idx].hora = h;
        alarmes[idx].minuto = m;
        alarmes[idx].ativo = ativo;
        alarmes[idx].melodia = melodia;
        alarmes[idx].jaTocou = false;
        logPrintf("[ALARMES] Alarme %d configurado: %02d:%02d\n", idx + 1, h, m);
    }
}
