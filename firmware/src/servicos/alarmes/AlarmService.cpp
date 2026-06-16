#include "servicos/alarmes/AlarmService.h"

void inicializarAlarmes() {
    carregarAlarmes();
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
            
            // Marca TODOS os alarmes deste mesmo horario como tocados
            // para evitar que um segundo alarme dispare logo após silenciar o primeiro
            for (int j = 0; j < NUM_ALARMES; j++) {
                if (alarmes[j].hora == hora && alarmes[j].minuto == minuto) {
                    alarmes[j].jaTocou = true;
                }
            }
            
            // Inicia exibição da mensagem de clima
            gerarMsgClima();
            mostrarMsgClima = true;
            inicioMsgClima = millis();
            
            logPrintf("*** ALARME %d DISPAROU (Mel%d)! ***\n", i + 1, melodiaAtualIdx + 1);
        }
    }

    tempoLogicaAlarmeUs = (uint32_t)(esp_timer_get_time() - t0);
}

void salvarAlarmes() {
    File f = SPIFFS.open("/alarmes.txt", FILE_WRITE);
    if (!f) {
        logError("[ALARMES] Erro ao abrir arquivo para salvar!\n");
        return;
    }
    for (int i = 0; i < NUM_ALARMES; i++) {
        f.printf("%d,%d,%d,%d\n", alarmes[i].hora, alarmes[i].minuto, alarmes[i].ativo ? 1 : 0, alarmes[i].melodia);
    }
    f.close();
    logPrintln("[ALARMES] Configurações salvas na flash.");
}

void carregarAlarmes() {
    if (!SPIFFS.exists("/alarmes.txt")) {
        logWarn("[ALARMES] Arquivo de alarmes nao existe. Usando padroes.\n");
        return;
    }
    File f = SPIFFS.open("/alarmes.txt", FILE_READ);
    if (!f) {
        logError("[ALARMES] Erro ao abrir arquivo para carregar!\n");
        return;
    }
    int i = 0;
    while (f.available() && i < NUM_ALARMES) {
        String linha = f.readStringUntil('\n');
        int h, m, a, mel;
        if (sscanf(linha.c_str(), "%d,%d,%d,%d", &h, &m, &a, &mel) == 4) {
            alarmes[i].hora = h;
            alarmes[i].minuto = m;
            alarmes[i].ativo = (a != 0);
            alarmes[i].melodia = mel;
            alarmes[i].jaTocou = false;
        }
        i++;
    }
    f.close();
    logPrintf("[ALARMES] %d alarmes carregados da flash.\n", i);
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
        salvarAlarmes();
        logPrintf("[ALARMES] Alarme %d configurado: %02d:%02d\n", idx + 1, h, m);
    }
}
