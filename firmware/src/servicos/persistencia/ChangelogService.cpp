#include "servicos/persistencia/ChangelogService.h"
#include "ChronosObsidian.h"

void ChangelogService::init() {
    if (!SPIFFS.exists("/changelog.txt")) {
        File f = SPIFFS.open("/changelog.txt", FILE_WRITE);
        if (f) {
            f.println("v4.0.0 | 2026-06-13 | Integracao RTOS Dual-Core, Modularizacao de Servicos e Persistencia CRC32.");
            f.println("v3.1.0 | 2026-05-23 | Dashboard Web com abas, Log Ring Buffer e Historico de Performance.");
            f.println("v2.0.0 | 2026-04-21 | Integracao RTC, OLED e Sensor DHT22.");
            f.close();
            logPrintln("[CHANGELOG] Arquivo inicial criado.");
        }
    }
}

String ChangelogService::getChangelog() {
    if (!SPIFFS.exists("/changelog.txt")) {
        return "Nenhum changelog encontrado.";
    }
    File f = SPIFFS.open("/changelog.txt", FILE_READ);
    if (!f) return "Erro ao abrir changelog.";
    String content = f.readString();
    f.close();
    return content;
}

void ChangelogService::addEntry(const String& versao, const String& data, const String& mudanca) {
    File f = SPIFFS.open("/changelog.txt", FILE_APPEND);
    if (f) {
        f.printf("%s | %s | %s\n", versao.c_str(), data.c_str(), mudanca.c_str());
        f.close();
        logPrintf("[CHANGELOG] Nova entrada: %s\n", versao.c_str());
    }
}
