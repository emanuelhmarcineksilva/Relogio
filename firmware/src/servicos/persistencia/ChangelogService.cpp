#include "servicos/persistencia/ChangelogService.h"
#include "ChronosObsidian.h"

void ChangelogService::init() {
    // FORÇA atualização: deleta arquivo antigo para forçar recriação com nova versão
    if (SPIFFS.exists("/changelog.txt")) {
        SPIFFS.remove("/changelog.txt");
        logInfo("[CHANGELOG] Arquivo antigo removido para atualização\n");
    }
    
    // Cria novo arquivo com histórico completo
    File f = SPIFFS.open("/changelog.txt", FILE_WRITE);
    if (f) {
            f.println("=== CHRONOS OBSIDIAN - CHANGELOG ===\n");
            
            f.println("v4.1.0 | 2026-06-15");
            f.println("  [ENERGIA] Sistema de Idle Timeout (50s) com controle de Light Sleep");
            f.println("    - Timer de 50s resetável por cliques de botão");
            f.println("    - Previne Light Sleep enquanto há atividade");
            f.println("    - Reduz consumo sem perder responsividade");
            f.println("  [CORRECAO] Ajuste no versionamento de dependências (Adafruit GFX ^1.11.0)");
            f.println();
            
            f.println("v4.0.0 | 2026-06-13");
            f.println("  [ARQUITETURA] Integração RTOS Dual-Core (Core 0: Sensores, Core 1: UI/Audio)");
            f.println("    - TaskAquisicao: leitura de RTC, DHT22, clima (30Hz)");
            f.println("    - TaskUI: atualização de displays (20Hz)");
            f.println("    - TaskAudio: reprodução de alarmes (on-demand)");
            f.println("    - TaskPersistencia: backup de logs/config (5min)");
            f.println("  [MODULARIZACAO] Divisão em serviços (AlarmService, WeatherService, etc)");
            f.println("  [PERSISTENCIA] Backup CRC32 e restauração automática de configurações");
            f.println("  [ENERGIA] Preparação para Light Sleep (RTC_DATA_ATTR)");
            f.println();
            
            f.println("v3.1.0 | 2026-05-23");
            f.println("  [WEB] Dashboard com 4 abas (Logs, Serial, Flash, Gráficos)");
            f.println("    - Visualização em tempo real de métricas de performance");
            f.println("    - Gráficos de CPU, Heap, Stack por tarefa");
            f.println("    - Histórico de performance até 60 amostras");
            f.println("  [OBSERVABILIDADE] Log Ring Buffer (8KB, ~1000 linhas)");
            f.println("  [PERFORMANCE] Métricas por função (us) + stack/heap por task");
            f.println();
            
            f.println("v3.0.0 | 2026-05-10");
            f.println("  [INTERFACE] Redesign OLED com ícones de clima");
            f.println("    - Modo Normal: Relógio + Temperatura + Clima");
            f.println("    - Modo Menu: Navegação com Botão1, Confirmação com Botão2");
            f.println("    - Modo Alarme: Tela de disparo com opções Adiar/Desativar");
            f.println("  [CLIMA] Integração Meteosource API (preview curitiba)");
            f.println("  [CONFIG] Menu de ajuste Hora/Minuto com debounce via timer");
            f.println();
            
            f.println("v2.0.0 | 2026-04-21");
            f.println("  [SENSORES] Integração RTC DS3231 + DHT22");
            f.println("    - Sincronização com NTP ao boot");
            f.println("    - Leitura de temperatura/umidade a cada 5s");
            f.println("  [DISPLAY] OLED SSD1306 (128x64, I2C)");
            f.println("  [AUDIO] Síntese I2S com 5 melodias pré-programadas");
            f.println("  [ALARMES] Sistema de 3 alarmes com hora/minuto configurável");
            f.println();
            
            f.println("v1.0.0 | 2026-03-15");
            f.println("  [INICIAL] Prototipo funcional com ESP32 + WiFi");
            f.println("    - Servidor Web na porta 80");
            f.println("    - Modo Access Point para configuração");
            f.println("    - Leitura de hora do sistema");
            f.println();
            
            f.close();
            logPrintln("[CHANGELOG] Arquivo criado/atualizado com histórico completo.");
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
