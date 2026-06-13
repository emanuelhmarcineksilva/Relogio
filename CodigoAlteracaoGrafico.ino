// RELOGIO INTELIGENTE ESP32
// Versao do firmware (exibida na aba Sobre)
#define FIRMWARE_VERSION "3.1.0"
// ============================================================================
//
// FUNCIONALIDADES:
//   -> Mostra hora em display OLED 128x64
//   -> 3 alarmes configuráveis com 5 melodias
//   -> Pagina web no celular/PC para controlar tudo via WiFi
//   -> Busca temperatura da cidade na internet (OpenWeatherMap)
//   -> Graficos de desempenho em tempo real na pagina web
//   -> Historico de desempenho salvo na flash (nao perde ao reiniciar)
//   -> Backup do historico como arquivo .txt (download/upload)
//
// HARDWARE NECESSARIO:
//   - ESP32 (qualquer modelo com WiFi)
//   - Display OLED SSD1306 128x64 (I2C: SDA=pino21, SCL=pino22)
//   - Modulo RTC DS3231 (I2C: mesmo barramento do OLED)
//   - Amplificador I2S MAX98357A + alto-falante
//   - 2 botoes push-button (pino 4 e pino 18)
//
// CONFIGURACAO RAPIDA:
//   1. Ligue o ESP32 e veja o IP no display OLED.
//   2. Se conectar no seu WiFi, abra o IP mostrado (ex.: http://192.168.0.20).
//   3. Se o WiFi falhar, conecte no AP de recuperacao:
//      SSID: RelogioConfig | Senha: 12345678 | URL: http://192.168.4.1
//   4. Na aba "Sobre e Configuracoes", atualize SSID e senha do WiFi.
//   5. O sistema salva a rede na flash e tenta reconectar automaticamente.
//   6. Clima da API e opcional: configure apiClima e cidade abaixo.
// ////////////////////////////////////////////////////////////////////////////////
#pragma GCC optimize("O2")  // Pede ao compilador para otimizar o codigo (mais rapido)

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
// PARTE 2: CONFIGURACOES DO HARDWARE
// ============================================================================
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

// --- Display LCD 16x2 I2C ---
#define LCD_ADDRESS 0x27       // Endereco I2C do LCD (pode ser 0x3F em alguns modelos)
#define LCD_COLUMNS 16         // Largura do LCD em caracteres
#define LCD_ROWS 2             // Altura do LCD em linhas
LiquidCrystal_I2C lcdTela(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Display LCD 16x2 via I2C

// --- Objetos de hardware ---
// Cada objeto representa um componente fisico e permite controla-lo.
RTC_DS3231 rtc;                                                // Relogio de tempo real
Adafruit_SSD1306 displayTela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // Display OLED
WebServer server(80); // Servidor web na porta 80 (porta padrao HTTP)

// ============================================================================
// PARTE 3: CREDENCIAIS E CONFIGURACOES
// ============================================================================
// !! CREDENCIAIS PADRAO do WiFi !!
// Estas sao usadas SOMENTE se nao houver configuracao salva na flash.
// Depois de configurar pela web, o ESP32 lembra a rede automaticamente.
const char* WIFI_SSID_DEFAULT     = "Galaxy S9ae76";    // Nome padrao da rede
const char* WIFI_PASSWORD_DEFAULT = "vgkb2019"; // Senha padrao da rede

// Variaveis que guardam o WiFi ATUAL (podem ser mudadas pela web)
// "char[]" = array de caracteres, como uma caixinha de texto.
// 64 caracteres e mais que suficiente para qualquer nome/senha de rede.
char wifiSSID[64]     = "Galaxy S9ae76";
char wifiPassword[64] = "vgkb2019";

// Flag que indica se estamos no modo Access Point ("roteador proprio")
// Quando o ESP32 nao consegue conectar no WiFi configurado, ele
// cria sua propria rede chamada "RelogioConfig" para voce acessar
// a pagina web e configurar um novo WiFi.
bool modoAP = false;

// Chave da API de clima - crie conta gratuita em: openweathermap.org
String apiClima = "76b19c5ce07af87452662d52bd95ce4f";
String cidade   = "Curitiba";  // Cidade para buscar temperatura

float temperatura     = 0.0; // Temperatura da API (previsao do tempo)
float dhtTemperature  = NAN; // Temperatura medida pelo DHT22 (local)
float dhtHumidity     = NAN; // Umidade medida pelo DHT22 (local)
unsigned long ultimoDhtLeitura = 0; // Momento da ultima leitura do DHT

// === PARAMETROS CONFIGURAVEIS PELA WEB ===
// Estes valores podem ser alterados pela aba "Config" da interface web.
// Isso permite ajustar o sistema sem regravar o firmware.
unsigned long cfgDhtIntervaloMs   = 5000; // Intervalo de leitura do DHT (ms)
unsigned long cfgClimaIntervaloMs = 600000; // Intervalo de busca do clima (ms) = 10min
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
const long  gmtOffset_sec    = -3 * 3600; // GMT-3 (Brasilia/Sao Paulo)
const int   daylightOffset_sec = 0;        // Sem horario de verao
unsigned long ultimoBackupFlash = 0;       // Quando foi o ultimo backup

// Adiciona texto ao log (salva no buffer E no monitor serial)
void logPrint(const char* msg) {
  Serial.print(msg); // Aparece no monitor serial do Arduino IDE
  int len = strlen(msg); // strlen conta quantos caracteres tem na mensagem
  for (int i = 0; i < len; i++) {
    logBuffer[logPos] = msg[i]; // Copia um caractere para o buffer
    logPos++;
    if (logPos >= LOG_BUF_SIZE) {          // Chegou no fim? Volta ao inicio
      logPos = 0;
      logWrapped = true; // Marca que o buffer ja deu a volta
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
  vsnprintf(tmp, sizeof(tmp), fmt, args); // Monta a string formatada
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
  ssid.trim(); // Remove espacos e \r\n extras
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
// Esses tempos sao gravados num ring buffer de 60 amostras por funcao.
// A cada 5 segundos, uma nova amostra e registrada.
// Os dados sao salvos na flash e restaurados ao ligar, entao os graficos
// continuam de onde pararam.
//
// 6 funcoes monitoradas:
//   [0] Loop completo   -> tempo total de um ciclo do loop()
//   [1] pegarClima()    -> tempo da requisicao HTTP de clima
//   [2] rtc.now()       -> tempo de leitura do relogio via I2C
//   [3] Botoes          -> tempo de verificar e fazer debounce dos botoes
//   [4] Alarmes         -> tempo de checar se algum alarme deve disparar
//   [5] Display OLED    -> tempo de redesenhar o display
//
// === HISTORICO DE 24 HORAS ===
// Para cumprir o requisito de persistencia minima de 24h:
//   288 amostras * 5 minutos = 1440 minutos = 24 horas
// Cada amostra ocupa 4 bytes (uint32_t).
// 7 funcoes * 288 amostras * 4 bytes = ~8KB de RAM (aceitavel no ESP32).
//
// 7 funcoes monitoradas:
//   [0] Loop completo     -> tempo total de um ciclo do loop()
//   [1] pegarClima()       -> tempo da requisicao HTTP de clima
//   [2] rtc.now()          -> tempo de leitura do relogio via I2C
//   [3] Botoes             -> tempo de verificar os botoes
//   [4] DHT Temperatura    -> valor da temperatura (x10, em inteiro)
//   [5] Display OLED       -> tempo de redesenhar o display
//   [6] DHT Umidade        -> valor da umidade (x10, em inteiro)
#define PERF_HIST_SIZE 60
#define PERF_FUNCOES 7
uint32_t perfHist[PERF_FUNCOES][PERF_HIST_SIZE]; // [funcao][amostra]
int      perfHistPos  = 0;                       // proxima posicao a escrever
bool     perfHistFull = false;                   // buffer ja completou a primeira volta?
unsigned long ultimaAmostraPerf = 0;
#define PERF_SAMPLE_MS 5000                      // 1 amostra a cada 5 segundos

// Salva histórico de performance na flash (binário)
void salvarPerfFlash() {
  File f = SPIFFS.open("/perf_hist.bin", FILE_WRITE);
  if (!f) { logPrintln("[PERF] Erro ao salvar!"); return; }
  f.write((uint8_t*)perfHist, sizeof(perfHist));
  f.write((uint8_t*)&perfHistPos, sizeof(perfHistPos));
  uint8_t fb = perfHistFull ? 1 : 0;
  f.write(&fb, 1);
  f.close();
  logPrintln("[PERF] Historico salvo na flash!");
}

// Carrega histórico de performance da flash
void carregarPerfFlash() {
  if (!SPIFFS.exists("/perf_hist.bin")) return;
  File f = SPIFFS.open("/perf_hist.bin", FILE_READ);
  if (!f) return;
  size_t expected = sizeof(perfHist) + sizeof(perfHistPos) + 1;
  if (f.size() == expected) {
    f.read((uint8_t*)perfHist, sizeof(perfHist));
    f.read((uint8_t*)&perfHistPos, sizeof(perfHistPos));
    uint8_t fb = 0; f.read(&fb, 1);
    perfHistFull = (fb == 1);
    if (perfHistPos < 0 || perfHistPos >= PERF_HIST_SIZE) perfHistPos = 0;
    logPrintln("[PERF] Historico restaurado da flash!");
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
String estadoAtual = "RELOGIO"; // Estado inicial: mostrando o relogio

// --- Controle do menu ---
int menuIndex       = 0; // Opcao selecionada no menu principal (0-2)
int alarmeListaIdx  = 0; // Alarme selecionado na lista (0-2)
int subMenuIndex    = 0; // Opcao dentro do detalhe de um alarme
int melodiaEscolha  = 0; // Melodia sendo escolhida (temporario)
int tempHora        = 0; // Hora temporaria durante edicao
int tempMinuto      = 0; // Minuto temporario durante edicao

// --- Estado do alarme ---
bool alarmeDisparo    = false; // True quando um alarme esta tocando
int  alarmeDisparoIdx = -1;    // Qual alarme disparou (-1 = nenhum)

// --- Pinos dos botoes ---
// INPUT_PULLUP: em estado solto o pino le HIGH (1).
// Quando pressionado, fecha o circuito com GND e le LOW (0).
int pinoButton1 = 4;   // Botao 1: navegar no menu
int pinoButton2 = 18;  // Botao 2: confirmar/selecionar

// --- Debounce: evita leituras falsas quando o botao "trepida" ---
// Guardamos o tempo do ultimo clique e so aceitamos novo apos 250ms.
unsigned long ultimoClique1 = 0;  // Momento do ultimo clique do botao 1
unsigned long ultimoClique2 = 0;  // Momento do ultimo clique do botao 2

// --- Rede e clima ---
bool  climaIniciado = false; // True apos primeiro fetch de clima
int32_t wifiRSSI    = 0;     // Intensidade do sinal WiFi em dBm

// --- Tempos de execucao das funcoes (atualizados a cada ciclo) ---
// uint32_t = inteiro de 32 bits sem sinal (0 a 4.294.967.295)
// Suficiente para guardar tempo em microsegundos por anos.
uint32_t tempoLoopUs            = 0; // Tempo total do loop()
uint32_t tempoPegarClimaUs      = 0; // Tempo do pegarClima()
uint32_t tempoPegarDHTUS        = 0; // Tempo do pegarClima()
uint32_t tempoRtcNowUs          = 0; // Tempo do rtc.now()
uint32_t tempoLeituraBotoesUs   = 0; // Tempo de leitura dos botoes
uint32_t tempoLogicaAlarmeUs    = 0; // Tempo da logica de alarmes
uint32_t tempoAtualizarDisplayUs = 0; // Tempo de atualizar o OLED
float    cpuUsoEstimado         = 0.0; // Estimativa de uso da CPU (%)
UBaseType_t stackLoopWords      = 0; // Stack livre da task principal

// --- Player de audio (I2S) ---
bool          i2sIniciado      = false; // True quando o I2S foi inicializado
int           melodiaAtualIdx  = 0;     // Qual melodia esta tocando (0-4)
int           notaAtualIdx     = 0;     // Qual nota dentro da melodia
unsigned long notaInicioMs     = 0;     // Quando a nota atual comecou
float         faseOnda         = 0.0;   // Fase da onda senoidal (para gerar som)

// Boot nao bloqueante: o setup so inicia a serial e o resto acontece no loop.
enum BootState {
  BOOT_SERIAL_INIT,
  BOOT_DISPLAY,
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
<div class="tpanel cgraf" id="tp3"><button class="sbtn" id="btnSalvar" onclick="forcaSalvar()">Forcar Salvamento na Flash</button><button class="sbtn" id="btnDownload" style="background:#065f46" onclick="baixarBackup()">Baixar Backup TXT</button><input type="file" id="arquivoBackup" accept=".txt" style="display:none" onchange="restaurarBackup(this)"><button class="sbtn" style="background:#7c3aed" onclick="document.getElementById('arquivoBackup').click()">Restaurar Backup TXT</button><div class="cgrid"><div class="ccard"><canvas id="pc0" height="115"></canvas></div><div class="ccard"><canvas id="pc1" height="115"></canvas></div><div class="ccard"><canvas id="pc2" height="115"></canvas></div><div class="ccard"><canvas id="pc3" height="115"></canvas></div><div class="ccard"><canvas id="pc4" height="115"></canvas></div><div class="ccard"><canvas id="pc5" height="115"></canvas></div><div class="ccard"><canvas id="pc6" height="115"></canvas></div></div></div>
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
function fixCanvas(c){c.width=c.parentElement.clientWidth-8||260;c.height=115;}

function drawChart(id,data,color,name,cur,label='us'){
  const c=document.getElementById(id);if(!c)return;
  fixCanvas(c);
  const ctx=c.getContext('2d'),w=c.width,h=c.height;
  ctx.fillStyle='#0d0d16';ctx.fillRect(0,0,w,h);
  ctx.strokeStyle='#1e2038';
  for(let i=0;i<4;i++){const y=30+(h-45)*i/3;ctx.beginPath();ctx.moveTo(0,y);ctx.lineTo(w,y);ctx.stroke();}
  ctx.fillStyle=color;ctx.font='bold 10px monospace';
  if(!data||data.length<2){ctx.fillText(name+': '+cur+label,4,14);return;}
  
  const dadosValidos=data.filter(v=>v>0);const mx=Math.max(...data,1),mn=dadosValidos.length>0?Math.min(...dadosValidos):0,sp=Math.max(mx-mn,1),yMax=mx+sp*0.3,yMin=Math.max(mn-sp*0.3,0),rng=yMax-yMin||1;
  
  const px=i=>w*i/(data.length-1),py=v=>h-15-(v-yMin)/rng*(h-45);
  const grad=ctx.createLinearGradient(0,0,0,h);grad.addColorStop(0,color+'44');grad.addColorStop(1,color+'08');
  ctx.fillStyle=grad;ctx.beginPath();
  data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));
  ctx.lineTo(w,h-15);ctx.lineTo(0,h-15);ctx.closePath();ctx.fill();
  ctx.strokeStyle=color;ctx.lineWidth=2;ctx.shadowColor=color;ctx.shadowBlur=10;
  ctx.beginPath();data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));ctx.stroke();
  ctx.shadowBlur=0;
  
  ctx.fillStyle=color;ctx.font='bold 10px monospace';
  ctx.fillText(name+': '+cur+label,4,12);
  
  ctx.font='9px monospace';
  let tN=new Date(),tS=new Date(tN.getTime()-300000);
  let sN=tN.toLocaleTimeString('pt-BR',{hour:'2-digit',minute:'2-digit'}),sS=tS.toLocaleTimeString('pt-BR',{hour:'2-digit',minute:'2-digit'});
  
  ctx.textAlign='left';
  ctx.fillText(sS,4,h-4);
  
  ctx.textAlign='right';
  ctx.fillText(sN,w-4,h-4);
  
  ctx.textAlign='center';
  // SE FOR NUMERO QUEBRADO, LIMITA EM 1 CASA DECIMAL NO TEXTO
  let tMx = (mx % 1 !== 0) ? mx.toFixed(1) : mx;
  let tMn = (mn % 1 !== 0) ? mn.toFixed(1) : mn;
  ctx.fillText('Maior:'+tMx+label+' Menor:'+tMn+label, w/2, h-4);
  
  ctx.textAlign='left';
}

async function gPerf(){try{const r=await fetch('/api/perf');const d=await r.json();for(let f=0;f<7;f++){const raw=d.hist[f],pos=d.pos,full=d.full;let ord=full?[...raw.slice(pos),...raw.slice(0,pos)]:raw.slice(0,pos);if(f===4||f===6)ord=ord.map(v=>v/10);const label=(f===4)?'°C':(f===6?'%':'us');const current=(f===4||f===6)?(d.atual[f]/10).toFixed(1):d.atual[f];drawChart('pc'+f,ord,PC[f],PN[f],current,label);}}catch(e){}}
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

// [TAG_CLIMA_API]
// === 11. CLIMA ===

void pegarClima() {
  uint64_t t0 = esp_timer_get_time(); // inicia o cronometro do clima
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + cidade
                 + "&appid=" + apiClima + "&lang=pt_br&units=metric";
    http.begin(url);
    int code = http.GET();
    if (code == 200) {
      JsonDocument doc;
      if (!deserializeJson(doc, http.getStream())) {
        temperatura = doc["main"]["temp"];
        Serial.print("Temp: "); Serial.println(temperatura);
      }
    }
    http.end();
  }
  tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0); // para o cronometro do clima
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
    "Previsao (API): %.1f C (%s)\n\n"
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
    temperatura, cidade.c_str(),
    (unsigned long)tempoLoopUs,
    cpuUsoEstimado,
    (unsigned long)ESP.getFreeHeap(),
    (unsigned long)ESP.getMinFreeHeap(),
    (unsigned long)(stackLoopWords * sizeof(StackType_t))
  );
  server.send(200, "text/plain", buf);
}

// GET /api/alarmes — JSON com alarmes + temp + hora + uptime + cidade
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
    horaStr, (unsigned long)(millis()/1000UL), cidade.c_str());
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
  json += tempoLoopUs;
  json += ',';
  json += tempoPegarClimaUs;    json += ',';
  json += tempoRtcNowUs;        json += ',';
  json += tempoLeituraBotoesUs;
  json += ',';
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
      int idx = perfHistFull ?
      (perfHistPos + i) % PERF_HIST_SIZE : i;
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
  if (ip >= 0) { int fim = corpo.indexOf('\n', ip+4);
  perfHistPos = corpo.substring(ip+4, fim).toInt(); }
  int ifl = corpo.indexOf("full:");
  if (ifl >= 0) { int fim = corpo.indexOf('\n', ifl+5);
  perfHistFull = corpo.substring(ifl+5, fim).toInt() == 1; }
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
      if (oledOkBoot) { displayTela.clearDisplay(); displayTela.setTextColor(SSD1306_WHITE);
      oledMsg("[1/8] Display", "OK"); }
      bootState = BOOT_SPIFFS; bootStepTime = agora; break;
    case BOOT_SPIFFS:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      SPIFFS.begin(true); carregarPerfFlash(); carregarWifiConfig(); carregarConfigParams();
      if (oledOkBoot) oledMsg("[2/8] SPIFFS", "OK");
      bootState = BOOT_RTC; bootStepTime = agora; break;
    case BOOT_RTC:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (rtc.begin() && rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      if (oledOkBoot) oledMsg("[3/8] RTC", "OK");
      bootState = BOOT_I2S; bootStepTime = agora; break;
    case BOOT_I2S:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      iniciarI2S();
      if (oledOkBoot) oledMsg("[4/8] Audio", i2sIniciado ? "OK" : "Falhou");
      bootState = BOOT_BOTOES; bootStepTime = agora; break;
    case BOOT_BOTOES:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      pinMode(pinoButton1, INPUT_PULLUP); pinMode(pinoButton2, INPUT_PULLUP);
      if (oledOkBoot) oledMsg("[5/8] Botoes", "OK");
      bootState = BOOT_WIFI_START; bootStepTime = agora; break;
    case BOOT_WIFI_START:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[6/8] WiFi", wifiSSID);
      logPrintf("[BOOT] Tentando WiFi: %s\n", wifiSSID);
      WiFi.mode(WIFI_STA); WiFi.setSleep(false); WiFi.setAutoReconnect(true); WiFi.begin(wifiSSID, wifiPassword);
      bootWifiT0 = agora; bootState = BOOT_WIFI_WAIT; break;
    case BOOT_WIFI_WAIT:
      if (WiFi.status() == WL_CONNECTED || (agora - bootWifiT0) >= WIFI_TIMEOUT_MS) { bootState = BOOT_WIFI_RESULT;
      bootStepTime = agora; }
      break;
    case BOOT_WIFI_RESULT:
      if (WiFi.status() == WL_CONNECTED) {
        modoAP = false;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char ipBuf[30];
          snprintf(ipBuf, sizeof(ipBuf), "IP: %s", WiFi.localIP().toString().c_str());
          oledMsg("[6/8] WiFi OK", ipBuf);
        }
        logPrintf("[BOOT] WiFi conectado: %s\n", WiFi.localIP().toString().c_str());
      } else {
        modoAP = true;
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP("RelogioConfig", "12345678");
        if (oledOkBoot) {
          char apBuf[30];
          snprintf(apBuf, sizeof(apBuf), "AP: %s", WiFi.softAPIP().toString().c_str());
          oledMsg("[6/8] MODO AP", apBuf);
        }
        logWarn("[BOOT] WiFi falhou. AP ativo: RelogioConfig / 12345678\n");
      }
      bootState = BOOT_NTP; bootStepTime = agora; break;
    case BOOT_NTP:
      if (agora - bootStepTime < BOOT_MSG_INTERVAL) break;
      if (oledOkBoot) oledMsg("[7/8] NTP", "Sincronizando");
      if (WiFi.status() == WL_CONNECTED) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        struct tm ti;
        if (getLocalTime(&ti, 5000)) {
          rtc.adjust(DateTime(ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec));
          if (oledOkBoot) oledMsg("[7/8] NTP", "OK");
        } else {
          if (oledOkBoot) oledMsg("[7/8] NTP", "Falhou");
        }
      } else {
        if (oledOkBoot) oledMsg("[7/8] NTP", "Sem WiFi");
      }
      bootState = BOOT_SERVER; bootStepTime = agora; break;
    case BOOT_SERVER:
      if (oledOkBoot) oledMsg("[8/8] Server", "Iniciando");
      iniciarServidorWeb(); bootState = BOOT_DHT; bootStepTime = agora; break;
    case BOOT_DHT:
      dht.begin();
      ultimoBackupFlash = millis();
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
  // Leitura periodica do DHT22 para temperatura/umidade locais.
  if (millis() - ultimoDhtLeitura >= cfgDhtIntervaloMs) {
    ultimoDhtLeitura = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      logPrintf("[DHT] Falha ao ler sensor DHT22 no GPIO %d\n", DHTPIN);
    } else {
      dhtHumidity = h;
      dhtTemperature = t;
      Serial.print("DHT22 Umidade: ");
      Serial.print(h);
      Serial.print(" % \t");
      Serial.print("Temperatura: ");
      Serial.print(t);
      Serial.println(" C");
    }
  }

  // ==================================================================
  // 14.0 SERVIDOR WEB
  // ==================================================================
  server.handleClient();
  
  // ==================================================================
  // 14.1 PLAYER DE MELODIA (não-bloqueante)
  // ==================================================================
  tocarMelodiaStep();
  
  // ==================================================================
  // 14.2 BOTÕES
  // ==================================================================
  uint64_t tBotoes0 = esp_timer_get_time();

  bool clicou1 = false;
  static bool est1 = HIGH;
  bool leitura1 = digitalRead(pinoButton1);
  if (est1 == HIGH && leitura1 == LOW && (agoraMs - ultimoClique1 > 250)) {
    clicou1 = true;
    ultimoClique1 = agoraMs;
    logPrintf("[BTN] Botao1 (Nav) | Estado: %s\n", estadoAtual.c_str());
  }
  est1 = leitura1;

  bool clicou2 = false;
  static bool est2 = HIGH;
  bool leitura2 = digitalRead(pinoButton2);
  if (est2 == HIGH && leitura2 == LOW && (agoraMs - ultimoClique2 > 250)) {
    clicou2 = true;
    ultimoClique2 = agoraMs;
    logPrintf("[BTN] Botao2 (Conf) | Estado: %s\n", estadoAtual.c_str());
  }
  est2 = leitura2;

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
      
      // IP no canto superior esquerdo
      displayTela.setTextSize(1);
      displayTela.setCursor(0, 0);
      if (wifiConectado) {
        displayTela.print(WiFi.localIP());
      } else {
        displayTela.print("AP:");
        displayTela.print(WiFi.softAPIP());
      }

      // Hora grande
      displayTela.setTextSize(4);
      displayTela.setCursor(4, 18);
      if (hora < 10) displayTela.print('0');
      displayTela.print(hora);
      displayTela.print(":");
      if (minuto < 10) displayTela.print("0");
      displayTela.print(minuto);

      displayTela.setTextSize(1);
      displayTela.setCursor(0, 55);
      if (alarmeDisparo) {
        displayTela.print(">>> ALARME ");
        displayTela.print(alarmeDisparoIdx + 1);
        displayTela.print(" <<<");
      } else {
        if (isnan(dhtTemperature)) {
          displayTela.print("T:--.-C ");
        } else {
          displayTela.print("T:");
          displayTela.print(dhtTemperature, 1);
          displayTela.print("C ");
        }
        if (isnan(dhtHumidity)) {
          displayTela.print("U:--% ");
        } else {
          displayTela.print("U:");
          displayTela.print((int)roundf(dhtHumidity));
          displayTela.print("%");
        }
      }

      displayTela.display();
    }
    tempoAtualizarDisplayUs = (uint32_t)(esp_timer_get_time() - tD0);

  // ---- MENU PRINCIPAL ----
  } else if (estadoAtual == "MENU") {
    if (clicou1) { menuIndex++;
      if (menuIndex >= 3) menuIndex = 0; }
    if (clicou2) {
      if (menuIndex == 0)      { estadoAtual = "LISTA_ALARMES";
      alarmeListaIdx = 0; }
      else if (menuIndex == 1) { estadoAtual = "HORA";
      tempHora = hora; tempMinuto = minuto; }
      else                     { estadoAtual = "RELOGIO";
      }
    }

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
    if (clicou1) { alarmeListaIdx++;
      if (alarmeListaIdx >= NUM_ALARMES + 1) alarmeListaIdx = 0; }
    if (clicou2) {
      if (alarmeListaIdx < NUM_ALARMES) {
        estadoAtual = "DETALHE_ALARME";
        subMenuIndex = 0;
      } else {
        estadoAtual = "MENU";
        // Sair -> volta ao menu
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
    if (clicou1) { subMenuIndex++;
      if (subMenuIndex >= 4) subMenuIndex = 0; }
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
    if (clicou1) { tempHora++;
      if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_ALARME_M";
    }

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
    if (clicou1) { tempMinuto++;
      if (tempMinuto > 59) tempMinuto = 0; }
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
    if (clicou1) { tempHora++;
      if (tempHora > 23) tempHora = 0; }
    if (clicou2) { estadoAtual = "EDITAR_HORA_M";
    }
    
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
    if (clicou1) { tempMinuto++;
      if (tempMinuto > 59) tempMinuto = 0; }
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
  // 14.6 METRICAS + BACKUP FLASH
  // ==================================================================
  tempoLoopUs = (uint32_t)(esp_timer_get_time() - tLoop0);
  cpuUsoEstimado = (tempoLoopUs / 50000.0f) * 100.0f;
  if (cpuUsoEstimado > 100.0f) cpuUsoEstimado = 100.0f;
  stackLoopWords = uxTaskGetStackHighWaterMark(nullptr);

  // Amostragem periódica de performance (1 ponto a cada 5s)
  if (millis() - ultimaAmostraPerf >= PERF_SAMPLE_MS) {
    ultimaAmostraPerf = millis();

    // IGNORAR GARGLOS INICIAIS: 
    // Só grava no histórico se o sistema estiver ligado a mais de 15 segundos (15000 ms) - NÃO USA DELAY()
    if (millis() > 15000) {
      // Ignora tempos iguais a zero (como o display ou clima caso não tenham rodado no loop exato)
      // Gravamos apenas se houver dado válido para não afundar o gráfico
      if (tempoLoopUs > 0) perfHist[0][perfHistPos] = tempoLoopUs;
      if (tempoPegarClimaUs > 0) perfHist[1][perfHistPos] = tempoPegarClimaUs;
      if (tempoRtcNowUs > 0) perfHist[2][perfHistPos] = tempoRtcNowUs;
      if (tempoLeituraBotoesUs > 0) perfHist[3][perfHistPos] = tempoLeituraBotoesUs;
      
      perfHist[4][perfHistPos] = (uint32_t)(isnan(dhtTemperature) ? 0 : dhtTemperature * 10.0f);
      
      if (tempoAtualizarDisplayUs > 0) perfHist[5][perfHistPos] = tempoAtualizarDisplayUs;
      
      perfHist[6][perfHistPos] = (uint32_t)(isnan(dhtHumidity) ? 0 : dhtHumidity * 10.0f);
      
      perfHistPos = (perfHistPos + 1) % PERF_HIST_SIZE;
      if (perfHistPos == 0) perfHistFull = true;
    }
  }

  // Backup na flash a cada 2 horas
  if (millis() - ultimoBackupFlash >= cfgBackupIntervaloMs) {
    salvarLogFlash();
    salvarPerfFlash();
    ultimoBackupFlash = millis();
  }
}