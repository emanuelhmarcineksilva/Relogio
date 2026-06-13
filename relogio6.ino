// RELOGIO INTELIGENTE ESP32
// Versao do firmware (exibida na aba Sobre)
#define FIRMWARE_VERSION "4.0.0"

// ============================================================================
// FASE 2.5: GCC OPTIMIZATION PRAGMAS
// ============================================================================
// Sinalizações de compilação para ajuste de desempenho:
//   -O0: Sem otimização (depuração, mais lento)
//   -O1: Otimização básica (~10% mais rápido)
//   -O2: Equilibrado (padrão, ~30% mais rápido, seguro, bom para embarcados)
//   -O3: Agressivo (~40% mais rápido, pode causar bugs sutis, use com cuidado)
//   -Os: Otimização de tamanho (para dispositivos com ROM limitada)
//
// Padrão do ESP32: -O2 (bom equilíbrio)
// Este pragma define -O2 como base (já definido em platform.txt)
// Pode ser sobrescrito por seção:
//   #pragma GCC optimize("O3")  → seção rápida (matemática pesada, compatível com ISR)
//   #pragma GCC optimize("O0")  → seção de depuração (crítica para segurança)
//
// Estratégia de teste:
//   1. Executar com -O2 (padrão): medir a base
//   2. Perfil de pontos quentes com esp_timer
//   3. Aplicar -O3 aos pontos quentes, testar a correção
//   4. Medir a melhoria
//
// NOTA: -O3 pode causar:
//   - Maior tempo de compilação
//   - Binário maior (excesso de código)
//   - Otimizações inseguras em alguns casos de borda
//   - Use apenas em código comprovado e testado

#pragma GCC optimize("O2")  // Baseline: balanced optimization

// ============================================================================
// PARTE 1: BIBLIOTECAS
// ============================================================================
// "#include" importa ferramentas prontas que outras pessoas escreveram.
// Sem elas precisariamos escrever centenas de linhas extras.

#include <WiFi.h>              // Conecta o ESP32 ao WiFi
#include <WebServer.h>         // Cria um servidor web dentro do ESP32
#include <HTTPClient.h>        // Faz pedidos HTTP (para buscar o clima)
#include <ArduinoJson.h>       // Le/escreve JSON (formato padrao da web)
#include <Wire.h>              // Protocolo I2C (para OLED e RTC conversarem)
#include <Adafruit_SSD1306.h>  // Controla o display OLED
#include <Adafruit_GFX.h>      // Desenha texto e formas no display
#include <RTClib.h>            // Le e ajusta o relogio de tempo real DS3231
#include <esp_heap_caps.h>     // Informa quanto de memoria RAM esta livre
#include <esp_timer.h>         // Timer de alta precisao (em microsegundos)
#include <driver/i2s.h>        // Protocolo I2S para enviar audio digital
#include <math.h>              // Funcoes matematicas (seno para gerar som)
#include <SPIFFS.h>            // Salva arquivos na memoria flash do ESP32
#include "time.h"              // Sincroniza hora com servidores NTP (internet)
#include <DHT.h>               // Biblioteca da temperatura (DHT22)
#include <LiquidCrystal_I2C.h> // Controla o display LCD 16x2 via I2C

// ============================================================================
// PARTE 2: CONFIGURACOES DO HARDWARE E TIPOS
// ============================================================================

// Razões de despertar (por que o dispositivo acordou)
// Deve ser definido antes do uso em funções
typedef enum {
  WAKE_POWER_ON,
  WAKE_TIMER,
  WAKE_GPIO,
  WAKE_UNKNOWN
} WakeReason;
// "#define" cria um apelido para um numero. Em vez de escrever 128
// espalhado pelo codigo, usamos SCREEN_WIDTH. Se precisar mudar, muda aqui.

// --- Display OLED ---
#define SCREEN_WIDTH  128  // Largura do display em pixels
#define SCREEN_HEIGHT  64  // Altura do display em pixels
#define OLED_ADDR    0x3C  // Endereco I2C do display (padrao do SSD1306)
#define SDA_PIN        21  // Pino de dados I2C
#define SCL_PIN        22  // Pino de clock I2C

// --- Audio I2S (amplificador MAX98357A) ---
#define I2S_BCLK       26     // Bit Clock: cadencia dos bits de audio
#define I2S_LRC        25     // Left/Right Clock: sincroniza os canais
#define I2S_DIN        27     // Dados de audio (saida do ESP32)
#define I2S_PORT   I2S_NUM_0  // Porta I2S do ESP32 (tem 2, usamos a 0)
#define SAMPLE_RATE 22050     // Amostras de audio por segundo (Hz)

// --- Objetos de hardware ---
// Cada objeto representa um componente fisico e permite controla-lo.
RTC_DS3231 rtc;                                                // Relogio de tempo real
Adafruit_SSD1306 displayTela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // Display OLED
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD 16x2 no endereco I2C 0x27 (SDA=21, SCL=22)
WebServer server(80);  // Servidor web na porta 80 (porta padrao HTTP)

// ============================================================================
// PARTE 3: CREDENCIAIS E CONFIGURACOES
// ============================================================================
// !! CREDENCIAIS PADRAO do WiFi !!
// Estas sao usadas SOMENTE se nao houver configuracao salva na flash.
// Depois de configurar pela web, o ESP32 lembra a rede automaticamente.
const char* WIFI_SSID_DEFAULT     = "Marcos";    // Nome padrao da rede
const char* WIFI_PASSWORD_DEFAULT = "henrique";   // Senha padrao da rede

// Variaveis que guardam o WiFi ATUAL (podem ser mudadas pela web)
// "char[]" = array de caracteres, como uma caixinha de texto.
// 64 caracteres e mais que suficiente para qualquer nome/senha de rede.
char wifiSSID[64]     = "Marcos";
char wifiPassword[64] = "henrique";

// Flag que indica se estamos no modo Access Point ("roteador proprio")
// Quando o ESP32 nao consegue conectar no WiFi configurado, ele
// cria sua propria rede chamada "RelogioConfig" para voce acessar
// a pagina web e configurar um novo WiFi.
bool modoAP = false;

// ============================================================================
// CONFIGURAÇÃO DA API METEOSOURCE (FASE 1.1)
// ============================================================================
// Meteosource: O plano gratuito permite ~400 requisições/dia (~96 usadas em intervalos de 15min)
// Documentação: https://www.meteosource.com/api/
// ID do Local: "curitiba" - fixo para este projeto (pode ser tornado configurável depois)
const char* METEOSOURCE_API_KEY = "fju60uilyihby9vsdjbwffyb7pr46vq9pfauc9d3";
const char* METEOSOURCE_PLACE_ID = "curitiba";
const char* METEOSOURCE_URL = "https://www.meteosource.com/api/v1/free/point"
                               "?place_id=curitiba"
                               "&sections=daily"
                               "&timezone=auto"
                               "&language=en"
                               "&units=metric"
                               "&key=fju60uilyihby9vsdjbwffyb7pr46vq9pfauc9d3";

// Estrutura de dados do tempo: guarda a previsão diária do Meteosource
// Atualizada a cada 10 minutos (configurável via cfgClimaIntervaloMs)
struct WeatherData {
  int weather_code;           // Código do tempo Meteosource (0-65)
  float temp_current;         // Temperatura atual (°C)
  float temp_min;             // Mínima diária (°C)
  float temp_max;             // Máxima diária (°C)
  bool valid;                 // Flag de validade dos dados (true = buscado recentemente)
  unsigned long last_fetch;   // Carimbo de tempo da última busca bem-sucedida (ms)
} climaAtual = {0, 0.0, 0.0, 0.0, false, 0};

// Variáveis de temperatura (mantidas para compatibilidade reversa com o código existente)
float temperatura     = 0.0;  // Temperatura da API (previsão)
float dhtTemperature  = NAN;  // Medição local do DHT22
float dhtHumidity     = NAN;  // Umidade local do DHT22
unsigned long ultimoDhtLeitura = 0;  // Carimbo de tempo da última leitura do DHT

// Inicialização do clima e rastreamento da força do sinal WiFi
bool  climaIniciado = false;  // Verdadeiro após a primeira busca de clima
int32_t wifiRSSI    = 0;      // Força do sinal WiFi em dBm


// === PARAMETROS CONFIGURAVEIS PELA WEB ===
// Estes valores podem ser alterados pela aba "Config" da interface web.
// Isso permite ajustar o sistema sem regravar o firmware.
unsigned long cfgDhtIntervaloMs   = 5000;    // Intervalo de leitura do DHT (ms)
unsigned long cfgClimaIntervaloMs = 600000;  // Intervalo de busca do clima (ms) = 10min
unsigned long cfgBackupIntervaloMs = 7200000; // Intervalo de backup flash (ms) = 2h

// Configuracoes do Sensor DHT22
#define DHTPIN 23       // Pino de dados do DHT22 (GPIO 23)
#define DHTTYPE DHT22   // Tipo de sensor: DHT22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// ============================================================================
// PARTE 4: NOTAS MUSICAIS
// ============================================================================
// Uma nota musical e definida pela sua frequencia em Hz.
// Hz = quantas vezes o ar vibra por segundo. Mais Hz = som mais agudo.
// C4 = Do central do piano = 262 Hz
// "N_SIL" significa silencio (pausa entre notas)
#define N_C4  262  // Do
#define N_D4  294  // Re
#define N_E4  330  // Mi
#define N_F4  349  // Fa
#define N_G4  392  // Sol
#define N_A4  440  // La
#define N_B4  494  // Si
#define N_C5  523  // Do (oitava acima)
#define N_D5  587
#define N_Ds5 622  // Re sustenido
#define N_E5  659
#define N_F5  698
#define N_G5  784
#define N_A5  880
#define N_C6 1047
#define N_SIL   0  // Silencio (pausa)

// "struct" cria um tipo de dado personalizado que agrupa informacoes.
// Aqui uma "Nota" tem frequencia + duracao.
struct Nota {
  uint16_t freq;   // Frequencia em Hz (0 = silencio)
  uint16_t durMs;  // Duracao em milissegundos
};

// As melodias ficam em PROGMEM (memoria flash de 4MB) em vez da RAM (300KB).
// RAM e limitada, flash e grande. Melodias sao fixas, entao flash e ideal.
// {0,0} no final marca o fim de cada melodia.
const Nota melodia_aurora[] PROGMEM = {
  {N_C4,400},{N_SIL,50},{N_E4,400},{N_SIL,50},{N_G4,400},{N_SIL,50},
  {N_C5,500},{N_SIL,80},{N_E5,500},{N_SIL,80},{N_G5,600},{N_SIL,100},
  {N_C6,800},{N_SIL,300},
  {N_G5,300},{N_SIL,50},{N_E5,300},{N_SIL,50},{N_C5,400},{N_SIL,200},
  {0,0}  // Fim da melodia
};
const Nota melodia_sino[] PROGMEM = {
  {N_E5,500},{N_SIL,50},{N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_G4,1000},{N_SIL,200},
  {N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_E5,500},{N_SIL,50},{N_C5,1000},{N_SIL,300},
  {0,0}
};
const Nota melodia_despertar[] PROGMEM = {
  {N_A4,600},{N_SIL,100},{N_C5,600},{N_SIL,100},{N_E5,800},{N_SIL,200},
  {N_D5,600},{N_SIL,100},{N_F5,600},{N_SIL,100},{N_A5,800},{N_SIL,300},
  {N_E5,400},{N_SIL,80},{N_C5,400},{N_SIL,80},{N_A4,600},{N_SIL,400},
  {0,0}
};
const Nota melodia_furelise[] PROGMEM = {
  {N_E5,250},{N_Ds5,250},{N_E5,250},{N_Ds5,250},{N_E5,250},
  {N_B4,250},{N_D5,250},{N_C5,250},{N_A4,500},{N_SIL,100},
  {N_C4,250},{N_E4,250},{N_A4,250},{N_B4,500},{N_SIL,100},
  {N_E4,250},{N_G4,250},{N_B4,250},{N_C5,500},{N_SIL,200},
  {0,0}
};
const Nota melodia_estrela[] PROGMEM = {
  {N_C4,300},{N_C4,300},{N_G4,300},{N_G4,300},{N_A4,300},{N_A4,300},{N_G4,500},{N_SIL,100},
  {N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,300},{N_D4,300},{N_C4,500},{N_SIL,200},
  {N_G4,300},{N_G4,300},{N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,500},{N_SIL,200},
  {0,0}
};
// Lista de ponteiros para as melodias
// Ponteiro = endereco de memoria onde a melodia começa
const Nota* const MELODIAS[] = {
  melodia_aurora, melodia_sino, melodia_despertar, melodia_furelise, melodia_estrela
};
const int TOTAL_MELODIAS = 5;
const char* MELODIA_NOMES[] = {"Aurora", "Sino", "Despertar", "Fur Elise", "Estrela"};
const char* MELODIA_CURTO[] = {"Aur", "Sin", "Des", "Fur", "Est"}; // Abreviado pro OLED

// ============================================================================
// PARTE 5: ESTRUTURA DOS ALARMES
// ============================================================================
// "struct" agrupa dados relacionados num unico tipo.
// Aqui, cada alarme tem: hora, minuto, se esta ativo, qual melodia e
// se ja tocou neste minuto (para nao repetir).
struct Alarme {
  uint8_t hora;     // Hora do alarme (0 a 23)
  uint8_t minuto;   // Minuto do alarme (0 a 59)
  bool    ativo;    // true = alarme ligado | false = desligado
  uint8_t melodia;  // Indice da melodia a tocar (0 a 4)
  bool    jaTocou;  // Trava: evita tocar 2x no mesmo minuto
};

#define NUM_ALARMES 3  // Quantidade de alarmes

// Os 3 alarmes pre-configurados - modifique conforme preferir
// Formato: {hora, minuto, ativo, melodia, jaTocou}
Alarme alarmes[NUM_ALARMES] = {
  { 7,  0, true,  0, false},  // Alarme 1: 07:00 - Aurora - ATIVO
  { 7, 30, false, 1, false},  // Alarme 2: 07:30 - Sino   - desativado
  {12,  0, false, 2, false},  // Alarme 3: 12:00 - Despertar - desativado
};

// ============================================================================
// PARTE 6: SISTEMA DE LOG (diario de eventos do sistema)
// ============================================================================
// O ESP32 geralmente nao esta conectado ao computador.
// Este sistema guarda mensagens numa memoria circular (ring buffer).
// Qualquer funcao pode chamar logPrint() e a mensagem fica disponivel na web.
//
// RING BUFFER (memoria circular):
// Imagine uma fita de 4000 caracteres em circulo.
// Escrevemos na posicao atual, avancamos, e quando chegamos no fim,
// voltamos ao inicio. Assim sempre temos os 4000 chars mais RECENTES.
#define LOG_BUF_SIZE 4000      // Maximo de caracteres no log
char logBuffer[LOG_BUF_SIZE];  // A memoria circular
int  logPos     = 0;           // Posicao atual de escrita
bool logWrapped = false;       // True quando o buffer deu a volta

// Configuracao NTP (sincronizacao de hora pela internet)
const char* ntpServer        = "pool.ntp.org";
const long  gmtOffset_sec    = -3 * 3600;  // GMT-3 (Brasilia/Sao Paulo)
const int   daylightOffset_sec = 0;        // Sem horario de verao
unsigned long ultimoBackupFlash = 0;        // Quando foi o ultimo backup

// Adiciona texto ao log (salva no buffer E no monitor serial)
void logPrint(const char* msg) {
  Serial.print(msg);  // Aparece no monitor serial do Arduino IDE
  int len = strlen(msg);  // strlen conta quantos caracteres tem na mensagem
  for (int i = 0; i < len; i++) {
    logBuffer[logPos] = msg[i];  // Copia um caractere para o buffer
    logPos++;
    if (logPos >= LOG_BUF_SIZE) {  // Chegou no fim? Volta ao inicio
      logPos = 0;
      logWrapped = true;  // Marca que o buffer ja deu a volta
    }
  }
}
// Versao com quebra de linha
void logPrintln(const char* msg) { logPrint(msg); logPrint("\n"); }
// Versao formatada (igual ao printf do C)
// "..." significa que aceita qualquer quantidade de argumentos extras
void logPrintf(const char* fmt, ...) {
  char tmp[256];
  va_list args;           // Lista de argumentos variaveis
  va_start(args, fmt);    // Inicia a lista
  vsnprintf(tmp, sizeof(tmp), fmt, args);  // Monta a string formatada
  va_end(args);           // Encerra a lista
  logPrint(tmp);
}

// === LOGS COM NIVEIS ===
// Em sistemas profissionais, logs tem niveis de severidade:
//   [INFO]  = informacao normal (tudo certo)
//   [WARN]  = aviso (algo estranho mas nao critico)
//   [ERROR] = erro (algo deu errado)
// Isso facilita filtrar problemas na aba de logs da web.
void logInfo(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[INFO] "); logPrint(tmp);
}
void logWarn(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[WARN] "); logPrint(tmp);
}
void logError(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[ERROR] "); logPrint(tmp);
}

// Retorna todo o log em ordem cronologica
String getLogContent() {
  String result;
  result.reserve(LOG_BUF_SIZE);
  // Se o buffer deu a volta, os dados mais antigos estao apos a posicao atual
  if (logWrapped) {
    for (int i = logPos; i < LOG_BUF_SIZE; i++) result += logBuffer[i];
  }
  for (int i = 0; i < logPos; i++) result += logBuffer[i];
  return result;
}

// [TAG_WIFI_CONFIG]
// === WIFI PERSISTENTE ===
// Salva SSID e senha na flash do ESP32 (arquivo /wifi_config.txt).
// Assim, quando o ESP32 reiniciar, ele lembra a ultima rede configurada
// sem precisar regravar o firmware.
void salvarWifiConfig() {
  File f = SPIFFS.open("/wifi_config.txt", FILE_WRITE);
  if (!f) { logError("Falha ao salvar WiFi config!\n"); return; }
  // Formato simples: primeira linha = SSID, segunda = senha
  f.println(wifiSSID);
  f.println(wifiPassword);
  f.close();
  logInfo("WiFi config salva: %s\n", wifiSSID);
}

// Carrega SSID e senha da flash. Retorna true se encontrou.
bool carregarWifiConfig() {
  if (!SPIFFS.exists("/wifi_config.txt")) return false;
  File f = SPIFFS.open("/wifi_config.txt", FILE_READ);
  if (!f) return false;
  // Lê o SSID (primeira linha)
  String ssid = f.readStringUntil('\n');
  ssid.trim();  // Remove espacos e \r\n extras
  // Lê a senha (segunda linha)
  String pass = f.readStringUntil('\n');
  pass.trim();
  f.close();
  if (ssid.length() > 0) {
    // Copia para os arrays globais
    // strncpy = copia texto com limite de tamanho (evita estouro de memoria)
    strncpy(wifiSSID, ssid.c_str(), sizeof(wifiSSID) - 1);
    strncpy(wifiPassword, pass.c_str(), sizeof(wifiPassword) - 1);
    logInfo("WiFi config carregada: %s\n", wifiSSID);
    return true;
  }
  return false;
}

// === CONFIGURACOES PERSISTENTES ===
// Salva os parametros configuraveis (intervalos do DHT, clima, backup)
void salvarConfigParams() {
  File f = SPIFFS.open("/config_params.txt", FILE_WRITE);
  if (!f) return;
  f.printf("%lu\n%lu\n%lu\n", cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs);
  f.close();
  logInfo("Parametros salvos\n");
}

// Carrega os parametros configuraveis da flash
void carregarConfigParams() {
  if (!SPIFFS.exists("/config_params.txt")) return;
  File f = SPIFFS.open("/config_params.txt", FILE_READ);
  if (!f) return;
  cfgDhtIntervaloMs    = f.readStringUntil('\n').toInt();
  cfgClimaIntervaloMs  = f.readStringUntil('\n').toInt();
  cfgBackupIntervaloMs = f.readStringUntil('\n').toInt();
  // Protecao: minimos razoaveis para nao travar o sistema
  if (cfgDhtIntervaloMs < 2000) cfgDhtIntervaloMs = 2000;
  if (cfgClimaIntervaloMs < 60000) cfgClimaIntervaloMs = 60000;
  if (cfgBackupIntervaloMs < 300000) cfgBackupIntervaloMs = 300000;
  f.close();
  logInfo("Config carregada: DHT=%lums Clima=%lums Backup=%lums\n",
    cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs);
}

// Salva log compacto na flash (SPIFFS)
void salvarLogFlash() {
  File f = SPIFFS.open("/log_backup.txt", FILE_WRITE);
  if (!f) return;
  DateTime agora = rtc.now();
  f.printf("=== BACKUP %02d:%02d ===\n", agora.hour(), agora.minute());
  for (int i = 0; i < NUM_ALARMES; i++) {
    f.printf("A%d:%02d:%02d,%s\n", i+1, alarmes[i].hora, alarmes[i].minuto, alarmes[i].ativo?"ON":"OFF");
  }
  f.printf("T:%.1f|H:%lu|U:%lu\n", temperatura, (unsigned long)ESP.getFreeHeap(), millis()/1000UL);
  f.close();
  logPrintln("[FLASH] Backup salvo!");
}

// Carrega log da flash
String carregarLogFlash() {
  if (!SPIFFS.exists("/log_backup.txt")) return "Sem backup.";
  File f = SPIFFS.open("/log_backup.txt", FILE_READ);
  if (!f) return "Erro ao ler.";
  String content = f.readString();
  f.close();
  return content;
}

// ============================================================================
// PARTE 7: HISTORICO DE PERFORMANCE (dados para os graficos)
// ============================================================================
// O programa mede quanto tempo cada funcao demora para rodar (microsegundos).
// Esses tempos sao gravados num ring buffer.
//
// FASE 2.4: PERSISTÊNCIA EXPANDIDA DE 24 HORAS
// ============================================================================
// Anterior: 60 amostras × 5s = 5 minutos
// Novo: 288 amostras × 5min = 24 horas
//
// Tempo de amostragem: 1 amostra a cada 5 minutos (300 segundos)
// Por que 5 min? Equilíbrio entre granularidade e armazenamento (~8KB para 7 funções)
//
// 7 funções monitoradas:
//   [0] Loop completo     -> tempo total de um ciclo do loop()
//   [1] pegarClima()      -> tempo da requisição HTTP de clima
//   [2] rtc.now()         -> tempo de leitura do relogio via I2C
//   [3] Botões            -> tempo de verificar os botões
//   [4] DHT Temperatura   -> valor da temperatura (x10, em inteiro)
//   [5] Display OLED      -> tempo de redesenhar o display
//   [6] DHT Umidade       -> valor da umidade (x10, em inteiro)

#define PERF_HIST_SIZE 288           // 288 samples = 24 hours at 5min interval
#define PERF_FUNCOES 7
uint32_t perfHist[PERF_FUNCOES][PERF_HIST_SIZE];  // [funcao][amostra]
int      perfHistPos  = 0;             // proxima posicao a escrever
bool     perfHistFull = false;         // buffer ja completou a primeira volta?
unsigned long ultimaAmostraPerf = 0;
#define PERF_SAMPLE_MS 300000          // 1 amostra a cada 5 minutos (300 segundos)

// === VALIDAÇÃO CRC DO HISTÓRICO PERF (FASE 2.4) ===
// Protege contra corrupção durante a escrita/leitura no SPIFFS
// Calculado sobre todas as 288×7 amostras
uint32_t perfHistCRC = 0;

// Calculadora CRC32 simples (polinômio 0x04C11DB7)
uint32_t crc32_simple(uint32_t *data, size_t len) {
  uint32_t crc = 0xFFFFFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 32; j++) {
      if (crc & 0x80000000) {
        crc = (crc << 1) ^ 0x04C11DB7;
      } else {
        crc = crc << 1;
      }
    }
  }
  return crc ^ 0xFFFFFFFF;
}

// Salva histórico de performance na flash (binário) com CRC (FASE 2.4)
void salvarPerfFlash() {
  File f = SPIFFS.open("/perf_hist.bin", FILE_WRITE);
  if (!f) { logPrintln("[PERF] Erro ao salvar!"); return; }
  
  // Calculate CRC before saving
  perfHistCRC = crc32_simple((uint32_t*)perfHist, PERF_FUNCOES * PERF_HIST_SIZE);
  
  // Write data
  f.write((uint8_t*)perfHist, sizeof(perfHist));
  f.write((uint8_t*)&perfHistPos, sizeof(perfHistPos));
  uint8_t fb = perfHistFull ? 1 : 0;
  f.write(&fb, 1);
  f.write((uint8_t*)&perfHistCRC, sizeof(perfHistCRC));  // CRC at end
  
  f.close();
  logPrintf("[PERF] Historico salvo na flash! (CRC: 0x%08X, %d samples)\n", 
    perfHistCRC, PERF_HIST_SIZE);
}

// Carrega histórico de performance da flash com CRC validation (FASE 2.4)
void carregarPerfFlash() {
  if (!SPIFFS.exists("/perf_hist.bin")) {
    logPrintln("[PERF] Arquivo /perf_hist.bin nao encontrado, iniciando novo");
    return;
  }
  
  File f = SPIFFS.open("/perf_hist.bin", FILE_READ);
  if (!f) return;
  
  // Expected size: perfHist + perfHistPos + perfHistFull + CRC
  size_t expected = sizeof(perfHist) + sizeof(perfHistPos) + 1 + sizeof(perfHistCRC);
  
  if (f.size() == expected) {
    // Read all data
    f.read((uint8_t*)perfHist, sizeof(perfHist));
    f.read((uint8_t*)&perfHistPos, sizeof(perfHistPos));
    uint8_t fb = 0;
    f.read(&fb, 1);
    perfHistFull = (fb == 1);
    
    uint32_t savedCRC = 0;
    f.read((uint8_t*)&savedCRC, sizeof(savedCRC));
    
    // Validate CRC
    uint32_t calculatedCRC = crc32_simple((uint32_t*)perfHist, PERF_FUNCOES * PERF_HIST_SIZE);
    
    if (calculatedCRC == savedCRC) {
      // CRC matches: data is valid
      if (perfHistPos < 0 || perfHistPos >= PERF_HIST_SIZE) perfHistPos = 0;
      logPrintf("[PERF] Historico restaurado da flash (CRC valido: 0x%08X, 24h data)\n", 
        calculatedCRC);
    } else {
      // CRC mismatch: data corrupted
      logWarn("[PERF] CRC invalido! Esperava 0x%08X, obteve 0x%08X - iniciando novo\n",
        calculatedCRC, savedCRC);
      // Reset data
      memset(perfHist, 0, sizeof(perfHist));
      perfHistPos = 0;
      perfHistFull = false;
    }
  } else {
    logWarn("[PERF] Tamanho invalido: %d bytes (esperado %d)\n", f.size(), expected);
  }
  
  f.close();
}

// ============================================================================
// PARTE 8: VARIAVEIS GLOBAIS DO PROGRAMA
// ============================================================================
// Variaveis globais existem durante TODA a execucao e sao visiveis
// para todas as funcoes. Use com moderacao para nao confundir.


// --- Maquina de estados: controla qual tela mostrar no OLED ---
// Pense como um fluxograma: cada "estado" e uma caixa.
// O programa sempre esta em um estado e pode mudar para outro.
// Estados possiveis: RELOGIO, MENU, LISTA_ALARMES, DETALHE_ALARME,
//                    EDITAR_ALARME_H, EDITAR_ALARME_M, ESCOLHER_MELODIA,
//                    HORA, EDITAR_HORA_M
String estadoAtual = "RELOGIO";  // Estado inicial: mostrando o relogio

// --- Controle do menu ---
int menuIndex       = 0;   // Opcao selecionada no menu principal (0-2)
int alarmeListaIdx  = 0;   // Alarme selecionado na lista (0-2)
int subMenuIndex    = 0;   // Opcao dentro do detalhe de um alarme
int melodiaEscolha  = 0;   // Melodia sendo escolhida (temporario)
int tempHora        = 0;   // Hora temporaria durante edicao
int tempMinuto      = 0;   // Minuto temporario durante edicao

// --- Estado do alarme ---
bool alarmeDisparo    = false;  // True quando um alarme esta tocando
int  alarmeDisparoIdx = -1;     // Qual alarme disparou (-1 = nenhum)

// --- Pinos dos botoes ---
// INPUT_PULLUP: em estado solto o pino le HIGH (1).
// Quando pressionado, fecha o circuito com GND e le LOW (0).
int pinoButton1 = 4;   // Botao 1: navegar no menu
int pinoButton2 = 18;  // Botao 2: confirmar/selecionar

// FASE 2.2: INTERRUPÇÕES DE HARDWARE DOS BOTÕES + FILA DE EVENTOS
// ============================================================================
// O debounce tradicional com delay() bloqueia toda a CPU.
// Melhor abordagem: ISR → Fila → Tarefa principal processa o evento (não bloqueante)
//
// Arquitetura:
//   1. ISR GPIO (manipulador de interrupção): Breve, apenas enfileira o evento (< 1µs)
//   2. Fila de eventos: Mantém eventos de botão pendentes (fila, não sobrescreve)
//   3. Timer de debounce: Evita o repique (esp_timer, ~20-50ms)
//   4. Loop principal: Consome eventos da fila (não bloqueante)
//
// Estrutura do evento de botão
struct ButtonEvent {
  int button_id;              // 1 ou 2
  unsigned long timestamp_ms; // Quando a ISR disparou
  uint8_t debounce_state;     // 0=pendente, 1=confirmado após debounce
};

// Fila de eventos (armazena até 5 eventos de botão pendentes)
QueueHandle_t buttonEventQueue = NULL;

// Timers de debounce (um por botão, usando esp_timer)
esp_timer_handle_t btn1_debounce_timer = NULL;
esp_timer_handle_t btn2_debounce_timer = NULL;

// Estado de debounce (rastreia qual botão está em debounce no momento)
volatile bool btn1_pending = false;
volatile bool btn2_pending = false;
volatile uint32_t btn1_bounce_time = 0;
volatile uint32_t btn2_bounce_time = 0;

// ============================================================================
// CALLBACK ISR: Timer de debounce do Botão 1 (dispara após 20ms)
// ============================================================================
// Chamado pelo esp_timer (contexto ISR) após o período de debounce
// Confirma o pressionamento do botão lendo o pino novamente + enfileirando evento
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

// ============================================================================
// FASE 2.3: MODOS DE SONO (Light + Deep) COM PERSISTÊNCIA RTC_DATA_ATTR
// ============================================================================
// RTC_DATA_ATTR: Variáveis globais armazenadas na RAM do RTC (8KB)
// - Sobrevive ao Light Sleep (SRAM ligada, timer de despertar/GPIO)
// - Sobrevive ao Deep Sleep (SRAM desligada, apenas RAM do RTC ativa)
// - Usado para: contador de boot, última temp conhecida, rastreamento de tempo de despertar
//
// Modos de sono:
//   - Light Sleep: Menor potência da CPU, WiFi+SRAM ligados, despertar via timer/GPIO
//   - Deep Sleep: Potência ultrabaixa, WiFi+SRAM desligados, apenas RAM do RTC + timer RTC ativo
//   - Limiares: Tempo ocioso, bateria fraca (futuro), etc.

// Estrutura RAM do RTC (persiste através do deep sleep)
RTC_DATA_ATTR struct {
  uint32_t boot_count;           // Número de boots (incrementa a cada inicialização)
  uint32_t deep_sleep_count;     // Número de ciclos de deep sleep
  float last_temperature;        // Última temperatura conhecida (para exibição após despertar)
  float last_humidity;           // Última umidade conhecida
  uint64_t last_wake_time_us;    // Carimbo de tempo do último despertar (microssegundos)
  uint32_t deep_sleep_seconds;   // Duração do último deep sleep
  uint8_t rtc_data_version;      // Versão do esquema (para atualizações futuras)
} rtc_data = {0, 0, 0.0, 0.0, 0, 0, 1};

// Configuração de sono
#define LIGHT_SLEEP_IDLE_MS 10000    // Entra em Light Sleep após 10s ocioso
#define DEEP_SLEEP_IDLE_MS 300000    // Entra em Deep Sleep após 5min ocioso
#define DEEP_SLEEP_DURATION_SEC 60   // Desperta após 60s (1 minuto)

// Sleep mode timers
unsigned long last_activity_ms = 0;  // Timestamp of last user activity
bool sleep_mode_active = false;      // true when in Light/Deep Sleep

// ============================================================================
// Funcao: Obter razao de despertar (por que o dispositivo acordou do sono)
// ============================================================================
WakeReason getWakeReason() {
  esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
  
  switch(wakeup_cause) {
    case ESP_SLEEP_WAKEUP_TIMER:
      return WAKE_TIMER;
    case ESP_SLEEP_WAKEUP_GPIO:
      return WAKE_GPIO;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      // Verifica se é o primeiro boot (os dados do RTC podem estar limpos)
      if (rtc_data.boot_count == 0) {
        return WAKE_POWER_ON;
      }
      return WAKE_UNKNOWN;
    default:
      return WAKE_UNKNOWN;
  }
}

// ============================================================================
// Funcao: Entrar no modo Light Sleep (WiFi ligado, SRAM ligada)
// ============================================================================
// Reduz a potencia da CPU em ~50x enquanto mantém o WiFi operacional
// Desperta ao pressionar o botao (interrupcao GPIO) ou timer
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

// ============================================================================
// Funcao: Entrar no modo Deep Sleep (WiFi desligado, SRAM desligada)
// ============================================================================
// Reduz a potencia ao minimo absoluto (~10µW)
// Apenas a RAM do RTC permanece ativa (para persistencia de dados)
// Desperta apenas por timer (interrupcoes GPIO nao disponiveis no deep sleep)
void enterDeepSleep(uint32_t duration_sec) {
  if (sleep_mode_active) return;
  
  logInfo("[SLEEP] Entrando em Deep Sleep por %lu segundos\n", duration_sec);
  
  // Atualiza os dados do RTC antes de dormir
  rtc_data.deep_sleep_count++;
  rtc_data.last_temperature = isnan(dhtTemperature) ? -127.0 : dhtTemperature;
  rtc_data.last_humidity = isnan(dhtHumidity) ? -127.0 : dhtHumidity;
  rtc_data.last_wake_time_us = esp_timer_get_time();
  rtc_data.deep_sleep_seconds = duration_sec;
  
  // Desativa o WiFi para economizar energia
  WiFi.disconnect(true);  // true = desliga o rádio WiFi
  WiFi.mode(WIFI_OFF);
  
  // Configura apenas a fonte de despertar por timer
  esp_sleep_enable_timer_wakeup(duration_sec * 1000000);  // Converte para microssegundos
  
  // Desperta apenas por timer
  // Pressionamentos de botão NÃO acordarão o dispositivo; apenas o timer
  
  logPrintf("[SLEEP] Dados RTC: Boots=%lu DeepSleeps=%lu T=%.1f H=%.1f\n",
    rtc_data.boot_count, rtc_data.deep_sleep_count,
    rtc_data.last_temperature, rtc_data.last_humidity);
  
  logPrintln("[SLEEP] Deep Sleep - o dispositivo reiniciará pelo timer!");
  delay(100);  // Allow serial to flush
  
  // Enter Deep Sleep (will restart on timer, not return here)
  esp_deep_sleep_start();
}

// --- Tempos de execucao das funcoes (atualizados a cada ciclo) ---
// uint32_t = inteiro de 32 bits sem sinal (0 a 4.294.967.295)
// Suficiente para guardar tempo em microsegundos por anos.
uint32_t tempoLoopUs            = 0;  // Tempo total do loop()
uint32_t tempoPegarClimaUs      = 0;  // Tempo do pegarClima()
uint32_t tempoPegarDHTUS        = 0;  // Tempo do pegarClima()
uint32_t tempoRtcNowUs          = 0;  // Tempo do rtc.now()
uint32_t tempoLeituraBotoesUs   = 0;  // Tempo de leitura dos botoes
uint32_t tempoLogicaAlarmeUs    = 0;  // Tempo da logica de alarmes
uint32_t tempoAtualizarDisplayUs = 0; // Tempo de atualizar o OLED
float    cpuUsoEstimado         = 0.0;  // Estimativa de uso da CPU (%)
UBaseType_t stackLoopWords      = 0;    // Stack livre da task principal

// --- Player de audio (I2S) ---
bool          i2sIniciado      = false; // True quando o I2S foi inicializado
int           melodiaAtualIdx  = 0;     // Qual melodia esta tocando (0-4)
int           notaAtualIdx     = 0;     // Qual nota dentro da melodia
unsigned long notaInicioMs     = 0;     // Quando a nota atual comecou
float         faseOnda         = 0.0;   // Fase da onda senoidal (para gerar som)

// --- LCD ---
bool lcdOkBoot = false;  // True quando LCD foi inicializado com sucesso

// === TIMER DE ROTAÇÃO DO LCD 5s (FASE 1.4) ===
// Timer não bloqueante (via esp_timer) que alterna a linha 2 do LCD a cada 5 segundos
// Linha 1: HH:MM | umidade% | temp°C (fixo, atualizado a cada segundo)
// Linha 2: alterna entre endereço IP/AP e Tmin/Tmax a cada 5s
esp_timer_handle_t lcd_rotation_timer = NULL;  // Handle do timer
bool lcd_line2_show_ip = true;                  // true=mostrar IP/AP, false=mostrar Tmin/Tmax
static void lcd_rotation_callback(void* arg) {
  // Chamado a cada 5 segundos pelo esp_timer (contexto não bloqueante)
  lcd_line2_show_ip = !lcd_line2_show_ip;
}

// --- Controle de mensagem de clima ---
bool mostrarMsgClima = false;       // True quando deve mostrar mensagem de clima
unsigned long inicioMsgClima = 0;   // Quando a mensagem de clima começou a aparecer
#define DURACAO_MSG_CLIMA 10000     // Duração da mensagem de clima em ms (10 segundos)
char msgClima[30] = "";             // String com a mensagem de clima

// Boot nao bloqueante: o setup so inicia a serial e o resto acontece no loop.
enum BootState {
  BOOT_SERIAL_INIT,
  BOOT_DISPLAY,
  BOOT_LCD,
  BOOT_SPIFFS,
  BOOT_RTC,
  BOOT_I2S,
  BOOT_BOTOES,
  BOOT_WIFI_START,
  BOOT_WIFI_WAIT,
  BOOT_WIFI_RESULT,
  BOOT_NTP,
  BOOT_SERVER,
  BOOT_DHT,
  BOOT_COMPLETO
};

BootState bootState = BOOT_SERIAL_INIT;
bool bootFinalizado = false;
bool oledOkBoot = false;
unsigned long bootStepTime = 0;
unsigned long bootWifiT0 = 0;
#define BOOT_MSG_INTERVAL 80
#define WIFI_TIMEOUT_MS   15000

// ============================================================================
// FASE 2.1: MULTITAREFA RTOS - ARQUITETURA DUAL-CORE
// ============================================================================
// O ESP32 tem 2 núcleos:
//   - Core 0 (PRO): Cuida de WiFi, BLE, tarefas do sistema (padrão Arduino loop no Core 1)
//   - Core 1 (APP): Tarefas padrão do Arduino, renderização de UI, atualizações de display
//
// Arquitetura:
//   - TaskAquisicao (Core 0, alta prioridade): Leitura de sensores (DHT22, RTC, API de Clima)
//     → Produz dados → xQueueOverwrite(1 elemento) → dados mais novos sempre disponíveis
//   - TaskUI (Core 1, prioridade média): Consome dados dos sensores, renderiza displays
//     → Lê da fila → Atualiza OLED/LCD/Web → Sem esperas bloqueantes (timeout 0)
//
// Estrutura de dados passada pela fila
struct SensorData {
  float dht_temp;          // Temperatura local do DHT22 (°C)
  float dht_humidity;      // Umidade local do DHT22 (%)
  float api_temp;          // Temperatura da API (°C)
  int   weather_code;      // Código do tempo Meteosource
  uint32_t timestamp_ms;   // Quando estes dados foram coletados
  bool  valid;             // true se os dados forem recentes/válidos
};

// Handles de Fila + Tarefa
QueueHandle_t sensorQueue = NULL;        // xQueueOverwrite (1 elemento, não bloqueante)
TaskHandle_t taskAquisicaoHandle = NULL;
TaskHandle_t taskUIHandle = NULL;
SensorData sensorDataCurrent = {};       // Dados de sensor mais recentes (fallback)

// Tempo das tarefas
unsigned long taskAquisicaoLastRun = 0;  // Último tempo de execução (ms)
unsigned long taskUILastRun = 0;
#define TASK_AQUISICAO_INTERVAL 5000     // Lê sensores a cada 5 segundos
#define TASK_UI_INTERVAL 500             // Atualização de UI a cada 500ms

// Prioridades de tarefas (número maior = prioridade maior)
#define TASK_AQUISICAO_PRIORITY (configMAX_PRIORITIES - 2)  // Alta: leituras de sensor + chamadas de API
#define TASK_UI_PRIORITY (configMAX_PRIORITIES - 3)         // Média-alta: renderização de UI

// ============================================================================
// TAREFA: TaskAquisicao (Core 0) - Aquisição de dados de sensores
// ============================================================================
// Executa no Core 0 (núcleo de WiFi/rede) em alta prioridade
// Responsabilidades:
//   1. Ler DHT22 (temperatura, umidade) - I2C no GPIO 23
//   2. Buscar clima na API Meteosource (HTTP) - usa WiFi
//   3. Ler tempo RTC (não usado aqui, mantido no loop principal por enquanto)
//   4. Escrever em xQueueOverwrite (sempre os dados mais recentes disponíveis)
//
// Design não bloqueante:
//   - Pula operações se não tiver passado tempo suficiente
//   - Timeouts curtos em requisições HTTP
//   - Sem chamadas delay() (usa vTaskDelay com portTICK_PERIOD_MS)
void TaskAquisicao(void *pvParameters) {
  logPrintln("[TASK] TaskAquisicao iniciada (Core 0)");
  SensorData dados = {};
  
  while (1) {
    // Executa aquisição a cada TASK_AQUISICAO_INTERVAL ms
    if (millis() - taskAquisicaoLastRun >= TASK_AQUISICAO_INTERVAL) {
      taskAquisicaoLastRun = millis();
      
      // ==================== LEITURA DHT22 ====================
      // Lê temperatura e umidade locais
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
      if (!isnan(h) && !isnan(t)) {
        dados.dht_temp = t;
        dados.dht_humidity = h;
        dados.valid = true;
        // logInfo("[TASK-ACQ] DHT22: T=%.1f°C H=%.1f%%\n", t, h); // Removido log excessivo
      } else {
        logWarn("[TASK-ACQ] DHT22 read failed\n");
        dados.dht_temp = NAN;
        dados.dht_humidity = NAN;
      }
      
      // ==================== API METEOSOURCE ====================
      // Busca previsão do tempo (a cada 10 minutos via cfgClimaIntervaloMs)
      static unsigned long lastClimaFetch = 0;
      if (millis() - lastClimaFetch >= cfgClimaIntervaloMs && WiFi.status() == WL_CONNECTED) {
        lastClimaFetch = millis();
        pegarClima();  // Atualiza temperatura global, climaAtual
        dados.api_temp = temperatura;
        dados.weather_code = climaAtual.weather_code;
        logInfo("[TASK-ACQ] API Clima atualizada\n"); // Log simplificado
      } else {
        dados.api_temp = temperatura;
        dados.weather_code = climaAtual.weather_code;
      }
      
      // Record timestamp and validity
      dados.timestamp_ms = millis();
      dados.valid = true;
      
      // ==================== ESCREVE NA FILA ====================
      // xQueueOverwrite: sempre substitui o mais antigo (fila de 1 elemento)
      // Sem espera - se a fila estiver cheia, sobrescreve; sempre tem sucesso
      if (sensorQueue != NULL) {
        xQueueOverwrite(sensorQueue, &dados);
        // Atualiza fallback global
        sensorDataCurrent = dados;
      }
    }
    
    // Atraso mínimo para evitar timeout do watchdog (cedência de 50ms)
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ============================================================================
// TAREFA: TaskUI (Core 1) - Interface de Usuário + Atualizações de display
// ============================================================================
// Executa no Core 1 (núcleo APP) em prioridade média-alta
// Responsabilidades:
//   1. Ler fila de sensores (não bloqueante, timeout=0)
//   2. Atualizar variáveis globais (dhtTemperature, dhtHumidity)
//   3. Renderizar OLED/LCD baseado nos dados do sensor
//   4. Lidar com requisições do servidor web (server.handleClient)
//
// Design não bloqueante:
//   - Leitura de fila com timeout 0 (retorna imediatamente se estiver vazia)
//   - Atualizações de display a cada 500ms
//   - Sem operações de rede bloqueantes (já na TaskAquisicao)
void TaskUI(void *pvParameters) {
  logPrintln("[TASK] TaskUI iniciada (Core 1)");
  SensorData dados = {};
  
  while (1) {
    // Executa atualização de UI a cada TASK_UI_INTERVAL ms
    if (millis() - taskUILastRun >= TASK_UI_INTERVAL) {
      taskUILastRun = millis();
      
      // ==================== LER FILA DE SENSORES ====================
      // Leitura não bloqueante: retorna imediatamente se estiver vazia
      // Usa os dados mais recentes (xQueueOverwrite garante que o último esteja disponível)
      if (xQueueReceive(sensorQueue, &dados, 0) == pdTRUE) {
        // A fila tem novos dados: atualiza as variáveis globais
        if (dados.valid) {
          dhtTemperature = dados.dht_temp;
          dhtHumidity = dados.dht_humidity;
          temperatura = dados.api_temp;
          climaAtual.weather_code = dados.weather_code;
        }
      }
      // Se a fila estiver vazia, continua com os últimos valores conhecidos (sem espera)
      
      // ==================== RENDERIZAÇÃO DE UI ====================
      // Aqui você poderia adicionar lógica adicional de renderização de UI se necessário
      // (atualmente lidado no loop() principal para compatibilidade reversa)
      
      // ==================== SERVIDOR WEB ====================
      // Lida com requisições HTTP (não bloqueante, retorna imediatamente)
      server.handleClient();
    }
    
    // Atraso mínimo para evitar timeout do watchdog (cedência de 50ms)
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ============================================================================
// PARTE 9: PAGINA WEB (HTML + CSS + JavaScript embutidos no ESP32)
// ============================================================================
// Todo o visual da interface web fica aqui como texto (string).
// Quando alguem acessa o IP do ESP32, ele envia este conteudo.
//
// R"rawliteral(...)rawliteral" = string "crua" em C++.
// Dentro dela nao precisamos escapar aspas (") nem barras (\).
// Perfeito para escrever HTML/CSS/JS diretamente no codigo C++.
//
// PROGMEM = guarda na memoria flash (4MB) em vez da RAM (300KB).
// Como essa string nunca muda, a flash e o lugar certo.
static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR"><head>
<meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Relogio Inteligente</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:'Courier New',monospace;background:#0a0a0f;color:#e0e0e0;padding:12px;max-width:700px;margin:0 auto}
h1{color:#38bdf8;font-size:1.3rem;margin-bottom:10px;text-align:center}
h2{color:#94a3b8;font-size:1rem;margin:14px 0 6px;border-bottom:1px solid #1a1a2e;padding-bottom:4px}
.clock{text-align:center;font-size:4rem;font-weight:bold;color:#4ade80;background:#111118;border:1px solid #1a1a2e;border-radius:12px;padding:16px 0 10px;margin:10px 0;letter-spacing:4px;text-shadow:0 0 20px #4ade8055}
.clock-sub{text-align:center;color:#64748b;font-size:0.85rem;margin-bottom:8px}
.card{background:#111118;border:1px solid #1a1a2e;border-radius:8px;padding:10px;margin:6px 0}
input[type=number],input[type=text],input[type=password]{width:110px;background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;text-align:center;font-family:inherit}
select{background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;font-family:inherit}
button{background:#1e40af;color:#fff;border:none;padding:5px 12px;border-radius:5px;cursor:pointer;margin:3px;font-size:0.85rem}
button:hover{background:#2563eb}.on{background:#16a34a}.on:hover{background:#15803d}.off{background:#dc2626}.off:hover{background:#b91c1c}
.row{display:flex;align-items:center;gap:6px;flex-wrap:wrap;margin:3px 0}.lbl{color:#64748b;min-width:180px;font-size:0.85rem}.sep{color:#555}
.tabs{display:flex;gap:4px;margin:10px 0}.tab{flex:1;padding:8px;text-align:center;background:#111118;border:1px solid #1a1a2e;border-radius:6px 6px 0 0;cursor:pointer;color:#64748b;font-size:0.78rem}
.tab.active{background:#1a1a2e;color:#4ade80;border-bottom:2px solid #4ade80}
.page{display:none}.page.active{display:block}
.tpanel{display:none;background:#0a0a0f;border:1px solid #1a1a2e;border-radius:0 0 6px 6px;padding:10px;min-height:200px;max-height:320px;overflow-y:auto;font-size:0.78rem;line-height:1.5;white-space:pre-wrap;color:#4ade80}
.tpanel.active{display:block}.tpanel.cgraf{white-space:normal;max-height:none;padding:8px}
.cgrid{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-top:8px}.ccard{background:#111118;border:1px solid #1a1a2e;border-radius:8px;padding:4px;overflow:hidden}
.sbtn{display:block;width:100%;padding:9px;background:#1d4ed8;color:#fff;border:none;border-radius:7px;cursor:pointer;font-size:0.9rem;font-family:inherit}
</style></head><body>
<h1>Relogio Inteligente</h1>
<div class="tabs"><div class="tab active" onclick="sp(0)">Principal</div><div class="tab" onclick="sp(1)">Monitoramento</div><div class="tab" onclick="sp(2)">Sobre</div></div>
<div id="pg0" class="page active">
<div class="clock" id="clk">--:--:--</div>
<div class="clock-sub"><span id="tmp">--</span> | <span id="upt">--</span></div>
<h2>Hora do Sistema</h2><div class="card"><div class="row"><input type="number" id="sh" min="0" max="23" value="0"><span class="sep">:</span><input type="number" id="sm" min="0" max="59" value="0"><button onclick="setH()">Salvar Hora</button></div></div>
<h2>Alarmes</h2><div id="als"></div>
<h2>Clima</h2><div class="card"><div class="row"><span id="tmpLocal" class="lbl">Temperatura Local (DHT): --</span><span id="humLocal" class="lbl">Umidade Local (DHT): --</span><span id="tmpApi" class="lbl">Previsao do Tempo (API): --</span><button onclick="doClima()">Atualizar Clima</button></div></div>
</div>
<div id="pg1" class="page">
<h2>Monitoramento</h2>
<div class="tabs"><div class="tab active" onclick="stab(0)">Logs</div><div class="tab" onclick="stab(1)">Serial</div><div class="tab" onclick="stab(2)">Flash</div><div class="tab" onclick="stab(3)">Graficos</div></div>
<div class="tpanel active" id="tp0">carregando...</div>
<div class="tpanel" id="tp1">carregando serial...</div>
<div class="tpanel" id="tp2">carregando flash...</div>
<div class="tpanel cgraf" id="tp3"><button class="sbtn" id="btnSalvar" onclick="forcaSalvar()">Forcar Salvamento na Flash</button><button class="sbtn" id="btnDownload" style="background:#065f46" onclick="baixarBackup()">Baixar Backup TXT</button><input type="file" id="arquivoBackup" accept=".txt" style="display:none" onchange="restaurarBackup(this)"><button class="sbtn" style="background:#7c3aed" onclick="document.getElementById('arquivoBackup').click()">Restaurar Backup TXT</button><div class="cgrid"><div class="ccard"><canvas id="pc0" height="90"></canvas></div><div class="ccard"><canvas id="pc1" height="90"></canvas></div><div class="ccard"><canvas id="pc2" height="90"></canvas></div><div class="ccard"><canvas id="pc3" height="90"></canvas></div><div class="ccard"><canvas id="pc4" height="90"></canvas></div><div class="ccard"><canvas id="pc5" height="90"></canvas></div><div class="ccard"><canvas id="pc6" height="90"></canvas></div></div></div>
</div>
<div id="pg2" class="page">
<h2>Sobre e Configuracoes</h2>
<div class="card">Projeto Relogio Inteligente ESP32.<br>Universidade: Pontifícia Universidade Católica do Paraná.<br>Integrantes: Emanuel Henrique, Julioo Miguel, Gabriel Rossi, Murilo Bizz, Olavo, Murilo.<br>GitHub: https://github.com/emanuelhmarcineksilva/Relogio</div>
<div class="card"><b>Configuracoes</b><br>Use estes campos para ajustar leitura dos sensores, intervalo do clima e backup da flash sem recompilar o firmware.<div class="row"><span class="lbl">Leitura DHT (ms)</span><input id="cfgDht" type="number" min="2000"></div><div class="row"><span class="lbl">Clima API (ms)</span><input id="cfgClima" type="number" min="60000"></div><div class="row"><span class="lbl">Backup Flash (ms)</span><input id="cfgBackup" type="number" min="300000"></div><button onclick="salvarCfg()">Salvar Configuracoes</button></div>
<div class="card"><b>Troca de WiFi</b><div class="row"><span class="lbl">SSID</span><input id="wssid" type="text"></div><div class="row"><span class="lbl">Senha</span><input id="wpass" type="password"></div><button onclick="salvarWifi()">Salvar WiFi</button><div id="wifiResp"></div></div>
</div>
<script>
const M=['Aurora','Sino','Despertar','Fur Elise','Estrela'];
const PN=['Loop','Clima API','RTC','Botoes','DHT Temp','Display','DHT Umidade'];
const PC=['#4ade80','#38bdf8','#f97316','#a78bfa','#facc15','#f472b6','#22d3ee'];
function sp(n){document.querySelectorAll('.page').forEach((p,i)=>p.classList.toggle('active',i==n));document.querySelectorAll('body>.tabs .tab').forEach((t,i)=>t.classList.toggle('active',i==n));if(n==1)gS();if(n==2)gCfg();}
function stab(n){document.querySelectorAll('#pg1 .tab').forEach((t,i)=>t.classList.toggle('active',i==n));document.querySelectorAll('#pg1 .tpanel').forEach((p,i)=>p.classList.toggle('active',i==n));if(n==2)gF();if(n==3)gPerf();}
function fixCanvas(c){c.width=c.parentElement.clientWidth-8||260;c.height=90;}
function drawChart(id,data,color,name,cur,label='us'){const c=document.getElementById(id);if(!c)return;fixCanvas(c);const ctx=c.getContext('2d'),w=c.width,h=c.height;ctx.fillStyle='#0d0d16';ctx.fillRect(0,0,w,h);ctx.strokeStyle='#1e2038';for(let i=0;i<4;i++){const y=10+(h-20)*i/3;ctx.beginPath();ctx.moveTo(0,y);ctx.lineTo(w,y);ctx.stroke();}if(!data||data.length<2){ctx.fillStyle=color;ctx.font='bold 10px monospace';ctx.fillText(name+': '+cur+label,4,14);return;}const mx=Math.max(...data,1),mn=Math.min(...data,0),sp=Math.max(mx-mn,1),yMax=mx+sp*0.3,yMin=Math.max(mn-sp*0.3,0),rng=yMax-yMin||1,px=i=>w*i/(data.length-1),py=v=>h-10-(v-yMin)/rng*(h-20);const grad=ctx.createLinearGradient(0,0,0,h);grad.addColorStop(0,color+'44');grad.addColorStop(1,color+'08');ctx.fillStyle=grad;ctx.beginPath();data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));ctx.lineTo(w,h-10);ctx.lineTo(0,h-10);ctx.closePath();ctx.fill();ctx.strokeStyle=color;ctx.lineWidth=2;ctx.shadowColor=color;ctx.shadowBlur=10;ctx.beginPath();data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));ctx.stroke();ctx.shadowBlur=0;ctx.fillStyle=color;ctx.font='bold 10px monospace';ctx.fillText(name+': '+cur+label,4,14);ctx.fillStyle='#64748b';ctx.font='9px monospace';ctx.fillText('max:'+mx+label+' min:'+mn+label,4,26);}
async function gPerf(){try{const r=await fetch('/api/perf');const d=await r.json();for(let f=0;f<7;f++){const raw=d.hist[f],pos=d.pos,full=d.full,ord=full?[...raw.slice(pos),...raw.slice(0,pos)]:raw.slice(0,pos);const label=(f===4)?'°C':(f===6?'%':'us');const current=(f===4||f===6)?(d.atual[f]/10).toFixed(1):d.atual[f];drawChart('pc'+f,ord,PC[f],PN[f],current,label);}}catch(e){}}
async function forcaSalvar(){await fetch('/api/salvar');}
async function baixarBackup(){const r=await fetch('/api/download_perf');const txt=await r.text();const blob=new Blob([txt],{type:'text/plain'});const url=URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download='backup_perf.txt';a.click();URL.revokeObjectURL(url);}
async function restaurarBackup(input){if(!input.files.length)return;const txt=await input.files[0].text();const r=await fetch('/api/upload_perf',{method:'POST',headers:{'Content-Type':'text/plain'},body:txt});alert(await r.text());input.value='';}
function rA(d){let h='';for(let i=0;i<3;i++){const a=d.alarmes[i];h+='<div class="card"><div class="row"><span class="lbl">Alm '+(i+1)+'</span><input type="number" id="ah'+i+'" min="0" max="23" value="'+a.h+'"><span class="sep">:</span><input type="number" id="am'+i+'" min="0" max="59" value="'+a.m+'"><select id="ml'+i+'">';for(let j=0;j<M.length;j++)h+='<option value="'+j+'"'+(j==a.mel?' selected':'')+'>'+M[j]+'</option>';h+='</select><button class="'+(a.a?'on':'off')+'" onclick="tg('+i+')">'+(a.a?'ON':'OFF')+'</button><button onclick="sv('+i+')">Salvar</button></div></div>';}document.getElementById('als').innerHTML=h;document.getElementById('tmp').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('tmpLocal').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('humLocal').textContent='Umidade Local (DHT): '+(d.dhtHum>-100?d.dhtHum.toFixed(1)+'%':'--');document.getElementById('tmpApi').textContent='Previsao do Tempo (API): '+d.temp.toFixed(1)+'°C';}
async function gS(){try{const r=await fetch('/api/log');document.getElementById('tp0').textContent=await r.text()}catch(e){}}
async function gSer(){try{const r=await fetch('/api/serial');const el=document.getElementById('tp1');el.textContent=await r.text();el.scrollTop=el.scrollHeight}catch(e){}}
async function gF(){try{const r=await fetch('/api/flash');document.getElementById('tp2').textContent=await r.text()}catch(e){}}
let editandoAlarmes=false;
document.addEventListener('focusin',e=>{if(e.target.closest&&e.target.closest('#als'))editandoAlarmes=true;});
document.addEventListener('focusout',e=>{if(e.target.closest&&e.target.closest('#als')){setTimeout(()=>{if(!document.querySelector('#als input:focus,#als select:focus'))editandoAlarmes=false;},200);}});
async function gA(){try{const r=await fetch('/api/alarmes');const d=await r.json();if(!editandoAlarmes)rA(d);document.getElementById('clk').textContent=d.hora;document.getElementById('upt').textContent='Up: '+d.uptime+'s';document.getElementById('tmp').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('tmpLocal').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('humLocal').textContent='Umidade Local (DHT): '+(d.dhtHum>-100?d.dhtHum.toFixed(1)+'%':'--');document.getElementById('tmpApi').textContent='Previsao do Tempo (API): '+d.temp.toFixed(1)+'°C';}catch(e){}}
async function gCfg(){try{const r=await fetch('/api/config');const d=await r.json();document.getElementById('cfgDht').value=d.dhtMs;document.getElementById('cfgClima').value=d.climaMs;document.getElementById('cfgBackup').value=d.backupMs;document.getElementById('wssid').value=d.ssid||'';}catch(e){}}
async function salvarCfg(){const d=document.getElementById('cfgDht').value,c=document.getElementById('cfgClima').value,b=document.getElementById('cfgBackup').value;await fetch('/api/config/set?dht='+d+'&clima='+c+'&backup='+b);}
async function salvarWifi(){const s=encodeURIComponent(document.getElementById('wssid').value),p=encodeURIComponent(document.getElementById('wpass').value);const r=await fetch('/api/wifi/set?ssid='+s+'&pass='+p);document.getElementById('wifiResp').textContent=await r.text();}
async function setH(){const h=document.getElementById('sh').value,m=document.getElementById('sm').value;await fetch('/api/hora?h='+h+'&m='+m);}
async function sv(i){const h=document.getElementById('ah'+i).value,m=document.getElementById('am'+i).value,ml=document.getElementById('ml'+i).value;await fetch('/api/alarme?id='+i+'&h='+h+'&m='+m+'&mel='+ml);editandoAlarmes=false;gA();}
async function tg(i){await fetch('/api/alarme/toggle?id='+i);gA();}
async function doClima(){await fetch('/api/clima');gA();}
gA();gS();gSer();setInterval(gA,2000);setInterval(gS,3000);setInterval(gSer,2000);setInterval(()=>{if(document.getElementById('tp3').classList.contains('active'))gPerf();},5000);
</script></body></html>
)rawliteral";

// [TAG_AUDIO_I2S]
// === 9. I2S — INICIALIZACAO E PLAYER DE MELODIA ===

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
  if (!alarmeDisparo) {
    if (notaAtualIdx != 0 || faseOnda != 0) {
      i2s_zero_dma_buffer(I2S_PORT);
      notaAtualIdx = 0;
      faseOnda = 0;
    }
    return;
  }

  // Le a nota atual da melodia armazenada na flash (PROGMEM).
  // Isso economiza RAM, pois as melodias sao fixas.
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
  int16_t samples[128];

  if (nota.freq > 0) {
    // Formula da senoide: sample = sin(fase) * volume.
    // incremento define quanto a fase avanca por amostra para atingir a frequencia desejada.
    float incremento = 2.0f * PI * nota.freq / (float)SAMPLE_RATE;
    for (int i = 0; i < 128; i++) {
      samples[i] = (int16_t)(sinf(faseOnda) * 8000); // Volume moderado
      faseOnda += incremento;
      if (faseOnda >= 2.0f * PI) faseOnda -= 2.0f * PI;
    }
  } else {
    // Nota com frequencia 0 representa pausa, entao preenchemos com zero.
    memset(samples, 0, sizeof(samples));
  }

  // Envia o bloco para o periférico I2S.
  // Timeout curto evita travar o restante do firmware.
  size_t written;
  i2s_write(I2S_PORT, samples, sizeof(samples), &written, 5 / portTICK_PERIOD_MS);
}

// [TAG_DISPLAY_OLED]
// === 10. DISPLAY — FUNCOES AUXILIARES ===

// Mostra 2 linhas durante o boot
void oledMsg(const char* l1, const char* l2 = nullptr) {
  displayTela.clearDisplay();
  displayTela.setTextSize(1);
  displayTela.setTextColor(SSD1306_WHITE);
  displayTela.setCursor(0, 12);
  displayTela.print(l1);
  if (l2) {
    displayTela.setCursor(0, 32);
    displayTela.print(l2);
  }
  displayTela.display();
}

// === LCD FUNCTIONS ===
void iniciarLCD() {
  lcd.begin();
  lcd.backlight();
  lcdOkBoot = true;
  logPrintln("LCD OK (16x2 I2C)");
  
  // FASE 1.4: Inicializa timer de rotação do LCD (5s não bloqueante)
  // Cria um esp_timer periódico que alterna o conteúdo da linha 2 do LCD a cada 5 segundos
  esp_timer_create_args_t timer_args = {
    .callback = &lcd_rotation_callback,
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK,      // Chama no contexto de tarefa (seguro, não bloqueante)
    .name = "lcd_rotation"
  };
  
  if (esp_timer_create(&timer_args, &lcd_rotation_timer) == ESP_OK) {
    // Inicia o timer com período de 5000ms (repetindo)
    if (esp_timer_start_periodic(lcd_rotation_timer, 5000000) == ESP_OK) {  // 5s em microssegundos
      logPrintln("[LCD] Timer de rotacao de 5s iniciado");
      lcd_line2_show_ip = true;  // Começa com exibição de IP/AP
    } else {
      logError("[LCD] Falha ao iniciar timer de rotacao\n");
    }
  } else {
    logError("[LCD] Falha ao criar timer de rotacao\n");
  }
}

void atualizarLCD(DateTime agora, bool wifiConectado) {
  if (!lcdOkBoot) return;
  
  // ============================================================
  // LINHA 1 (FIXA): HH:MM | umidade% | temp°C
  // ============================================================
  // Formato: "HH:MM | UU% | TT°C" (exatamente 16 caracteres quando formatado corretamente)
  // Exemplo: "14:23 | 65% | 22°C" - cabe no display 16x2
  lcd.setCursor(0, 0);
  char linha1[17];
  
  if (isnan(dhtTemperature) || isnan(dhtHumidity)) {
    // Fallback se o DHT ainda não estiver inicializado
    snprintf(linha1, sizeof(linha1), "%02d:%02d |  -- | --c", 
      agora.hour(), agora.minute());
  } else {
    // Formato completo com temperatura e umidade
    snprintf(linha1, sizeof(linha1), "%02d:%02d |%2.0f%% |%2.0fc", 
      agora.hour(), agora.minute(), dhtHumidity, dhtTemperature);
  }
  
  // Garante exatamente 16 caracteres (preenche com espaços se necessário)
  for (int i = strlen(linha1); i < 16; i++) {
    linha1[i] = ' ';
  }
  linha1[16] = '\0';
  lcd.print(linha1);
  
  // ============================================================
  // LINHA 2 (ROTATIVA): IP/AP ou Tmin/Tmax (alternância de 5s via esp_timer)
  // ============================================================
  // FASE 1.4: Alterna a cada 5 segundos SEM delay() bloqueante
  // Usa o callback esp_timer (lcd_rotation_callback) para inverter lcd_line2_show_ip
  lcd.setCursor(0, 1);
  char linha2[17];
  
  if (lcd_line2_show_ip) {
    // Modo 1: Mostrar endereço WiFi (IP ou AP)
    if (wifiConectado) {
      String ip = WiFi.localIP().toString();
      // Trunca para caber 16 caracteres: "IP: XXX.XXX.X.XX"
      snprintf(linha2, sizeof(linha2), "w %s", ip.c_str());
    } else {
      // Sem WiFi: mostra endereço de fallback AP
      snprintf(linha2, sizeof(linha2), "AP: 192.168.4.1");
    }
  } else {
    // Modo 2: Mostrar temperatura min/max da API
    // Formato: "Tmin:XX Tmax:XX" (14 caracteres, cabe em 16)
    if (climaAtual.valid) {
      snprintf(linha2, sizeof(linha2), "Tmin:%2.0f Tmax:%2.0f",
        climaAtual.temp_min, climaAtual.temp_max);
    } else {
      snprintf(linha2, sizeof(linha2), "Tmin: -- Tmax: --");
    }
  }
  
  // Trunca para no máximo 16 caracteres (largura do LCD)
  if (strlen(linha2) > 16) {
    linha2[16] = '\0';
  }
  
  // Preenche com espaços para completar as colunas restantes
  for (int i = strlen(linha2); i < 16; i++) {
    linha2[i] = ' ';
  }
  linha2[16] = '\0';
  lcd.print(linha2);
}

// === DESENHO DE ÍCONES DE CLIMA NO OLED ===
// Ícones 5x8 para o OLED
void desenharNuvem(int x, int y) {
  // Desenha uma nuvem simples (3x3 pixels)
  displayTela.fillCircle(x, y, 3, SSD1306_WHITE);
  displayTela.fillCircle(x+4, y, 3, SSD1306_WHITE);
  displayTela.fillRect(x-2, y+1, 8, 3, SSD1306_WHITE);
}

void desenharSol(int x, int y) {
  // Desenha um sol (círculo + raios)
  displayTela.fillCircle(x, y, 2, SSD1306_WHITE);
  displayTela.drawLine(x, y-5, x, y-3, SSD1306_WHITE);  // Raio superior
  displayTela.drawLine(x, y+3, x, y+5, SSD1306_WHITE);  // Raio inferior
  displayTela.drawLine(x-5, y, x-3, y, SSD1306_WHITE);  // Raio esquerdo
  displayTela.drawLine(x+3, y, x+5, y, SSD1306_WHITE);  // Raio direito
}

void desenharTermometro(int x, int y) {
  // Desenha um termômetro (T vertical)
  displayTela.drawLine(x-2, y-3, x-2, y+2, SSD1306_WHITE);  // Bastão esquerdo
  displayTela.drawLine(x+2, y-3, x+2, y+2, SSD1306_WHITE);  // Bastão direito
  displayTela.fillCircle(x, y+3, 2, SSD1306_WHITE);  // Bulbo
}

void desenharNeve(int x, int y) {
  // Desenha um floco de neve (asterisco estilizado)
  displayTela.drawLine(x-2, y, x+2, y, SSD1306_WHITE);        // Horizontal
  displayTela.drawLine(x, y-2, x, y+2, SSD1306_WHITE);        // Vertical
  displayTela.drawLine(x-2, y-2, x+2, y+2, SSD1306_WHITE);    // Diagonal 1
  displayTela.drawLine(x-2, y+2, x+2, y-2, SSD1306_WHITE);    // Diagonal 2
}

// ============================================================================
// FASE 1.2: MAPEAMENTO DE CÓDIGO DE TEMPO + FUNÇÕES DE DESENHO DE ÍCONES
// ============================================================================
// Códigos de tempo Meteosource (veja ApiParPrevisaoDiaria.txt linhas 58-265):
//   1-5: chuva/chuva leve / possível chuva
//   6-10: variantes de nuvens
//   11-13: variantes ensolaradas (parcialmente ensolarado, céu limpo, etc.)
//   etc. (tabela completa na documentação)
//
// Nosso mapeamento simplificado (5 ícones):
//   Ícone NEVE: T < 15°C
//   Ícone GUARDA-CHUVA: weather_code EM {chuva, chuva_leve, poss_chuva}
//   Ícone NUVEM: weather_code EM {nublado, muito_nublado}
//   Ícone SOL: weather_code EM {parcialmente_ensolarado, ceu_limpo}
//   Ícone TERMÔMETRO: T >= 25°C (quente)
//
// Função: Mapeia código de tempo Meteosource → tipo de clima (0-4)
// Retorna:
//   0 = NEVE (frio, T < 15)
//   1 = GUARDA-CHUVA (chuvoso)
//   2 = NUVEM (nublado)
//   3 = SOL (limpo/parcialmente ensolarado)
//   4 = TERMÔMETRO (quente, T >= 25)
int getTipoClima(float temp, int weather_code) {
  // Prioridade 1: Extremos de temperatura sobrescrevem o código do tempo
  if (temp < 15.0f) return 0;    // NEVE (muito frio)
  if (temp >= 25.0f) return 4;   // TERMÔMETRO (quente)
  
  // Prioridade 2: Código do tempo para temperaturas moderadas (15-25°C)
  // Códigos de chuva: 1 (chuva), 2 (chuva leve), 3 (possível chuva)
  if (weather_code >= 1 && weather_code <= 3) return 1;  // GUARDA-CHUVA (chuvoso)
  
  // Códigos de nuvem: 6-10 (várias condições de nuvens)
  if (weather_code >= 6 && weather_code <= 10) return 2;  // NUVEM (nublado)
  
  // Códigos de céu limpo/ensolarado: 11-13+ (variantes ensolaradas, céu limpo, parcialmente ensolarado)
  if (weather_code >= 11 && weather_code <= 20) return 3;  // SOL (limpo)
  
  // Padrão: trata códigos desconhecidos como nublado
  return 2;  // NUVEM (fallback)
}

// ============================================================================
// FUNÇÕES DE DESENHO DE ÍCONES (ESPAÇOS RESERVADOS) (FASE 1.2)
// ============================================================================
// Estes são apenas espaços reservados. O usuário substituirá por designs SVG/BMP reais depois.
// Cada função desenha um ícone simples na posição (x, y) no OLED 128x64.
// Os ícones têm ~16-20 pixels de largura para exibição no OLED.

// Ícone 1: GUARDA-CHUVA (proteção contra chuva) - espaço reservado
// Formato de guarda-chuva simples: arco em cima, cabo embaixo
void desenharGuarda_Chuva(int x, int y) {
  // Topo do guarda-chuva (arco)
  displayTela.drawLine(x-5, y, x+5, y, SSD1306_WHITE);      // Linha superior
  displayTela.drawCircle(x, y, 5, SSD1306_WHITE);           // Topo curvado
  // Cabo do guarda-chuva (linha vertical + gancho)
  displayTela.drawLine(x, y+5, x, y+10, SSD1306_WHITE);     // Haste
  displayTela.drawLine(x, y+10, x+3, y+11, SSD1306_WHITE);  // Gancho
}

// Ícone 2: NUVEM (nublado) - espaço reservado
// Já definido acima como desenharNuvem() - nenhuma ação necessária

// Ícone 3: SOL (claro/ensolarado) - espaço reservado
// Já definido acima como desenharSol() - nenhuma ação necessária

// Ícone 4: TERMÔMETRO (quente) - espaço reservado
// Já definido acima como desenharTermometro() - nenhuma ação necessária

// Ícone 5: FLOCO DE NEVE (frio/neve) - espaço reservado
// Já definido acima como desenharNeve() - nenhuma ação necessária

// ============================================================================
// GERAÇÃO DE MENSAGEM BASEADA NO CLIMA
// ============================================================================
// Atualiza a string global msgClima[] baseada no weather_code e temp atuais
// Chamada quando o alarme dispara para fornecer conselhos contextuais sobre o tempo
void gerarMsgClima() {
  int tipoClima = getTipoClima(temperatura, climaAtual.weather_code);
  
  switch(tipoClima) {
    case 0:  // NEVE (T < 15)
      strncpy(msgClima, "Muito frio! Agasalho", sizeof(msgClima) - 1);
      break;
    case 1:  // GUARDA-CHUVA (chuvoso)
      strncpy(msgClima, "Chuva prevista hoje!", sizeof(msgClima) - 1);
      break;
    case 2:  // NUVEM (nublado)
      strncpy(msgClima, "Dia nublado", sizeof(msgClima) - 1);
      break;
    case 3:  // SOL (claro)
      strncpy(msgClima, "Dia lindo!", sizeof(msgClima) - 1);
      break;
    case 4:  // TERMÔMETRO (T >= 25)
      strncpy(msgClima, "Dia quente! Hidrate", sizeof(msgClima) - 1);
      break;
    default:
      strncpy(msgClima, "Clima indefinido", sizeof(msgClima) - 1);
  }
  msgClima[sizeof(msgClima) - 1] = '\0';  // Garante terminação nula
}



// [TAG_CLIMA_API]
// === 11. CLIMA (INTEGRAÇÃO METEOSOURCE - FASE 1.1) ===

// Busca a previsão do tempo diária na API Meteosource (HTTP não bloqueante)
// Atualiza a estrutura global climaAtual e a variável temperatura
// Temporização: Chamada periodicamente (a cada 10 minutos por padrão)
// Não bloqueante: Usa HTTPClient com timeout curto para evitar travamento
void pegarClima() {
  uint64_t t0 = esp_timer_get_time(); // Inicia o temporizador de desempenho
  
  if (WiFi.status() != WL_CONNECTED) {
    logWarn("[CLIMA] WiFi desconectado, pulando requisicao\n");
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }

  HTTPClient http;
  http.setConnectTimeout(5000);   // Timeout de conexão 5s
  http.setTimeout(5000);          // Timeout de resposta 5s
  
  logInfo("[CLIMA] Buscando previsao Meteosource...\n");
  
  // HTTPS (seguro) - Meteosource usa HTTPS para o plano gratuito
  if (!http.begin(METEOSOURCE_URL)) {
    logError("[CLIMA] Falha ao inicializar HTTP\n");
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  int httpCode = http.GET();
  
  if (httpCode != 200) {
    logWarn("[CLIMA] HTTP error: %d\n", httpCode);
    http.end();
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  // Analisa a resposta JSON
  // Estrutura esperada: {daily: {data: [{all_day: {weather: "...", icon: N, temperature_min: X, temperature_max: Y}}]}}
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, http.getStream());
  
  if (error) {
    logError("[CLIMA] JSON parse error: %s\n", error.c_str());
    http.end();
    tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
    return;
  }
  
  // Extrai dados do tempo da primeira entrada diária
  // Meteosource agrupa os dados sob: daily -> data -> [0] -> all_day
  if (doc["daily"]["data"].size() > 0) {
    JsonObject daily = doc["daily"]["data"][0];
    
    // Temperatura atual (valor principal para exibição)
    if (daily["all_day"]["temperature"].is<float>()) {
      temperatura = daily["all_day"]["temperature"].as<float>();
      climaAtual.temp_current = temperatura;
    }
    
    // Extremos diários
    if (daily["all_day"]["temperature_min"].is<float>()) {
      climaAtual.temp_min = daily["all_day"]["temperature_min"].as<float>();
    }
    if (daily["all_day"]["temperature_max"].is<float>()) {
      climaAtual.temp_max = daily["all_day"]["temperature_max"].as<float>();
    }
    
    // Código do tempo (usado para seleção de ícone)
    if (daily["all_day"]["weather"].is<int>()) {
      climaAtual.weather_code = daily["all_day"]["weather"].as<int>();
    }
    
    // Marca como válido e atualiza o carimbo de tempo
    climaAtual.valid = true;
    climaAtual.last_fetch = millis();
    
    logInfo("[CLIMA] Sucesso! T=%.1f°C Tmin=%.1f Tmax=%.1f Code=%d\n",
      temperatura, climaAtual.temp_min, climaAtual.temp_max, climaAtual.weather_code);
  } else {
    logWarn("[CLIMA] Resposta vazia ou mal formatada\n");
  }
  
  http.end();
  tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0); // Stop timer
}

// [TAG_WEB_SERVER]
// === 12. SERVIDOR WEB — ROTAS ===

const char* estadoToStr(String st) {
  if (st == "RELOGIO")          return "RELOGIO";
  if (st == "MENU")             return "MENU";
  if (st == "LISTA_ALARMES")    return "LISTA_ALARMES";
  if (st == "DETALHE_ALARME")   return "DETALHE_ALARME";
  if (st == "EDITAR_ALARME_H")  return "EDITAR_ALARME_H";
  if (st == "EDITAR_ALARME_M")  return "EDITAR_ALARME_M";
  if (st == "ESCOLHER_MELODIA") return "ESCOLHER_MELODIA";
  if (st == "HORA")             return "HORA";
  if (st == "EDITAR_HORA_M")    return "EDITAR_HORA_M";
  return "?";
}

// GET / — página HTML
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

// GET /api/log — texto formatado como terminal
void handleLog() {
  DateTime agora = rtc.now();
  bool wOk = (WiFi.status() == WL_CONNECTED);
  char buf[800];
  char alarmTxt[300];
  char* p = alarmTxt;

  for (int i = 0; i < NUM_ALARMES; i++) {
    p += sprintf(p, "#%d: %02d:%02d [%s] Mel: %s\n",
      i + 1, alarmes[i].hora, alarmes[i].minuto,
      alarmes[i].ativo ? "ON " : "OFF",
      MELODIA_NOMES[alarmes[i].melodia]);
  }

   snprintf(buf, sizeof(buf),
     "=== RELOGIO INTELIGENTE ===\n"
     "Estado: %s\n"
     "Hora RTC: %02d:%02d:%02d\n"
     "Uptime: %lu s\n\n"
     "--- ALARMES ---\n"
     "%s"
     "Disparando: %s%s\n\n"
     "--- REDE ---\n"
     "WiFi: %s (RSSI %d)\n"
     "IP: %s\n\n"
     "--- CLIMA ---\n"
     "Local (DHT): %.1f C\n"
     "Previsao (API): %.1f C (%s)\n"
     "Mensagem: %s\n\n"
     "--- PERFORMANCE ---\n"
     "Loop: %lu us\n"
     "CPU: %.1f%%\n"
     "Heap: %lu / min %lu\n"
     "Stack: %lu bytes\n",
     estadoToStr(estadoAtual),
     agora.hour(), agora.minute(), agora.second(),
     (unsigned long)(millis() / 1000UL),
     alarmTxt,
     alarmeDisparo ? "SIM (Alarme " : "NAO",
     alarmeDisparo ? (String(alarmeDisparoIdx + 1) + ")").c_str() : "",
     wOk ? "OK" : "OFF", wOk ? WiFi.RSSI() : 0,
     wOk ? WiFi.localIP().toString().c_str() : "...",
      isnan(dhtTemperature) ? 0.0f : dhtTemperature,
      temperatura, METEOSOURCE_PLACE_ID,
      msgClima,
     (unsigned long)tempoLoopUs,
     cpuUsoEstimado,
     (unsigned long)ESP.getFreeHeap(),
     (unsigned long)ESP.getMinFreeHeap(),
     (unsigned long)(stackLoopWords * sizeof(StackType_t))
   );
  server.send(200, "text/plain", buf);
}

// GET /api/alarmes — JSON com alarmes + temp + hora + uptime + local
void handleAlarmes() {
  DateTime agora = rtc.now();
  char horaStr[12];
  snprintf(horaStr, sizeof(horaStr), "%02d:%02d:%02d", agora.hour(), agora.minute(), agora.second());
  char json[600];
  char* p = json;
  float dhtTemp = isnan(dhtTemperature) ? -127.0f : dhtTemperature;
  float dhtHum  = isnan(dhtHumidity) ? -127.0f : dhtHumidity;
  p += sprintf(p, "{\"alarmes\":[");
  for (int i = 0; i < NUM_ALARMES; i++) {
    if (i > 0) p += sprintf(p, ",");
    p += sprintf(p, "{\"h\":%d,\"m\":%d,\"a\":%s,\"mel\":%d}",
      alarmes[i].hora, alarmes[i].minuto,
      alarmes[i].ativo ? "true" : "false",
      alarmes[i].melodia);
  }
  p += sprintf(p, "],\"temp\":%.1f,\"hum\":%.1f,\"dhtTemp\":%.1f,\"dhtHum\":%.1f,\"hora\":\"%s\",\"uptime\":%lu,\"cidade\":\"%s\"}",
    temperatura,
    dhtHum,
    dhtTemp,
    dhtHum,
    horaStr, (unsigned long)(millis()/1000UL), METEOSOURCE_PLACE_ID);
  server.send(200, "application/json", json);
}

// GET /api/hora?h=HH&m=MM — ajusta o RTC
void handleSetHora() {
  if (server.hasArg("h") && server.hasArg("m")) {
    int h = server.arg("h").toInt();
    int m = server.arg("m").toInt();
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), h, m, 0));
    logPrintf("[WEB] Hora ajustada: %02d:%02d\n", h, m);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Faltam h e m");
  }
}

// GET /api/alarme?id=N&h=HH&m=MM&mel=M — edita um alarme
void handleSetAlarme() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id >= 0 && id < NUM_ALARMES) {
      if (server.hasArg("h"))   alarmes[id].hora    = server.arg("h").toInt();
      if (server.hasArg("m"))   alarmes[id].minuto  = server.arg("m").toInt();
      if (server.hasArg("mel")) alarmes[id].melodia = server.arg("mel").toInt() % TOTAL_MELODIAS;
      logPrintf("[WEB] Alarme %d: %02d:%02d mel=%d\n", id+1, alarmes[id].hora, alarmes[id].minuto, alarmes[id].melodia);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "ID invalido");
    }
  } else {
    server.send(400, "text/plain", "Falta id");
  }
}

// GET /api/alarme/toggle?id=N — liga/desliga um alarme
void handleToggleAlarme() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id >= 0 && id < NUM_ALARMES) {
      alarmes[id].ativo = !alarmes[id].ativo;
      logPrintf("[WEB] Alarme %d: %s\n", id+1, alarmes[id].ativo ? "ON" : "OFF");
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "ID invalido");
    }
  }
}

// GET /api/clima — força atualização do clima
void handleClima() {
  pegarClima();
  char buf[30];
  snprintf(buf, sizeof(buf), "Temp: %.1f C", temperatura);
  server.send(200, "text/plain", buf);
}

// GET /api/serial — log serial em tempo real (ring buffer)
void handleSerial() {
  server.send(200, "text/plain", getLogContent());
}

// GET /api/flash — log salvo na flash (SPIFFS)
void handleFlash() {
  server.send(200, "text/plain", carregarLogFlash());
}

// GET /api/config — retorna parametros configuraveis atuais
void handleConfigGet() {
  String json = "{";
  json += "\"dhtMs\":" + String(cfgDhtIntervaloMs) + ",";
  json += "\"climaMs\":" + String(cfgClimaIntervaloMs) + ",";
  json += "\"backupMs\":" + String(cfgBackupIntervaloMs) + ",";
  json += "\"ssid\":\"" + String(wifiSSID) + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// GET /api/config/set?dht=&clima=&backup= — salva parametros da aba Config
void handleConfigSet() {
  if (server.hasArg("dht")) cfgDhtIntervaloMs = max(2000UL, (unsigned long)server.arg("dht").toInt());
  if (server.hasArg("clima")) cfgClimaIntervaloMs = max(60000UL, (unsigned long)server.arg("clima").toInt());
  if (server.hasArg("backup")) cfgBackupIntervaloMs = max(300000UL, (unsigned long)server.arg("backup").toInt());
  salvarConfigParams();
  logPrintf("[CFG] Parametros salvos: DHT=%lu Clima=%lu Backup=%lu\n", cfgDhtIntervaloMs, cfgClimaIntervaloMs, cfgBackupIntervaloMs);
  server.send(200, "text/plain", "Parametros salvos");
}

// GET /api/wifi/set?ssid=&pass= — troca SSID/senha e tenta conectar sem regravar firmware
void handleWifiSet() {
  if (!server.hasArg("ssid") || !server.hasArg("pass")) {
    server.send(400, "text/plain", "Envie ssid e pass");
    return;
  }
  String novoSsid = server.arg("ssid");
  String novaSenha = server.arg("pass");
  novoSsid.trim();
  if (novoSsid.length() == 0) {
    server.send(400, "text/plain", "SSID vazio");
    return;
  }
  strncpy(wifiSSID, novoSsid.c_str(), sizeof(wifiSSID) - 1);
  wifiSSID[sizeof(wifiSSID) - 1] = '\0';
  strncpy(wifiPassword, novaSenha.c_str(), sizeof(wifiPassword) - 1);
  wifiPassword[sizeof(wifiPassword) - 1] = '\0';
  salvarWifiConfig();

  WiFi.disconnect(true, false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - t0) < 15000) {
    yield();
  }

  if (WiFi.status() == WL_CONNECTED) {
    modoAP = false;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("RelogioConfig", "12345678");
    logPrintf("[WIFI] Conectado em nova rede: %s | IP: %s\n", wifiSSID, WiFi.localIP().toString().c_str());
    String msg = "Conectado com sucesso. IP WiFi: ";
    msg += WiFi.localIP().toString();
    msg += " | AP de recuperacao: 192.168.4.1";
    server.send(200, "text/plain", msg);
  } else {
    // Se falhou conectar na nova rede, ativa AP imediatamente para nao perder acesso.
    modoAP = true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("RelogioConfig", "12345678");
    logWarn("[WIFI] Falha ao conectar com nova rede. Mantendo configuracao salva para proximo boot.\n");
    server.send(200, "text/plain", "Falha ao conectar agora. AP ativo em 192.168.4.1 (RelogioConfig/12345678).");
  }
}

// GET /api/perf — histórico de performance (JSON)
void handlePerf() {
  String json;
  json.reserve(7000);
  json += "{\"atual\":["; 
  json += tempoLoopUs;          json += ',';
  json += tempoPegarClimaUs;    json += ',';
  json += tempoRtcNowUs;        json += ',';
  json += tempoLeituraBotoesUs; json += ',';
  json += (uint32_t)(isnan(dhtTemperature) ? 0 : dhtTemperature * 10.0f);
  json += ',';
  json += tempoAtualizarDisplayUs;
  json += ',';
  json += (uint32_t)(isnan(dhtHumidity) ? 0 : dhtHumidity * 10.0f);
  json += "],\"hist\":[";
  for (int f = 0; f < PERF_FUNCOES; f++) {
    if (f > 0) json += ',';
    json += '[';
    for (int i = 0; i < PERF_HIST_SIZE; i++) {
      if (i > 0) json += ',';
      json += perfHist[f][i];
    }
    json += ']';
  }
  json += "],\"pos\":";
  json += perfHistPos;
  json += ",\"full\":";
  json += perfHistFull ? "true" : "false";
  json += ",\"cpu\":";
  json += cpuUsoEstimado;
  json += ",\"heap\":";
  json += ESP.getFreeHeap();
  json += ",\"minHeap\":";
  json += ESP.getMinFreeHeap();
  json += ",\"stack\":";
  json += (unsigned long)(stackLoopWords * sizeof(StackType_t));
  json += ",\"flashUsed\":";
  json += SPIFFS.usedBytes();
  json += ",\"flashTotal\":";
  json += SPIFFS.totalBytes();
  json += ",\"psramTotal\":";
  json += ESP.getPsramSize();
  json += ",\"psramFree\":";
  json += ESP.getFreePsram();
  json += ",\"tasks\":";
  json += uxTaskGetNumberOfTasks();
  json += ",\"wifi\":\"";
  json += (WiFi.status() == WL_CONNECTED) ? "CONECTADO" : "DESCONECTADO";
  json += "\",\"ap\":\"";
  json += modoAP ? "ATIVO" : "INATIVO";
  json += "\"";
  json += '}';
  server.send(200, "application/json", json);
}

// GET /api/salvar — força salvamento completo na flash
void handleSalvar() {
  salvarLogFlash();
  salvarPerfFlash();
  server.send(200, "text/plain", "OK");
}

// GET /api/download_perf — retorna histórico de performance como texto CSV
void handleDownloadPerf() {
  const char* nomes[] = {"Loop","Clima","RTC","Botoes","DHT Temp","Display","DHT Umidade"};
  String txt = "# Historico de Performance - ESP32 Relogio\n";
  txt += "# Uma amostra a cada 5 segundos\n";
  txt += "# Formato: Funcao:val1,val2,...\n";
  for (int f = 0; f < PERF_FUNCOES; f++) {
    txt += String(nomes[f]) + ":";
    int total = perfHistFull ? PERF_HIST_SIZE : perfHistPos;
    for (int i = 0; i < total; i++) {
      int idx = perfHistFull ? (perfHistPos + i) % PERF_HIST_SIZE : i;
      if (i > 0) txt += ",";
      txt += String(perfHist[f][idx]);
    }
    txt += "\n";
  }
  txt += "pos:" + String(perfHistPos) + "\n";
  txt += "full:" + String(perfHistFull ? "1" : "0") + "\n";
  server.sendHeader("Content-Disposition", "attachment; filename=backup_perf.txt");
  server.send(200, "text/plain", txt);
}

// POST /api/upload_perf — restaura histórico de performance a partir de CSV
void handleUploadPerf() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Nenhum dado recebido.");
    return;
  }
  String corpo = server.arg("plain");
  const char* nomes[] = {"Loop","Clima","RTC","Botoes","DHT Temp","Display","DHT Umidade"};
  for (int f = 0; f < PERF_FUNCOES; f++) {
    String prefixo = String(nomes[f]) + ":";
    int inicio = corpo.indexOf(prefixo);
    if (inicio < 0) continue;
    inicio += prefixo.length();
    int fim = corpo.indexOf('\n', inicio);
    if (fim < 0) fim = corpo.length();
    String vals = corpo.substring(inicio, fim);
    int i = 0, pos = 0;
    while (i < PERF_HIST_SIZE && pos < (int)vals.length()) {
      int virgula = vals.indexOf(',', pos);
      if (virgula < 0) virgula = vals.length();
      perfHist[f][i++] = (uint32_t)vals.substring(pos, virgula).toInt();
      pos = virgula + 1;
    }
  }
  int ip = corpo.indexOf("pos:");
  if (ip >= 0) { int fim = corpo.indexOf('\n', ip+4); perfHistPos = corpo.substring(ip+4, fim).toInt(); }
  int ifl = corpo.indexOf("full:");
  if (ifl >= 0) { int fim = corpo.indexOf('\n', ifl+5); perfHistFull = corpo.substring(ifl+5, fim).toInt() == 1; }
  if (perfHistPos < 0 || perfHistPos >= PERF_HIST_SIZE) perfHistPos = 0;
  salvarPerfFlash();
  server.send(200, "text/plain", "Backup restaurado com sucesso!");
}

// Inicializa todas as rotas
void iniciarServidorWeb() {
  server.on("/",                handleRoot);
  server.on("/api/log",         handleLog);
  server.on("/api/alarmes",     handleAlarmes);
  server.on("/api/hora",        handleSetHora);
  server.on("/api/alarme",      handleSetAlarme);
  server.on("/api/alarme/toggle",handleToggleAlarme);
  server.on("/api/clima",       handleClima);
  server.on("/api/serial",      handleSerial);
  server.on("/api/flash",       handleFlash);
  server.on("/api/config",      handleConfigGet);
  server.on("/api/config/set",  handleConfigSet);
  server.on("/api/wifi/set",    handleWifiSet);
  server.on("/api/perf",        handlePerf);
  server.on("/api/salvar",      handleSalvar);
  server.on("/api/download_perf", handleDownloadPerf);
  server.on("/api/upload_perf",   handleUploadPerf);
  server.begin();
  logPrintln("Servidor Web Iniciado!");
}

// [TAG_BOOT]
// === 13. SETUP/BOOT ===

void setup() {
  Serial.begin(115200);
  bootStepTime = millis();
}

bool bootStep() {
  unsigned long agora = millis();
  switch (bootState) {
    case BOOT_SERIAL_INIT:
      Serial.println();
      Serial.println(F("========================================"));
      Serial.println(F(" Relogiov3 - BOOT SEM BLOQUEIO"));
      Serial.println(F("========================================"));
      bootState = BOOT_DISPLAY;
      bootStepTime = agora;
      break;
    case BOOT_DISPLAY:
      Wire.begin(SDA_PIN, SCL_PIN);
      oledOkBoot = displayTela.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
      if (oledOkBoot) { displayTela.clearDisplay(); displayTela.setTextColor(SSD1306_WHITE); oledMsg("[1/9] Display", "OK"); }
      bootState = BOOT_LCD; bootStepTime = agora; break;
    case BOOT_LCD:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      iniciarLCD();
      if (oledOkBoot) oledMsg("[2/9] LCD", lcdOkBoot ? "OK" : "ERRO");
      bootState = BOOT_SPIFFS; bootStepTime = agora; break;
    case BOOT_SPIFFS:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      SPIFFS.begin(true); carregarPerfFlash(); carregarWifiConfig(); carregarConfigParams();
      if (oledOkBoot) oledMsg("[3/9] SPIFFS", "OK");
      bootState = BOOT_RTC; bootStepTime = agora; break;
    case BOOT_RTC:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (rtc.begin() && rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      if (oledOkBoot) oledMsg("[4/9] RTC", "OK");
      bootState = BOOT_I2S; bootStepTime = agora; break;
    case BOOT_I2S:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      iniciarI2S(); if (oledOkBoot) oledMsg("[5/9] Audio", i2sIniciado ? "OK" : "Falhou");
      bootState = BOOT_BOTOES; bootStepTime = agora; break;
     case BOOT_BOTOES: {
       if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
       
       // ===== FASE 2.2: INICIALIZAR ISR DE BOTÕES + FILA =====
       // Cria fila de eventos de botão (5 elementos)
       buttonEventQueue = xQueueCreate(5, sizeof(ButtonEvent));
       if (buttonEventQueue == NULL) {
         logError("[BOOT] Falha ao criar buttonEventQueue\n");
       } else {
         logPrintln("[BOOT] buttonEventQueue criada");
       }
       
       // Cria timers de debounce
       esp_timer_create_args_t btn1_timer_args = {
         .callback = &btn1_debounce_callback,
         .arg = NULL,
         .dispatch_method = ESP_TIMER_TASK,
         .name = "btn1_debounce"
       };
       esp_timer_create(&btn1_timer_args, &btn1_debounce_timer);
       
       esp_timer_create_args_t btn2_timer_args = {
         .callback = &btn2_debounce_callback,
         .arg = NULL,
         .dispatch_method = ESP_TIMER_TASK,
         .name = "btn2_debounce"
       };
       esp_timer_create(&btn2_timer_args, &btn2_debounce_timer);
       
       // Configura pinos GPIO com ISR na borda de descida (falling) HIGH→LOW
       pinMode(pinoButton1, INPUT_PULLUP);
       pinMode(pinoButton2, INPUT_PULLUP);
       attachInterrupt(digitalPinToInterrupt(pinoButton1), btn1_isr, FALLING);
       attachInterrupt(digitalPinToInterrupt(pinoButton2), btn2_isr, FALLING);
       
       logPrintln("[BOOT] ISRs de botões inicializadas");
       
       if (oledOkBoot) oledMsg("[6/9] Botoes", "OK + ISR");
       bootState = BOOT_WIFI_START; bootStepTime = agora; break;
     }
    case BOOT_WIFI_START:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[7/9] WiFi", wifiSSID);
      logPrintf("[BOOT] Tentando WiFi: %s\n", wifiSSID);
      WiFi.mode(WIFI_STA); WiFi.setSleep(false); WiFi.setAutoReconnect(true); WiFi.begin(wifiSSID, wifiPassword);
      bootWifiT0 = agora; bootState = BOOT_WIFI_WAIT; break;
    case BOOT_WIFI_WAIT:
      if (WiFi.status() == WL_CONNECTED || (agora - bootWifiT0) >= WIFI_TIMEOUT_MS) { bootState = BOOT_WIFI_RESULT; bootStepTime = agora; }
      break;
    case BOOT_WIFI_RESULT:
      if (WiFi.status() == WL_CONNECTED) {
        modoAP = false;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char ipBuf[30];
          snprintf(ipBuf, sizeof(ipBuf), "IP: %s", WiFi.localIP().toString().c_str());
          oledMsg("[7/9] WiFi OK", ipBuf);
        }
        logPrintf("[BOOT] WiFi conectado: %s\n", WiFi.localIP().toString().c_str());
      } else {
        modoAP = true;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char apBuf[30];
          snprintf(apBuf, sizeof(apBuf), "AP: %s", WiFi.softAPIP().toString().c_str());
          oledMsg("[7/9] MODO AP", apBuf);
        }
        logWarn("[BOOT] WiFi falhou. AP ativo: RelogioConfig / 12345678\n");
      }
      bootState = BOOT_NTP; bootStepTime = agora; break;
    case BOOT_NTP:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[8/9] NTP", "Sincronizando");
      if (WiFi.status() == WL_CONNECTED) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        struct tm ti;
        if (getLocalTime(&ti, 5000)) {
          rtc.adjust(DateTime(ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec));
          if (oledOkBoot) oledMsg("[8/9] NTP", "OK");
        } else {
          if (oledOkBoot) oledMsg("[8/9] NTP", "Falhou");
        }
      } else {
        if (oledOkBoot) oledMsg("[8/9] NTP", "Sem WiFi");
      }
      bootState = BOOT_SERVER; bootStepTime = agora; break;
    case BOOT_SERVER:
      if (oledOkBoot) oledMsg("[9/9] Server", "Iniciando");
      iniciarServidorWeb(); bootState = BOOT_DHT; bootStepTime = agora; break;
     case BOOT_DHT: {
       dht.begin();
       ultimoBackupFlash = millis();
       
       // ===== FASE 2.1: INICIALIZAR TAREFAS RTOS =====
       // Cria fila de sensores (1 elemento, 100 bytes por elemento)
       sensorQueue = xQueueCreate(1, sizeof(SensorData));
       if (sensorQueue == NULL) {
         logError("[BOOT] Falha ao criar sensorQueue\n");
       } else {
         logPrintln("[BOOT] sensorQueue criada");
       }
       
       // Cria TaskAquisicao no Core 0 (prioridade alta)
       BaseType_t resultA = xTaskCreatePinnedToCore(
         TaskAquisicao,                    // Função a executar
         "TaskAquisicao",                  // Nome da tarefa (para depuração)
         4096,                             // Tamanho da pilha (bytes) - ops de sensor + HTTP
         NULL,                             // Parâmetros da tarefa
         TASK_AQUISICAO_PRIORITY,          // Prioridade (alta)
         &taskAquisicaoHandle,             // Handle da tarefa
         0                                 // Core 0 (WiFi/rede)
       );
       
       if (resultA == pdPASS) {
         logPrintln("[BOOT] TaskAquisicao criada no Core 0");
       } else {
         logError("[BOOT] Falha ao criar TaskAquisicao\n");
       }
       
       // Cria TaskUI no Core 1 (prioridade média-alta)
       BaseType_t resultU = xTaskCreatePinnedToCore(
         TaskUI,                           // Função a executar
         "TaskUI",                         // Nome da tarefa
         4096,                             // Tamanho da pilha (bytes) - ops de display
         NULL,                             // Parâmetros da tarefa
         TASK_UI_PRIORITY,                 // Prioridade (média-alta)
         &taskUIHandle,                    // Handle da tarefa
         1                                 // Core 1 (app/display)
       );
       
       if (resultU == pdPASS) {
         logPrintln("[BOOT] TaskUI criada no Core 1");
       } else {
         logError("[BOOT] Falha ao criar TaskUI\n");
       }
       
       if (oledOkBoot) {
         if (WiFi.status() == WL_CONNECTED) {
           char urlBuf[40];
           snprintf(urlBuf, sizeof(urlBuf), "http://%s", WiFi.localIP().toString().c_str());
           oledMsg("[OK] Sistema", urlBuf);
         } else {
           oledMsg("[OK] Sistema", "AP: 192.168.4.1");
         }
       }
       bootState = BOOT_COMPLETO;
       break;
     }
    case BOOT_COMPLETO:
      logPrintln("BOOT COMPLETO");
      return true;
  }
  return false;
}

// === 14. LOOP PRINCIPAL ===

void loop() {
  if (!bootFinalizado) {
    bootFinalizado = bootStep();
    return;
  }
  uint64_t tLoop0 = esp_timer_get_time();
  unsigned long agoraMs = millis();

  // [TAG_DHT_SENSOR]
  // A leitura do sensor DHT22 agora é feita de forma assíncrona pela TaskAquisicao (Core 0).
  // Os valores são atualizados globalmente através da fila de sensores na TaskUI.


  // ==================================================================
  // 14.0 SERVIDOR WEB
  // ==================================================================
  server.handleClient();

  // ==================================================================
  // 14.1 PLAYER DE MELODIA (não-bloqueante)
  // ==================================================================
  tocarMelodiaStep();

  // ==================================================================
  // 14.2 BOTÕES (FASE 2.2: ISR + FILA DE EVENTOS)
  // ==================================================================
  // Processamento de eventos de botão não bloqueante via ISR + fila
  // Em vez de varrer digitalRead() todo loop, a ISR enfileira eventos
  // O loop principal consome a fila (não bloqueante, timeout 0ms)
  //
  // ISR → timer de debounce 20ms → Fila de eventos → Loop principal consome
  uint64_t tBotoes0 = esp_timer_get_time();

  bool clicou1 = false;
  bool clicou2 = false;
  
  // ===== CONSOME EVENTOS DE BOTÃO DA FILA =====
  // Leitura não bloqueante: retorna imediatamente se a fila estiver vazia
  ButtonEvent btn_event = {};
  while (xQueueReceive(buttonEventQueue, &btn_event, 0) == pdTRUE) {
    // Processa evento de botão confirmado
    if (btn_event.debounce_state == 1) {  // Confirmado após debounce
      if (btn_event.button_id == 1) {
        clicou1 = true;
        logPrintf("[BTN] Botao1 (Nav) | Estado: %s | Tempo: %lu ms\n", 
          estadoAtual.c_str(), btn_event.timestamp_ms);
      } else if (btn_event.button_id == 2) {
        clicou2 = true;
        logPrintf("[BTN] Botao2 (Conf) | Estado: %s | Tempo: %lu ms\n", 
          estadoAtual.c_str(), btn_event.timestamp_ms);
      }
    }
  }

  tempoLeituraBotoesUs = (uint32_t)(esp_timer_get_time() - tBotoes0);

  // ==================================================================
  // 14.3 RTC + WIFI
  // ==================================================================
  uint64_t tRtc0 = esp_timer_get_time();
  DateTime agora = rtc.now();
  tempoRtcNowUs = (uint32_t)(esp_timer_get_time() - tRtc0);
  int hora   = agora.hour();
  int minuto = agora.minute();

  static int minutoAnterior = -1;
  bool precisaRedesenhar = false;

  bool wifiConectado = (WiFi.status() == WL_CONNECTED);
  wifiRSSI = wifiConectado ? WiFi.RSSI() : 0;

  // Busca clima na primeira conexão
  if (wifiConectado && !climaIniciado) {
    pegarClima();
    climaIniciado = true;
    precisaRedesenhar = true;
  }

  if (minuto != minutoAnterior || clicou1 || clicou2) {
    precisaRedesenhar = true;
    minutoAnterior = minuto;
  }

  static String estadoPassado = "";
  if (estadoAtual != estadoPassado) {
    precisaRedesenhar = true;
    estadoPassado = estadoAtual;
    displayTela.clearDisplay();
  }

  // ==================================================================
  // 14.4 LÓGICA DOS 3 ALARMES
  // ==================================================================
  uint64_t tAlarme0 = esp_timer_get_time();

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
       precisaRedesenhar = true;
       
       // Inicia exibição da mensagem de clima
       gerarMsgClima();
       mostrarMsgClima = true;
       inicioMsgClima = millis();
       
       logPrintf("*** ALARME %d DISPAROU! ***\n", i + 1);
     }
  }

  tempoLogicaAlarmeUs = (uint32_t)(esp_timer_get_time() - tAlarme0);

  // ==================================================================
  // 14.5 MÁQUINA DE ESTADOS (menus + botões)
  // ==================================================================
  if (estadoAtual == "RELOGIO") {
    // Clima usando o intervalo configurado na interface web
    static unsigned long tempoClima = 0;
    if (millis() - tempoClima > cfgClimaIntervaloMs && wifiConectado) {
      pegarClima();
      tempoClima = millis();
      precisaRedesenhar = true;
    }

    // Verifica timeout da mensagem de clima (10 segundos sem delay)
    if (mostrarMsgClima && (millis() - inicioMsgClima >= DURACAO_MSG_CLIMA)) {
      mostrarMsgClima = false;
    }

    // Atualiza LCD continuamente (a cada segundo a hora muda)
    static unsigned long ultimaAtualizacaoLCD = 0;
    static int segundoAnterior = -1;
    if (lcdOkBoot && agora.second() != segundoAnterior) {
      atualizarLCD(agora, wifiConectado);
      segundoAnterior = agora.second();
    }

    if (clicou1) {
      if (alarmeDisparo) {
        // Silencia o alarme
        alarmeDisparo = false;
        if (alarmeDisparoIdx >= 0) alarmes[alarmeDisparoIdx].jaTocou = true;
        logPrintln("[ALARME] Silenciado");
      } else {
        estadoAtual = "MENU";
        menuIndex = 0;
      }
    }

    // Render Relógio
    uint64_t tD0 = esp_timer_get_time();
    if (precisaRedesenhar && estadoAtual == "RELOGIO") {
      displayTela.clearDisplay();
      
      // Atualiza o LCD com hora, umidade e temperatura
      if (lcdOkBoot) {
        atualizarLCD(agora, wifiConectado);
      }

      // ================================================================
      // FASE 1.3: REDESIGN DO OLED - EXIBIÇÃO DE ÍCONE DE CLIMA
      // ================================================================
      // Obtém o tipo de clima atual (0-4)
      int tipoClima = getTipoClima(temperatura, climaAtual.weather_code);
      int iconX = 50;
      int iconY = 20;
      
      // Desenha o ícone apropriado baseado no tipo de clima
      // FASE 1.2: Usando funções de ícone de espaço reservado (o usuário customizará depois)
      switch(tipoClima) {
        case 0:  // NEVE (T < 15°C)
          desenharNeve();
          displayTela.setCursor(70, 15);
          displayTela.setTextSize(1);
          displayTela.print("NEVE");
          break;
          
        case 1:  // GUARDA-CHUVA (chuvoso)
          desenharGuarda_Chuva();
          displayTela.setCursor(70, 15);
          displayTela.setTextSize(1);
          displayTela.print("CHUVA");
          break;
          
        case 2:  // NUVEM (nublado)
          desenharNuvem();
          displayTela.setCursor(70, 15);
          displayTela.setTextSize(1);
          displayTela.print("NUVEM");
          break;
          
        case 3:  // SOL (claro)
          desenharSol();
          displayTela.setCursor(70, 15);
          displayTela.setTextSize(1);
          displayTela.print("SOL");
          break;
          
        case 4:  // TERMÔMETRO (T >= 25°C, quente)
          desenharTermometro();
          displayTela.setCursor(68, 15);
          displayTela.setTextSize(1);
          displayTela.print("QUENTE");
          break;
          
        default:
          // Fallback: desenha nuvem + ponto de interrogação
          desenharNuvem();
          displayTela.setCursor(70, 15);
          displayTela.setTextSize(1);
          displayTela.print("???");
          break;
      }

      // Exibição de temperatura (API vs Local)
      // Formato: "API: XX°C | DHT: XX°C"
      displayTela.setTextSize(1);
      displayTela.setCursor(0, 40);
      displayTela.print("API:");
      displayTela.setCursor(30, 40);
      if (climaAtual.valid) {
        displayTela.print(temperatura, 1);
        displayTela.print("C");
      } else {
        displayTela.print("--C");
      }

      // Temperatura DHT local (se disponível)
      displayTela.setCursor(60, 40);
      displayTela.print("DHT:");
      displayTela.setCursor(85, 40);
      if (!isnan(dhtTemperature)) {
        displayTela.print(dhtTemperature, 1);
        displayTela.print("C");
      } else {
        displayTela.print("--C");
      }

      // Linha de status (WiFi ou mensagem de clima durante o alarme)
      displayTela.setCursor(0, 55);
      if (mostrarMsgClima && alarmeDisparo) {
        // Durante o alarme: mostra mensagem de clima (limitada a 16 caracteres)
        char msgLimitada[17];
        strncpy(msgLimitada, msgClima, 16);
        msgLimitada[16] = '\0';
        displayTela.print(msgLimitada);
      } else {
        // Normal: mostra status do WiFi
        if (wifiConectado) {
          displayTela.print("WiFi: OK");
        } else {
          displayTela.print("WiFi: OFF");
        }
      }

      // Indicador de alarme (canto superior direito)
      if (alarmeDisparo) {
        displayTela.setCursor(80, 55);
        displayTela.setTextSize(1);
        displayTela.print("ALARME!");
      }

      displayTela.display();
    }
    tempoAtualizarDisplayUs = (uint32_t)(esp_timer_get_time() - tD0);

  // ---- MENU PRINCIPAL ----
  } else if (estadoAtual == "MENU") {
    if (clicou1) { menuIndex++; if (menuIndex >= 3) menuIndex = 0; }
     if (clicou2) {
       if (menuIndex == 0)      { estadoAtual = "LISTA_ALARMES"; alarmeListaIdx = 0; }
       else if (menuIndex == 1) { estadoAtual = "HORA"; tempHora = hora; tempMinuto = minuto; }
       else                     { estadoAtual = "RELOGIO"; }
     }

    // ---- MENU PRINCIPAL (continua no OLED) ----
    if (precisaRedesenhar && estadoAtual == "MENU") {
      displayTela.clearDisplay();
      displayTela.setTextSize(2);
      displayTela.setCursor(40, 0);
      displayTela.print("MENU");
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 25);
      displayTela.print(menuIndex == 0 ? "> ALARMES" : "  Alarmes");
      displayTela.setCursor(10, 40);
      displayTela.print(menuIndex == 1 ? "> HORA" : "  Hora");
      displayTela.setCursor(10, 55);
      displayTela.print(menuIndex == 2 ? "> SAIR" : "  Sair");
      displayTela.display();
    }

  // ---- LISTA DE 3 ALARMES + SAIR ----
  } else if (estadoAtual == "LISTA_ALARMES") {
    if (clicou1) { alarmeListaIdx++; if (alarmeListaIdx >= NUM_ALARMES + 1) alarmeListaIdx = 0; }
    if (clicou2) {
      if (alarmeListaIdx < NUM_ALARMES) {
        estadoAtual = "DETALHE_ALARME"; subMenuIndex = 0;
      } else {
        estadoAtual = "MENU"; // Sair -> volta ao menu
      }
    }

    if (precisaRedesenhar && estadoAtual == "LISTA_ALARMES") {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(30, 0);
      displayTela.print("ALARMES");
      for (int i = 0; i < NUM_ALARMES; i++) {
        displayTela.setCursor(2, 18 + i * 12);
        char linha[25];
        snprintf(linha, sizeof(linha), "%s%d: %02d:%02d %s %s",
          i == alarmeListaIdx ? ">" : " ",
          i + 1,
          alarmes[i].hora, alarmes[i].minuto,
          alarmes[i].ativo ? "ON " : "OFF",
          MELODIA_CURTO[alarmes[i].melodia]);
        displayTela.print(linha);
      }
      // Opção Sair
      displayTela.setCursor(2, 18 + NUM_ALARMES * 12);
      displayTela.print(alarmeListaIdx == NUM_ALARMES ? "> SAIR" : "  Sair");
      displayTela.display();
    }

  // ---- DETALHE DE UM ALARME ----
  } else if (estadoAtual == "DETALHE_ALARME") {
    if (clicou1) { subMenuIndex++; if (subMenuIndex >= 4) subMenuIndex = 0; }
    if (clicou2) {
      if (subMenuIndex == 0) {
        // Toggle ativo
        alarmes[alarmeListaIdx].ativo = !alarmes[alarmeListaIdx].ativo;
      } else if (subMenuIndex == 1) {
        // Editar hora
        estadoAtual = "EDITAR_ALARME_H";
        tempHora = alarmes[alarmeListaIdx].hora;
        tempMinuto = alarmes[alarmeListaIdx].minuto;
      } else if (subMenuIndex == 2) {
        // Escolher melodia
        estadoAtual = "ESCOLHER_MELODIA";
        melodiaEscolha = alarmes[alarmeListaIdx].melodia;
      } else {
        // Voltar
        estadoAtual = "LISTA_ALARMES";
      }
    }

    if (precisaRedesenhar && estadoAtual == "DETALHE_ALARME") {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      char titulo[15];
      snprintf(titulo, sizeof(titulo), "ALARME %d", alarmeListaIdx + 1);
      displayTela.setCursor(30, 0);
      displayTela.print(titulo);

      displayTela.setCursor(5, 18);
      char statusTxt[20];
      snprintf(statusTxt, sizeof(statusTxt), "%sStatus: %s",
        subMenuIndex == 0 ? "> " : "  ",
        alarmes[alarmeListaIdx].ativo ? "ON" : "OFF");
      displayTela.print(statusTxt);

      displayTela.setCursor(5, 30);
      displayTela.print(subMenuIndex == 1 ? "> Editar hora" : "  Editar hora");

      displayTela.setCursor(5, 42);
      char melTxt[25];
      snprintf(melTxt, sizeof(melTxt), "%sMelodia: %s",
        subMenuIndex == 2 ? "> " : "  ",
        MELODIA_NOMES[alarmes[alarmeListaIdx].melodia]);
      displayTela.print(melTxt);

      displayTela.setCursor(5, 54);
      displayTela.print(subMenuIndex == 3 ? "> Voltar" : "  Voltar");

      displayTela.display();
    }

  // ---- ESCOLHER MELODIA ----
  } else if (estadoAtual == "ESCOLHER_MELODIA") {
    if (clicou1) {
      melodiaEscolha++;
      if (melodiaEscolha >= TOTAL_MELODIAS) melodiaEscolha = 0;
    }
    if (clicou2) {
      alarmes[alarmeListaIdx].melodia = melodiaEscolha;
      estadoAtual = "DETALHE_ALARME";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(20, 0);
      displayTela.print("MELODIA");

      for (int i = 0; i < TOTAL_MELODIAS; i++) {
        displayTela.setCursor(5, 16 + i * 10);
        displayTela.print(i == melodiaEscolha ? "> " : "  ");
        displayTela.print(MELODIA_NOMES[i]);
      }
      displayTela.display();
    }
 
  // ---- EDITAR HORA DO ALARME ----
  } else if (estadoAtual == "EDITAR_ALARME_H") {
    if (clicou1) { tempHora++; if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_ALARME_M"; }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("AJUSTAR HORAS");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(20, 50, 55, 50, SSD1306_WHITE);
      displayTela.display();
    }

  // ---- EDITAR MINUTO DO ALARME ----
  } else if (estadoAtual == "EDITAR_ALARME_M") {
    if (clicou1) { tempMinuto++; if (tempMinuto > 59) tempMinuto = 0; }
    if (clicou2) {
      alarmes[alarmeListaIdx].hora = tempHora;
      alarmes[alarmeListaIdx].minuto = tempMinuto;
      logPrintf("[ALARME] %d salvo: %02d:%02d\n", alarmeListaIdx+1, tempHora, tempMinuto);
      estadoAtual = "RELOGIO";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("AJUSTAR MINUTOS");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(75, 50, 110, 50, SSD1306_WHITE);
      displayTela.display();
    }

  // ---- AJUSTAR HORA DO SISTEMA (RTC) ----
  } else if (estadoAtual == "HORA") {
    if (clicou1) { tempHora++; if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_HORA_M"; }

    
    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("HORA SISTEMA");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(20, 50, 55, 50, SSD1306_WHITE);
      displayTela.display();
    }

  } else if (estadoAtual == "EDITAR_HORA_M") {
    if (clicou1) { tempMinuto++; if (tempMinuto > 59) tempMinuto = 0; }
    if (clicou2) {
      rtc.adjust(DateTime(agora.year(), agora.month(), agora.day(), tempHora, tempMinuto, 0));
      logPrintf("[HORA] Sistema: %02d:%02d\n", tempHora, tempMinuto);
      estadoAtual = "RELOGIO";
    }

    if (precisaRedesenhar) {
      displayTela.clearDisplay();
      displayTela.setTextSize(1);
      displayTela.setCursor(10, 5);
      displayTela.print("MINUTO SISTEMA");
      displayTela.setTextSize(3);
      displayTela.setCursor(20, 25);
      if (tempHora < 10) displayTela.print("0");
      displayTela.print(tempHora);
      displayTela.print(":");
      if (tempMinuto < 10) displayTela.print("0");
      displayTela.print(tempMinuto);
      displayTela.drawLine(75, 50, 110, 50, SSD1306_WHITE);
      displayTela.display();
    }
  }

  // [TAG_PERF_METRICS]
  // ==================================================================
  // 14.6 MÉTRICAS + BACKUP FLASH (FASE 2.4: persistência de 24h)
  // ==================================================================
  tempoLoopUs = (uint32_t)(esp_timer_get_time() - tLoop0);
  cpuUsoEstimado = (tempoLoopUs / 50000.0f) * 100.0f;
  if (cpuUsoEstimado > 100.0f) cpuUsoEstimado = 100.0f;
  stackLoopWords = uxTaskGetStackHighWaterMark(nullptr);

  // Amostragem periodica de performance (1 ponto a cada 5 minutos = 300 segundos)
  // FASE 2.4: Changed from 5s to 5min for 24-hour history
  // 288 samples × 5min = 1440min = 24 hours
  if (millis() - ultimaAmostraPerf >= PERF_SAMPLE_MS) {
    perfHist[0][perfHistPos] = tempoLoopUs;
    perfHist[1][perfHistPos] = tempoPegarClimaUs;
    perfHist[2][perfHistPos] = tempoRtcNowUs;
    perfHist[3][perfHistPos] = tempoLeituraBotoesUs;
    perfHist[4][perfHistPos] = (uint32_t)(isnan(dhtTemperature) ? 0 : dhtTemperature * 10.0f);
    perfHist[5][perfHistPos] = tempoAtualizarDisplayUs;
    perfHist[6][perfHistPos] = (uint32_t)(isnan(dhtHumidity) ? 0 : dhtHumidity * 10.0f);
    
    perfHistPos = (perfHistPos + 1) % PERF_HIST_SIZE;
    if (perfHistPos == 0) perfHistFull = true;
    ultimaAmostraPerf = millis();
    
    logInfo("[PERF] Sample #%d: Loop=%lus API=%lus RTC=%lus DHT_T=%.1f°C DHT_H=%.1f%%\n",
      perfHistPos - 1, tempoLoopUs, tempoPegarClimaUs, tempoRtcNowUs,
      isnan(dhtTemperature) ? 0.0 : dhtTemperature,
      isnan(dhtHumidity) ? 0.0 : dhtHumidity);
  }

  // Backup na flash a cada 2 horas (com validação CRC)
  if (millis() - ultimoBackupFlash >= cfgBackupIntervaloMs) {
    salvarLogFlash();
    salvarPerfFlash();  // Agora inclui CRC
    ultimoBackupFlash = millis();
    logInfo("[BACKUP] Flash salvo com CRC - Boot #%d\n", rtc_data.boot_count);
  }
}