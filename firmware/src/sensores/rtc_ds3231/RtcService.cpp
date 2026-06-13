#include "sensores/rtc_ds3231/RtcService.h"

bool iniciarRTC() {
    if (!rtc.begin()) {
        logError("[RTC] Nao encontrado!\n");
        return false;
    }
    
    if (rtc.lostPower()) {
        logWarn("[RTC] Perdeu energia! Ajustando para data de compilacao...\n");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    logPrintln("[RTC] Inicializado com sucesso.");
    return true;
}

DateTime obterHoraAtual() {
    uint64_t t0 = esp_timer_get_time();
    DateTime agora = rtc.now();
    tempoRtcNowUs = (uint32_t)(esp_timer_get_time() - t0);
    return agora;
}

void ajustarHoraSistema(int h, int m) {
    DateTime agora = rtc.now();
    rtc.adjust(DateTime(agora.year(), agora.month(), agora.day(), h, m, 0));
    logPrintf("[RTC] Hora ajustada manualmente: %02d:%02d\n", h, m);
}

void sincronizarRTC(struct tm ti) {
    rtc.adjust(DateTime(ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec));
    logPrintln("[RTC] Sincronizado com NTP.");
}
