#pragma once
// Chronos Obsidian — header central (derivado de relogio7.ino v4.0.0)

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <driver/i2s.h>
#include <math.h>
#include <SPIFFS.h>
#include "time.h"
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_sleep.h>

#include "config/Version.h"
#include "config/Pins.h"
#include "config/Config.h"
#include "config/Credentials.h"
#include "core/models/WeatherData.h"
#include "core/models/Nota.h"
#include "core/models/Alarm.h"
#include "core/models/ButtonEvent.h"
#include "core/models/SensorData.h"
#include "sistema/boot/BootState.h"
#include "sistema/energia/SleepManager.h"

// --- Serviços e Sensores ---
#include "sensores/rtc_ds3231/RtcService.h"
#include "sensores/dht22/DhtService.h"
#include "servicos/alarmes/AlarmService.h"
#include "servicos/persistencia/ChangelogService.h"

// --- Objetos de hardware ---
extern RTC_DS3231 rtc;
extern Adafruit_SSD1306 displayTela;
extern LiquidCrystal_I2C lcd;
extern WebServer server;
extern DHT dht;

// --- WiFi / clima ---
extern char wifiSSID[64];
extern char wifiPassword[64];
extern bool modoAP;
extern float temperatura;
extern float dhtTemperature;
extern float dhtHumidity;
extern unsigned long ultimoDhtLeitura;
extern bool climaIniciado;
extern int32_t wifiRSSI;
extern unsigned long cfgDhtIntervaloMs;
extern unsigned long cfgClimaIntervaloMs;
extern unsigned long cfgBackupIntervaloMs;
extern char cfgClimaPlaceId[64];

// --- UI / estado ---
extern String estadoAtual;
extern int menuIndex;
extern int alarmeListaIdx;
extern int subMenuIndex;
extern int melodiaEscolha;
extern int tempHora;
extern int tempMinuto;
extern bool alarmeDisparo;
extern int alarmeDisparoIdx;
extern int pinoButton1;
extern int pinoButton2;

// --- Botões ISR ---
extern QueueHandle_t buttonEventQueue;
extern esp_timer_handle_t btn1_debounce_timer;
extern esp_timer_handle_t btn2_debounce_timer;
extern volatile bool btn1_pending;
extern volatile bool btn2_pending;
extern volatile uint32_t btn1_bounce_time;
extern volatile uint32_t btn2_bounce_time;

// --- Energia (RTC_DATA_ATTR — definido em Globals.cpp) ---
struct RtcPersistData {
  uint32_t boot_count;
  float last_temperature;
  float last_humidity;
  uint64_t last_wake_time_us;
  uint8_t rtc_data_version;
};
extern RtcPersistData rtc_data;
extern unsigned long last_activity_ms;
extern bool sleep_mode_active;

// --- Performance ---
extern uint32_t tempoLoopUs;
extern uint32_t tempoPegarClimaUs;
extern uint32_t tempoPegarDHTUS;
extern uint32_t tempoRtcNowUs;
extern uint32_t tempoLeituraBotoesUs;
extern uint32_t tempoLogicaAlarmeUs;
extern uint32_t tempoAtualizarDisplayUs;
extern float cpuUsoEstimado;
extern UBaseType_t stackLoopWords;
extern uint32_t perfHist[PERF_FUNCOES][PERF_HIST_SIZE];
extern int perfHistPos;
extern bool perfHistFull;
extern unsigned long ultimaAmostraPerf;
extern uint32_t perfHistCRC;

// --- Áudio ---
extern bool i2sIniciado;
extern int melodiaAtualIdx;
extern int notaAtualIdx;
extern unsigned long notaInicioMs;
extern float faseOnda;

// --- LCD ---
extern bool lcdOkBoot;
extern esp_timer_handle_t lcd_rotation_timer;
extern bool lcd_line2_show_ip;

// --- Mensagem clima ---
extern bool mostrarMsgClima;
extern unsigned long inicioMsgClima;
extern char msgClima[30];

// --- Log ---
extern char logBuffer[LOG_BUF_SIZE];
extern int logPos;
extern bool logWrapped;
extern unsigned long ultimoBackupFlash;

// --- RTOS ---
extern QueueHandle_t sensorQueue;
extern TaskHandle_t taskAquisicaoHandle;
extern TaskHandle_t taskUIHandle;
extern TaskHandle_t taskAudioHandle;
extern TaskHandle_t taskPersistenciaHandle;
extern SensorData sensorDataCurrent;
extern unsigned long taskAquisicaoLastRun;
extern unsigned long taskUILastRun;
extern unsigned long taskAudioLastRun;
extern unsigned long taskPersistenciaLastRun;

// --- Web assets ---
extern const char INDEX_HTML[] PROGMEM;

// --- Protótipos: observabilidade ---
void logPrint(const char* msg);
void logPrintln(const char* msg);
void logPrintf(const char* fmt, ...);
void logInfo(const char* fmt, ...);
void logWarn(const char* fmt, ...);
void logError(const char* fmt, ...);
String getLogContent();
uint32_t crc32_simple(uint32_t* data, size_t len);
void salvarPerfFlash();
void carregarPerfFlash();

// --- Protótipos: persistência ---
void salvarWifiConfig();
bool carregarWifiConfig();
void salvarConfigParams();
void carregarConfigParams();
void salvarLogFlash();
String carregarLogFlash();

// --- Protótipos: entradas ---
void IRAM_ATTR btn1_isr();
void IRAM_ATTR btn2_isr();
void btn1_debounce_callback(void* arg);
void btn2_debounce_callback(void* arg);

// --- Protótipos: energia ---
void enterLightSleep(uint32_t duration_ms);

// --- Protótipos: tasks ---
void TaskAquisicao(void* pvParameters);
void TaskUI(void* pvParameters);
void TaskAudio(void* pvParameters);
void TaskPersistencia(void* pvParameters);
void validarIntegridadeFlash();

// --- Protótipos: atuadores ---
void iniciarI2S();
void tocarMelodiaStep();
void oledMsg(const char* l1, const char* l2 = nullptr);
void iniciarLCD();
void atualizarLCD(DateTime agora, bool wifiConectado);

// --- Protótipos: ícones clima ---
void desenharNuvem();
void desenharSol();
void desenharTermometro();
void desenharNeve();
void desenharNuvemDeChuva();
int getTipoClima(float temp, int weather_code);

// --- Protótipos: serviços ---
void gerarMsgClima();
void pegarClima();

// --- Protótipos: web ---
const char* estadoToStr(String st);
void handleRoot();
void handleLog();
void handleAlarmes();
void handleSetHora();
void handleSetAlarme();
void handleToggleAlarme();
void handleClima();
void handleSerial();
void handleFlash();
void handleConfigGet();
void handleConfigSet();
void handleWifiSet();
void handlePerf();
void handleSalvar();
void handleDownloadPerf();
void handleUploadPerf();
void handleChangelog();
void iniciarServidorWeb();

// --- Protótipos: boot / loop ---
void setup();
bool bootStep();
void loop();
