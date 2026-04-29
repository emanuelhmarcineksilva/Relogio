////////////////////////////////////////////////////////////////////////////////
// Relogiov3 copy — VERSÃO COMPLETA
// WebServer síncrono + RTC + 3 Alarmes + I2S (MAX98357A) + Clima + OLED + Logs
// Boot estilo MVP1 (passo-a-passo no display)
////////////////////////////////////////////////////////////////////////////////
#pragma GCC optimize("O2")

// === 1. BIBLIOTECAS ===
#include <WiFi.h>              // Conexão WiFi
#include <WebServer.h>         // Servidor Web síncrono (estável no ESP32)
#include <HTTPClient.h>        // Requisições HTTP para o clima
#include <ArduinoJson.h>       // Parse JSON do clima
#include <Wire.h>              // I2C (OLED + RTC)
#include <Adafruit_SSD1306.h>  // Display OLED
#include <Adafruit_GFX.h>      // Gráficos
#include <RTClib.h>            // RTC DS3231
#include <esp_heap_caps.h>     // Info de memória
#include <esp_timer.h>         // Timer microsegundos
#include <driver/i2s.h>        // I2S nativo — para o MAX98357A
#include <math.h>              // sinf() para gerar ondas
#include <SPIFFS.h>            // Flash filesystem — logs persistentes
#include "time.h"              // NTP sync

// === 2. HARDWARE ===
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_ADDR    0x3C
#define SDA_PIN        21
#define SCL_PIN        22

// MAX98357A — amplificador I2S
#define I2S_BCLK       26     // Bit Clock
#define I2S_LRC        25     // Left/Right Clock (Word Select)
#define I2S_DIN        27     // Data In
#define I2S_PORT   I2S_NUM_0
#define SAMPLE_RATE 22050     // Taxa de amostragem (suficiente para melodias)

RTC_DS3231 rtc;
Adafruit_SSD1306 displayTela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WebServer server(80);

// === 3. CREDENCIAIS ===
const char* WIFI_SSID     = "Galaxy S9ae76"; // Nome do seu roteador - Galaxy S9ae76
const char* WIFI_PASSWORD = "vgkb2019"; // Senha do seu WiFi - vgkb2019
String apiClima = "76b19c5ce07af87452662d52bd95ce4f";
String cidade   = "Curitiba";

float temperatura = 0.0;

// === 4. NOTAS MUSICAIS (frequências em Hz) ===
#define N_C4  262
#define N_D4  294
#define N_E4  330
#define N_F4  349
#define N_G4  392
#define N_A4  440
#define N_B4  494
#define N_C5  523
#define N_D5  587
#define N_Ds5 622
#define N_E5  659
#define N_F5  698
#define N_G5  784
#define N_A5  880
#define N_C6  1047
#define N_SIL 0

// Estrutura de uma nota musical
struct Nota {
  uint16_t freq;   // Hz (0 = silêncio)
  uint16_t durMs;  // duração em ms
};

// === MELODIA 0: Aurora (acordar suave — arpejo ascendente) ===
const Nota melodia_aurora[] PROGMEM = {
  {N_C4,400},{N_SIL,50},{N_E4,400},{N_SIL,50},{N_G4,400},{N_SIL,50},
  {N_C5,500},{N_SIL,80},{N_E5,500},{N_SIL,80},{N_G5,600},{N_SIL,100},
  {N_C6,800},{N_SIL,300},
  {N_G5,300},{N_SIL,50},{N_E5,300},{N_SIL,50},{N_C5,400},{N_SIL,200},
  {0,0} // FIM
};

// === MELODIA 1: Sino de Cristal (Westminster chime) ===
const Nota melodia_sino[] PROGMEM = {
  {N_E5,500},{N_SIL,50},{N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_G4,1000},{N_SIL,200},
  {N_C5,500},{N_SIL,50},{N_D5,500},{N_SIL,50},{N_E5,500},{N_SIL,50},{N_C5,1000},{N_SIL,300},
  {0,0}
};

// === MELODIA 2: Despertar Suave (acordes suaves) ===
const Nota melodia_despertar[] PROGMEM = {
  {N_A4,600},{N_SIL,100},{N_C5,600},{N_SIL,100},{N_E5,800},{N_SIL,200},
  {N_D5,600},{N_SIL,100},{N_F5,600},{N_SIL,100},{N_A5,800},{N_SIL,300},
  {N_E5,400},{N_SIL,80},{N_C5,400},{N_SIL,80},{N_A4,600},{N_SIL,400},
  {0,0}
};

// === MELODIA 3: Für Elise (abertura do Beethoven) ===
const Nota melodia_furelise[] PROGMEM = {
  {N_E5,250},{N_Ds5,250},{N_E5,250},{N_Ds5,250},{N_E5,250},
  {N_B4,250},{N_D5,250},{N_C5,250},{N_A4,500},{N_SIL,100},
  {N_C4,250},{N_E4,250},{N_A4,250},{N_B4,500},{N_SIL,100},
  {N_E4,250},{N_G4,250},{N_B4,250},{N_C5,500},{N_SIL,200},
  {0,0}
};

// === MELODIA 4: Estrelinha (Twinkle Twinkle) ===
const Nota melodia_estrela[] PROGMEM = {
  {N_C4,300},{N_C4,300},{N_G4,300},{N_G4,300},{N_A4,300},{N_A4,300},{N_G4,500},{N_SIL,100},
  {N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,300},{N_D4,300},{N_C4,500},{N_SIL,200},
  {N_G4,300},{N_G4,300},{N_F4,300},{N_F4,300},{N_E4,300},{N_E4,300},{N_D4,500},{N_SIL,200},
  {0,0}
};

// Array de ponteiros para as melodias
const Nota* const MELODIAS[] = {
  melodia_aurora, melodia_sino, melodia_despertar, melodia_furelise, melodia_estrela
};
const int TOTAL_MELODIAS = 5;

// Nomes (para display e web)
const char* MELODIA_NOMES[]  = {"Aurora", "Sino", "Despertar", "Fur Elise", "Estrela"};
const char* MELODIA_CURTO[]  = {"Aur", "Sin", "Des", "Fur", "Est"}; // Abreviado pro OLED

// === 5. ESTRUTURA DOS ALARMES (3 alarmes editáveis) ===
struct Alarme {
  uint8_t hora;
  uint8_t minuto;
  bool    ativo;
  uint8_t melodia;   // índice 0-4
  bool    jaTocou;   // trava para não repetir no mesmo minuto
};

#define NUM_ALARMES 3
Alarme alarmes[NUM_ALARMES] = {
  { 7,  0, true,  0, false},  // Alarme 1: 07:00, Aurora
  { 7, 30, false, 1, false},  // Alarme 2: 07:30, Sino
  {12,  0, false, 2, false},  // Alarme 3: 12:00, Despertar
};

// === 5B. SISTEMA DE LOGS ===
// Ring buffer para log serial (captura tudo)
#define LOG_BUF_SIZE 4000
char logBuffer[LOG_BUF_SIZE];
int logPos = 0;
bool logWrapped = false;

// NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600; // GMT-3 (Brasília)
const int daylightOffset_sec = 0;
unsigned long ultimoBackupFlash = 0;
#define BACKUP_INTERVAL_MS 7200000UL // 2 horas

// Função para adicionar texto ao log buffer
void logPrint(const char* msg) {
  Serial.print(msg); // Continua imprimindo no Serial real
  int len = strlen(msg);
  for (int i = 0; i < len; i++) {
    logBuffer[logPos] = msg[i];
    logPos++;
    if (logPos >= LOG_BUF_SIZE) {
      logPos = 0;
      logWrapped = true;
    }
  }
}

void logPrintln(const char* msg) {
  logPrint(msg);
  logPrint("\n");
}

void logPrintf(const char* fmt, ...) {
  char tmp[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint(tmp);
}

// Retorna o conteúdo do log buffer como String
String getLogContent() {
  String result;
  result.reserve(LOG_BUF_SIZE);
  if (logWrapped) {
    for (int i = logPos; i < LOG_BUF_SIZE; i++) result += logBuffer[i];
  }
  for (int i = 0; i < logPos; i++) result += logBuffer[i];
  return result;
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

// === 6. ESTADO GLOBAL ===
String estadoAtual = "RELOGIO";

int menuIndex       = 0;   // Menu principal (0=Alarmes, 1=Hora, 2=Sair)
int alarmeListaIdx  = 0;   // Índice na lista de alarmes (0-3)
int subMenuIndex    = 0;   // Submenu do alarme (0=Status, 1=Editar, 2=Melodia, 3=Voltar)
int melodiaEscolha  = 0;   // Índice temporário ao escolher melodia
int tempHora        = 0;   // Hora temporária durante edição
int tempMinuto      = 0;   // Minuto temporário durante edição

// Alarme disparando
bool alarmeDisparo    = false;
int  alarmeDisparoIdx = -1;  // Qual alarme disparou

// Pinos (botões)
int pinoButton1 = 4;    // Navegar
int pinoButton2 = 18;   // Confirmar

// Debounce
unsigned long ultimoClique1 = 0;
unsigned long ultimoClique2 = 0;

// Clima
bool  climaIniciado = false;


// WiFi
int32_t wifiRSSI = 0;

// Métricas de performance
volatile uint32_t tempoLoopUs            = 0;
volatile uint32_t tempoPegarClimaUs      = 0;
volatile uint32_t tempoRtcNowUs          = 0;
volatile uint32_t tempoLeituraBotoesUs   = 0;
volatile uint32_t tempoLogicaAlarmeUs    = 0;
volatile uint32_t tempoAtualizarDisplayUs = 0;
volatile float    cpuUsoEstimado         = 0.0f;
volatile UBaseType_t stackLoopWords      = 0;

// === 7. ESTADO DO PLAYER DE MELODIA (I2S) ===
int     melodiaAtualIdx  = 0;     // Qual melodia está tocando
int     notaAtualIdx     = 0;     // Qual nota da melodia
unsigned long notaInicioMs = 0;   // Quando a nota atual começou
float   faseOnda         = 0.0f;  // Fase do oscilador senoidal
bool    i2sIniciado      = false;

// === 8. HTML DA PÁGINA WEB (PROGMEM — não usa RAM) ===
static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Relogio Inteligente</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:'Courier New',monospace;background:#0a0a0f;color:#e0e0e0;padding:12px;max-width:620px;margin:0 auto}
h1{color:#38bdf8;font-size:1.3rem;margin-bottom:10px;text-align:center}
h2{color:#94a3b8;font-size:1rem;margin:14px 0 6px;border-bottom:1px solid #1a1a2e;padding-bottom:4px}
.clock{text-align:center;font-size:4rem;font-weight:bold;color:#4ade80;background:#111118;border:1px solid #1a1a2e;border-radius:12px;padding:16px 0 10px;margin:10px 0;letter-spacing:4px;text-shadow:0 0 20px #4ade8055}
.clock-sub{text-align:center;color:#64748b;font-size:0.85rem;margin-bottom:8px}
.card{background:#111118;border:1px solid #1a1a2e;border-radius:8px;padding:10px;margin:6px 0}
input[type=number]{width:48px;background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;text-align:center;font-family:inherit}
select{background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;font-family:inherit}
button{background:#1e40af;color:#fff;border:none;padding:5px 12px;border-radius:5px;cursor:pointer;margin:3px;font-size:0.85rem}
button:hover{background:#2563eb}
.on{background:#16a34a}.on:hover{background:#15803d}
.off{background:#dc2626}.off:hover{background:#b91c1c}
.row{display:flex;align-items:center;gap:6px;flex-wrap:wrap;margin:3px 0}
.lbl{color:#64748b;min-width:65px;font-size:0.85rem}
.sep{color:#555}
.tabs{display:flex;gap:4px;margin:10px 0}
.tab{flex:1;padding:8px;text-align:center;background:#111118;border:1px solid #1a1a2e;border-radius:6px 6px 0 0;cursor:pointer;color:#64748b;font-size:0.85rem}
.tab.active{background:#1a1a2e;color:#4ade80;border-bottom:2px solid #4ade80}
.tpanel{display:none;background:#0a0a0f;border:1px solid #1a1a2e;border-radius:0 0 6px 6px;padding:10px;min-height:200px;max-height:300px;overflow-y:auto;font-size:0.78rem;line-height:1.5;white-space:pre-wrap;color:#4ade80}
.tpanel.active{display:block}
.exit-btn{display:block;width:100%;padding:10px;background:#dc2626;color:#fff;border:none;border-radius:8px;cursor:pointer;font-size:1rem;margin:12px 0;font-family:inherit}
.exit-btn:hover{background:#b91c1c}
.sethora{display:flex;align-items:center;gap:6px;justify-content:center;margin:6px 0}
</style>
</head>
<body>
<h1>Relogio Inteligente</h1>

<div class="clock" id="clk">--:--:--</div>
<div class="clock-sub"><span id="tmp">--</span> | <span id="upt">--</span></div>

<h2>Hora do Sistema</h2>
<div class="card"><div class="sethora">
<input type="number" id="sh" min="0" max="23" value="0"><span class="sep">:</span>
<input type="number" id="sm" min="0" max="59" value="0">
<button onclick="setH()">Salvar Hora</button>
</div></div>

<h2>Alarmes</h2>
<div id="als"></div>

<h2>Clima</h2>
<div class="card"><div class="row">
<span id="tmpC" class="lbl">--</span>
<button onclick="doClima()">Atualizar Clima</button>
</div></div>

<h2>Logs</h2>
<div class="tabs">
<div class="tab active" onclick="stab(0)">Status</div>
<div class="tab" onclick="stab(1)">Serial</div>
<div class="tab" onclick="stab(2)">Flash</div>
</div>
<div class="tpanel active" id="tp0">carregando...</div>
<div class="tpanel" id="tp1">carregando serial...</div>
<div class="tpanel" id="tp2">carregando flash...</div>

<button class="exit-btn" onclick="window.location.href='/'">&#128281; Voltar / Atualizar</button>

<script>
const M=['Aurora','Sino','Despertar','Fur Elise','Estrela'];
function stab(n){
  document.querySelectorAll('.tab').forEach((t,i)=>{t.classList.toggle('active',i==n)});
  document.querySelectorAll('.tpanel').forEach((p,i)=>{p.classList.toggle('active',i==n)});
  if(n==2) gF();
}
function rA(d){
  let h='';
  for(let i=0;i<3;i++){
    const a=d.alarmes[i];
    h+='<div class="card"><div class="row">';
    h+='<span class="lbl">Alm '+(i+1)+'</span>';
    h+='<input type="number" id="ah'+i+'" min="0" max="23" value="'+a.h+'">';
    h+='<span class="sep">:</span>';
    h+='<input type="number" id="am'+i+'" min="0" max="59" value="'+a.m+'">';
    h+='<select id="ml'+i+'">';
    for(let j=0;j<M.length;j++) h+='<option value="'+j+'"'+(j==a.mel?' selected':'')+'>'+M[j]+'</option>';
    h+='</select>';
    h+='<button class="'+(a.a?'on':'off')+'" onclick="tg('+i+')">'+(a.a?'ON':'OFF')+'</button>';
    h+='<button onclick="sv('+i+')">Salvar</button>';
    h+='</div></div>';
  }
  document.getElementById('als').innerHTML=h;
  document.getElementById('tmp').textContent='Temp: '+d.temp.toFixed(1)+'\u00B0C';
  document.getElementById('tmpC').textContent='Temp: '+d.temp.toFixed(1)+'\u00B0C - '+d.cidade;
}
async function gS(){try{const r=await fetch('/api/log');document.getElementById('tp0').textContent=await r.text()}catch(e){}}
async function gSer(){try{const r=await fetch('/api/serial');const el=document.getElementById('tp1');el.textContent=await r.text();el.scrollTop=el.scrollHeight}catch(e){}}
async function gF(){try{const r=await fetch('/api/flash');document.getElementById('tp2').textContent=await r.text()}catch(e){}}
async function gA(){try{const r=await fetch('/api/alarmes');const d=await r.json();rA(d);document.getElementById('clk').textContent=d.hora;document.getElementById('upt').textContent='Up: '+d.uptime+'s'}catch(e){}}
async function setH(){const h=document.getElementById('sh').value,m=document.getElementById('sm').value;await fetch('/api/hora?h='+h+'&m='+m);gS()}
async function sv(i){const h=document.getElementById('ah'+i).value,m=document.getElementById('am'+i).value,ml=document.getElementById('ml'+i).value;await fetch('/api/alarme?id='+i+'&h='+h+'&m='+m+'&mel='+ml);gA();gS()}
async function tg(i){await fetch('/api/alarme/toggle?id='+i);gA();gS()}
async function doClima(){await fetch('/api/clima');gA();gS()}
gS();gA();gSer();setInterval(gS,3000);setInterval(gA,2000);setInterval(gSer,2000);
</script>
</body>
</html>
)rawliteral";

// === 9. I2S — INICIALIZAÇÃO E PLAYER DE MELODIA ===

void iniciarI2S() {
  // Configura o periférico I2S para mandar áudio ao MAX98357A
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  cfg.sample_rate = SAMPLE_RATE;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;  // MAX98357A é mono
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  cfg.dma_buf_count = 8;
  cfg.dma_buf_len = 64;
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
  if (!i2sIniciado) return;

  // Se não está disparando, silencia e sai
  if (!alarmeDisparo) {
    if (notaAtualIdx != 0 || faseOnda != 0) {
      i2s_zero_dma_buffer(I2S_PORT);
      notaAtualIdx = 0;
      faseOnda = 0;
    }
    return;
  }

  // Lê a nota atual da melodia (da PROGMEM)
  const Nota* mel = MELODIAS[melodiaAtualIdx];
  Nota nota;
  memcpy_P(&nota, &mel[notaAtualIdx], sizeof(Nota));

  // Verifica se a nota acabou (duração expirou)
  if (millis() - notaInicioMs >= nota.durMs) {
    notaAtualIdx++;

    // Verifica fim da melodia (freq=0, dur=0) → volta pro início (loop)
    Nota proxima;
    memcpy_P(&proxima, &mel[notaAtualIdx], sizeof(Nota));
    if (proxima.freq == 0 && proxima.durMs == 0) {
      notaAtualIdx = 0;
    }

    notaInicioMs = millis();
    memcpy_P(&nota, &mel[notaAtualIdx], sizeof(Nota));
    faseOnda = 0; // Reseta fase ao mudar de nota
  }

  // Gera 128 amostras da nota atual
  int16_t samples[128];

  if (nota.freq > 0) {
    // Onda senoidal com a frequência da nota
    float incremento = 2.0f * PI * nota.freq / (float)SAMPLE_RATE;
    for (int i = 0; i < 128; i++) {
      samples[i] = (int16_t)(sinf(faseOnda) * 8000); // Volume moderado
      faseOnda += incremento;
      if (faseOnda >= 2.0f * PI) faseOnda -= 2.0f * PI;
    }
  } else {
    // Silêncio entre notas
    memset(samples, 0, sizeof(samples));
  }

  // Envia ao I2S (timeout curto de 5ms para não bloquear o loop)
  size_t written;
  i2s_write(I2S_PORT, samples, sizeof(samples), &written, 5 / portTICK_PERIOD_MS);
}

// === 10. DISPLAY — FUNÇÕES AUXILIARES ===

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

// === 11. CLIMA ===

void pegarClima() {
  uint64_t t0 = esp_timer_get_time();
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
  tempoPegarClimaUs = (uint32_t)(esp_timer_get_time() - t0);
}

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
    "Temp: %.1f C (%s)\n\n"
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
  char json[500];
  char* p = json;
  p += sprintf(p, "{\"alarmes\":[");
  for (int i = 0; i < NUM_ALARMES; i++) {
    if (i > 0) p += sprintf(p, ",");
    p += sprintf(p, "{\"h\":%d,\"m\":%d,\"a\":%s,\"mel\":%d}",
      alarmes[i].hora, alarmes[i].minuto,
      alarmes[i].ativo ? "true" : "false",
      alarmes[i].melodia);
  }
  p += sprintf(p, "],\"temp\":%.1f,\"hora\":\"%s\",\"uptime\":%lu,\"cidade\":\"%s\"}",
    temperatura, horaStr, (unsigned long)(millis()/1000UL), cidade.c_str());
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
  server.begin();
  logPrintln("Servidor Web Iniciado!");
}

// === 13. SETUP — BOOT PASSO-A-PASSO ===

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F(" Relogiov3 — BOOT PASSO-A-PASSO (8)"));
  Serial.println(F("========================================"));

  // ---- PASSO 1/8: DISPLAY ----
  Serial.print(F("[1/8] Display OLED... "));
  Wire.begin(SDA_PIN, SCL_PIN);
  bool oledOk = displayTela.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  if (!oledOk) { Serial.println(F("FALHOU!")); }
  else {
    Serial.println(F("OK"));
    displayTela.clearDisplay();
    displayTela.setTextColor(SSD1306_WHITE);
    oledMsg("[1/8] Display", "OK!");
    delay(500);
  }

  // ---- PASSO 2/8: SPIFFS ----
  Serial.print(F("[2/8] SPIFFS... "));
  if (oledOk) oledMsg("[2/8] SPIFFS...", "");
  if (!SPIFFS.begin(true)) {
    Serial.println(F("FALHOU!"));
    if (oledOk) oledMsg("[2/8] SPIFFS", "FALHOU!");
  } else {
    Serial.println(F("OK"));
    if (oledOk) oledMsg("[2/8] SPIFFS", "OK!");
  }
  delay(300);

  // ---- PASSO 3/8: RTC ----
  Serial.print(F("[3/8] RTC DS3231... "));
  if (oledOk) oledMsg("[3/8] RTC...", "");
  if (!rtc.begin()) {
    Serial.println(F("FALHOU!"));
    if (oledOk) oledMsg("[3/8] RTC", "FALHOU!");
  } else {
    if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    DateTime now = rtc.now();
    Serial.printf("OK (%02d:%02d:%02d)\n", now.hour(), now.minute(), now.second());
    if (oledOk) {
      char buf[20];
      snprintf(buf, sizeof(buf), "OK  %02d:%02d:%02d", now.hour(), now.minute(), now.second());
      oledMsg("[3/8] RTC", buf);
    }
  }
  delay(500);

  // ---- PASSO 4/8: I2S (MAX98357A) ----
  Serial.print(F("[4/8] I2S Audio... "));
  if (oledOk) oledMsg("[4/8] Audio I2S...", "MAX98357A");
  iniciarI2S();
  if (oledOk) oledMsg("[4/8] Audio", i2sIniciado ? "OK!" : "FALHOU");
  delay(500);

  // ---- PASSO 5/8: BOTÕES ----
  Serial.println(F("[5/8] Botoes..."));
  pinMode(pinoButton1, INPUT_PULLUP);
  pinMode(pinoButton2, INPUT_PULLUP);
  if (oledOk) oledMsg("[5/8] Botoes", "OK!");
  delay(300);

  // ---- PASSO 6/8: WIFI ----
  Serial.print(F("[6/8] WiFi... "));
  if (oledOk) oledMsg("[6/8] WiFi...", WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - t0) < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi OK! IP: ");
    Serial.println(WiFi.localIP());
    if (oledOk) {
      char ipBuf[30];
      snprintf(ipBuf, sizeof(ipBuf), "IP: %s", WiFi.localIP().toString().c_str());
      oledMsg("[6/8] WiFi OK!", ipBuf);
    }
  } else {
    Serial.println("WiFi FALHOU (retry no loop)");
    if (oledOk) oledMsg("[6/8] WiFi", "FALHOU - retry");
  }
  delay(800);

  // ---- PASSO 7/8: NTP (sincroniza hora real) ----
  Serial.print(F("[7/8] NTP Sync... "));
  if (oledOk) oledMsg("[7/8] NTP...", "Sincronizando");
  if (WiFi.status() == WL_CONNECTED) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo, 5000)) {
      // Sincroniza o RTC com hora da internet
      rtc.adjust(DateTime(
        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec
      ));
      Serial.printf("OK (%02d:%02d:%02d)\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      if (oledOk) {
        char buf[20];
        snprintf(buf, sizeof(buf), "OK %02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        oledMsg("[7/8] NTP", buf);
      }
    } else {
      Serial.println(F("FALHOU (usando RTC)"));
      if (oledOk) oledMsg("[7/8] NTP", "FALHOU-usa RTC");
    }
  } else {
    Serial.println(F("Sem WiFi"));
    if (oledOk) oledMsg("[7/8] NTP", "Sem WiFi");
  }
  delay(500);

  // ---- PASSO 8/8: WEB SERVER ----
  Serial.print(F("[8/8] Web Server... "));
  if (oledOk) oledMsg("[8/8] Server...", "Iniciando...");

  iniciarServidorWeb();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("URL: http://");
    Serial.println(WiFi.localIP());
    if (oledOk) {
      char urlBuf[40];
      snprintf(urlBuf, sizeof(urlBuf), "http://%s", WiFi.localIP().toString().c_str());
      oledMsg("[8/8] Server OK!", urlBuf);
    }
  } else {
    if (oledOk) oledMsg("[8/8] Server OK!", "Sem WiFi...");
  }
  delay(1000);

  // BOOT COMPLETO
  logPrintln("====================================");
  logPrintf(" BOOT COMPLETO | Heap: %lu\n", (unsigned long)ESP.getFreeHeap());
  logPrintln("====================================");
  ultimoBackupFlash = millis();
}

// === 14. LOOP PRINCIPAL ===

void loop() {
  uint64_t tLoop0 = esp_timer_get_time();
  unsigned long agoraMs = millis();

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
    // Clima a cada 10 min
    static unsigned long tempoClima = 0;
    if (millis() - tempoClima > 600000 && wifiConectado) {
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
        displayTela.print("WiFi...");
      }

      // Hora grande
      displayTela.setTextSize(4);
      displayTela.setCursor(4, 18);
      if (hora < 10) displayTela.print('0');
      displayTela.print(hora);
      displayTela.print(":");
      if (minuto < 10) displayTela.print("0");
      displayTela.print(minuto);

      // Barra inferior: temp + alarme ativo
      displayTela.setTextSize(1);
      displayTela.setCursor(0, 55);
      if (alarmeDisparo) {
        displayTela.print(">>> ALARME ");
        displayTela.print(alarmeDisparoIdx + 1);
        displayTela.print(" <<<");
      } else {
        displayTela.print(temperatura, 1);
        displayTela.print("C ");
        displayTela.print(cidade);
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

  // ==================================================================
  // 14.6 MÉTRICAS + BACKUP FLASH
  // ==================================================================
  tempoLoopUs = (uint32_t)(esp_timer_get_time() - tLoop0);
  cpuUsoEstimado = (tempoLoopUs / 50000.0f) * 100.0f;
  if (cpuUsoEstimado > 100.0f) cpuUsoEstimado = 100.0f;
  stackLoopWords = uxTaskGetStackHighWaterMark(nullptr);

  // Backup na flash a cada 2 horas
  if (millis() - ultimoBackupFlash >= BACKUP_INTERVAL_MS) {
    salvarLogFlash();
    ultimoBackupFlash = millis();
  }
}