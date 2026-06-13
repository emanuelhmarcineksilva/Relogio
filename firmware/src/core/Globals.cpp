#include "ChronosObsidian.h"

// --- Credenciais ---
const char* WIFI_SSID_DEFAULT     = "Marcos";
const char* WIFI_PASSWORD_DEFAULT = "henrique";
const char* METEOSOURCE_API_KEY = "fju60uilyihby9vsdjbwffyb7pr46vq9pfauc9d3";
const char* METEOSOURCE_PLACE_ID = "curitiba";
const char* METEOSOURCE_URL = "https://www.meteosource.com/api/v1/free/point"
                               "?place_id=curitiba"
                               "&sections=daily"
                               "&timezone=auto"
                               "&language=en"
                               "&units=metric"
                               "&key=fju60uilyihby9vsdjbwffyb7pr46vq9pfauc9d3";
const char* ntpServer        = "pool.ntp.org";
const long  gmtOffset_sec    = -3 * 3600;
const int   daylightOffset_sec = 0;

// --- Hardware ---
RTC_DS3231 rtc;
Adafruit_SSD1306 displayTela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

// --- WiFi / clima ---
char wifiSSID[64]     = "Marcos";
char wifiPassword[64] = "henrique";
bool modoAP = false;
WeatherData climaAtual = {0, 0.0, 0.0, 0.0, false, 0};
float temperatura     = 0.0;
float dhtTemperature  = NAN;
float dhtHumidity     = NAN;
unsigned long ultimoDhtLeitura = 0;
bool  climaIniciado = false;
int32_t wifiRSSI    = 0;
unsigned long cfgDhtIntervaloMs   = 5000;
unsigned long cfgClimaIntervaloMs = 600000;
unsigned long cfgBackupIntervaloMs = 7200000;
char cfgClimaPlaceId[64] = "curitiba";

// --- Alarmes ---
Alarme alarmes[NUM_ALARMES] = {
  { 7,  0, true,  0, false},
  { 7, 30, false, 1, false},
  {12,  0, false, 2, false},
};

// --- Log ---
char logBuffer[LOG_BUF_SIZE];
int  logPos     = 0;
bool logWrapped = false;
unsigned long ultimoBackupFlash = 0;

// --- Performance ---
uint32_t perfHist[PERF_FUNCOES][PERF_HIST_SIZE];
int      perfHistPos  = 0;
bool     perfHistFull = false;
unsigned long ultimaAmostraPerf = 0;
uint32_t perfHistCRC = 0;

// --- UI / estado ---
String estadoAtual = "RELOGIO";
int menuIndex       = 0;
int alarmeListaIdx  = 0;
int subMenuIndex    = 0;
int melodiaEscolha  = 0;
int tempHora        = 0;
int tempMinuto      = 0;
bool alarmeDisparo    = false;
int  alarmeDisparoIdx = -1;
int pinoButton1 = PIN_BUTTON1;
int pinoButton2 = PIN_BUTTON2;

// --- Botões ---
QueueHandle_t buttonEventQueue = NULL;
esp_timer_handle_t btn1_debounce_timer = NULL;
esp_timer_handle_t btn2_debounce_timer = NULL;
volatile bool btn1_pending = false;
volatile bool btn2_pending = false;
volatile uint32_t btn1_bounce_time = 0;
volatile uint32_t btn2_bounce_time = 0;

// --- Energia ---
RTC_DATA_ATTR RtcPersistData rtc_data = {0, 0.0, 0.0, 0, 1};
unsigned long last_activity_ms = 0;
bool sleep_mode_active = false;

// --- Métricas tempo real ---
uint32_t tempoLoopUs            = 0;
uint32_t tempoPegarClimaUs      = 0;
uint32_t tempoPegarDHTUS        = 0;
uint32_t tempoRtcNowUs          = 0;
uint32_t tempoLeituraBotoesUs   = 0;
uint32_t tempoLogicaAlarmeUs    = 0;
uint32_t tempoAtualizarDisplayUs = 0;
float    cpuUsoEstimado         = 0.0;
UBaseType_t stackLoopWords      = 0;

// --- Áudio ---
bool          i2sIniciado      = false;
int           melodiaAtualIdx  = 0;
int           notaAtualIdx     = 0;
unsigned long notaInicioMs     = 0;
float         faseOnda         = 0.0;

// --- LCD ---
bool lcdOkBoot = false;
esp_timer_handle_t lcd_rotation_timer = NULL;
bool lcd_line2_show_ip = true;

// --- Mensagem clima ---
bool mostrarMsgClima = false;
unsigned long inicioMsgClima = 0;
char msgClima[30] = "";

// --- Boot ---
BootState bootState = BOOT_SERIAL_INIT;
bool bootFinalizado = false;
bool oledOkBoot = false;
unsigned long bootStepTime = 0;
unsigned long bootWifiT0 = 0;

// --- RTOS ---
QueueHandle_t sensorQueue = NULL;
TaskHandle_t taskAquisicaoHandle = NULL;
TaskHandle_t taskUIHandle = NULL;
TaskHandle_t taskAudioHandle = NULL;
TaskHandle_t taskPersistenciaHandle = NULL;
SensorData sensorDataCurrent = {};
unsigned long taskAquisicaoLastRun = 0;
unsigned long taskUILastRun = 0;
unsigned long taskAudioLastRun = 0;
unsigned long taskPersistenciaLastRun = 0;
