#include "sensores/dht22/DhtService.h"

void iniciarDHT() {
    dht.begin();
    logPrintln("[DHT] Sensor inicializado.");
}

bool lerDadosDHT(float &temp, float &humi) {
    uint64_t t0 = esp_timer_get_time();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    tempoPegarDHTUS = (uint32_t)(esp_timer_get_time() - t0);

    if (isnan(h) || isnan(t)) {
        logWarn("[DHT] Erro na leitura do sensor!\n");
        return false;
    }

    temp = t;
    humi = h;
    ultimoDhtLeitura = millis();
    return true;
}
