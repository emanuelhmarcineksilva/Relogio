# Chronos Obsidian — Estrutura Modular do Firmware

Estrutura alinhada ao documento **NossoProjeto.text** (cap. 5–7) e ao código `relogio7.ino` v4.0.0.  
Pastas vazias — prontas para receber o código modular.

## Camadas (NossoProjeto §6.1)

```
┌─────────────────────────────────────────────────────────┐
│  interface/          Web embarcada + UI local (OLED)    │
├─────────────────────────────────────────────────────────┤
│  servicos/           Alarmes, clima API, rede, SPIFFS   │
├─────────────────────────────────────────────────────────┤
│  observabilidade/    Logs + métricas de performance     │
├─────────────────────────────────────────────────────────┤
│  sistema/            Boot, tasks RTOS, energia           │
├─────────────────────────────────────────────────────────┤
│  sensores/           DHT22, RTC DS3231                  │
│  entradas/           Botões GPIO (ISR + debounce)        │
│  atuadores/          OLED, LCD, áudio I2S               │
├─────────────────────────────────────────────────────────┤
│  config/ + core/     Pinos, versão, structs globais      │
└─────────────────────────────────────────────────────────┘
```

## Árvore completa

```
ChronosObsidian/
└── firmware/
    ├── include/                          ← cabeçalhos (.h)
    │   ├── config/                       ← pinos, versão, credenciais
    │   ├── core/models/                  ← Alarme, Nota, WeatherData, SensorData…
    │   ├── sensores/
    │   │   ├── dht22/                    ← temperatura + umidade local
    │   │   └── rtc_ds3231/               ← relógio de tempo real + NTP
    │   ├── entradas/
    │   │   └── botoes/                   ← GPIO4/18, ISR, fila de eventos
    │   ├── atuadores/
    │   │   ├── oled/                     ← driver SSD1306 (oledMsg)
    │   │   ├── lcd/                      ← driver LCD 16x2 + rotação 5s
    │   │   └── audio_i2s/                ← MAX98357A + melodias PROGMEM
    │   ├── servicos/
    │   │   ├── alarmes/                  ← 3 alarmes, disparo, silenciamento
    │   │   ├── clima/                    ← API Meteosource (pegarClima)
    │   │   ├── rede/                     ← WiFi STA/AP, NTP, modoAP
    │   │   └── persistencia/             ← SPIFFS: wifi, config, log, perf
    │   ├── interface/
    │   │   ├── oled_ui/                  ← máquina de estados (menus OLED)
    │   │   │   └── icones_clima/         ← sol, nuvem, chuva, neve, termômetro
    │   │   └── web/
    │   │       ├── assets/               ← INDEX_HTML PROGMEM
    │   │       └── rotas/                ← handleRoot, handlePerf, /api/*
    │   ├── observabilidade/
    │   │   ├── logging/                  ← ring buffer 4KB, INFO/WARN/ERROR
    │   │   └── performance/              ← perfHist, CRC, 7 séries, µs
    │   └── sistema/
    │       ├── boot/                     ← bootStep, enum BootState (9 etapas)
    │       ├── tasks/                    ← TaskAquisicao (C0), TaskUI (C1)
    │       └── energia/                  ← Light Sleep, RTC_DATA_ATTR
    ├── src/                              ← implementações (.cpp)
    │   └── (espelha include/)
    ├── tests/                            ← cap. 7.1 do documento
    │   ├── sensores/                     ← 7.1.1 RTC + DHT22
    │   ├── atuadores/                    ← 7.1.2 OLED + LCD + I2S
    │   └── software/                   ← 7.1.3 web + persistência + log
    ├── platformio.ini                    ← (a criar)
    └── ChronosObsidian.ino               ← (a criar — setup + loop)
```

## Mapa detalhado: relogio7.ino → pasta

| Pasta | Código em relogio7.ino | Requisito (NossoProjeto) |
|-------|------------------------|--------------------------|
| **config/** | PARTE 2–3: pinos, `FIRMWARE_VERSION`, `cfg*`, credenciais WiFi/API | RH01–RH04, §6.2 |
| **core/models/** | `Alarme`, `Nota`, `WeatherData`, `SensorData`, `ButtonEvent` | §6.2 estruturas |
| **sensores/dht22/** | `DHT dht`, leitura na `TaskAquisicao`, `dhtTemperature` | RH02, §7.1.1 |
| **sensores/rtc_ds3231/** | `RTC_DS3231 rtc`, `rtc.now()`, ajuste NTP/web | RH02, §7.1.1 |
| **entradas/botoes/** | FASE 2.2: ISR, debounce `esp_timer`, `buttonEventQueue` | §6.4, GPIO4/18 |
| **atuadores/oled/** | `displayTela`, `oledMsg`, boot display | RH03, §7.1.2 |
| **atuadores/lcd/** | `LiquidCrystal_I2C`, `atualizarLCD`, timer rotação 5s | RH03, §7.1.2 |
| **atuadores/audio_i2s/** | PARTE 4 notas, `iniciarI2S`, `tocarMelodiaStep` | RH03, §7.1.2 |
| **servicos/alarmes/** | PARTE 5, lógica §14.4 loop, `gerarMsgClima` | RF05, §7.2.1 |
| **servicos/clima/** | FASE 1.1: `pegarClima`, `WeatherData`, Meteosource | RF05, objetivo (a) |
| **servicos/rede/** | WiFi persistente, boot WiFi/NTP, `modoAP` | RF05, §6.7 |
| **servicos/persistencia/** | SPIFFS: wifi_config, config_params, log_backup, perf_hist.bin | RNF01, §6.7 |
| **interface/oled_ui/** | PARTE 8 estados, §14.5 máquina de estados, render telas | §6.2 estados |
| **interface/oled_ui/icones_clima/** | FASE 1.2: `desenharSol`, `getTipoClima`, ícones | FASE 1.2/1.3 |
| **interface/web/assets/** | PARTE 9: `INDEX_HTML` PROGMEM (~4000 linhas) | RF01 |
| **interface/web/rotas/** | §12: `handleRoot`, `handlePerf`, `/api/*` | RF01–RF07, §6.6 |
| **observabilidade/logging/** | PARTE 6: `logPrint`, `logInfo/Warn/Error`, ring buffer | RF04 |
| **observabilidade/performance/** | PARTE 7: `perfHist`, CRC, tempos µs, `handlePerf` | RF02–RF03 |
| **sistema/boot/** | §13: `setup`, `bootStep`, `BootState` (9 etapas) | §5.2, RNF02 |
| **sistema/tasks/** | FASE 2.1: `TaskAquisicao`, `TaskUI`, `sensorQueue` | RNF03, §6.3 |
| **sistema/energia/** | FASE 2.3: `enterLightSleep`, `RTC_DATA_ATTR` | RNF05, §6.5 |

## Estados da máquina (→ `interface/oled_ui/`)

Conforme §6.2 do documento:

```
RELOGIO → MENU → LISTA_ALARMES → DETALHE_ALARME
                              → EDITAR_ALARME_H → EDITAR_ALARME_M → ESCOLHER_MELODIA
         → HORA → EDITAR_HORA_M
```

## Arquivos planejados por pasta

```
config/          Pins.h  Config.h  Credentials.h
core/models/     Alarm.h  Nota.h  WeatherData.h  SensorData.h  ButtonEvent.h
sensores/dht22/  DHT22Sensor.h
sensores/rtc/    RTC_DS3231.h  (pasta: rtc_ds3231/)
entradas/botoes/ Buttons.h
atuadores/oled/  OLEDDisplay.h
atuadores/lcd/   LCDDisplay.h
atuadores/audio/ I2SAudio.h  Melodias.h  Notes.h
servicos/alarmes/     AlarmService.h
servicos/clima/       WeatherService.h
servicos/rede/        WiFiManager.h  NtpSync.h
servicos/persistencia/ WifiStorage.h  ConfigStorage.h  LogStorage.h  PerfStorage.h
interface/oled_ui/    StateMachine.h
interface/oled_ui/icones_clima/  WeatherIcons.h
interface/web/assets/ WebAssets.h
interface/web/rotas/  WebRoutes.h  WebServer.h
observabilidade/logging/      AppLog.h
observabilidade/performance/ PerfMetrics.h
sistema/boot/     BootSequence.h
sistema/tasks/    SensorTask.h  DisplayTask.h  WebTask.h
sistema/energia/  SleepManager.h
```

## Fluxo de dados (relogio7.ino atual)

```
                    ┌── sensores/dht22 ──┐
Core 0              │                    ├──► sensorQueue ──► Core 1 TaskUI
  TaskAquisicao ────┤                    │         │
                    └── servicos/clima ──┘         ├──► atuadores/lcd
                                                   ├──► interface/web (handleClient)
Loop principal ──► entradas/botoes (fila ISR)
              ──► servicos/alarmes
              ──► interface/oled_ui (render)
              ──► atuadores/audio_i2s (melodia step)
              ──► observabilidade/performance (amostragem)
```

## O que foi removido da estrutura anterior

| Pasta antiga | Motivo | Novo destino |
|--------------|--------|--------------|
| `hardware/` | Genérica demais | `sensores/`, `atuadores/`, `entradas/` |
| `weather/` | Serviço, não sensor | `servicos/clima/` + `interface/oled_ui/icones_clima/` |
| `audio/` | É atuador (RH03) | `atuadores/audio_i2s/` |
| `web/` | Parte da interface | `interface/web/` |
| `storage/` | Camada de dados §6.1 | `servicos/persistencia/` |
| `network/` | Serviço de comunicação | `servicos/rede/` |
| `ui/` | UI local específica OLED | `interface/oled_ui/` |
| `app/` | Observabilidade | `observabilidade/logging/` |
| `performance/` | Observabilidade | `observabilidade/performance/` |
| `boot/`, `power/`, `tasks/` | Infraestrutura | `sistema/` |
| `models/` | Tipos compartilhados | `core/models/` |

## Próximo passo sugerido

1. `config/` + `core/models/` — extrair defines e structs (sem dependências).
2. `sensores/` e `atuadores/` — drivers isolados (testáveis em `tests/`).
3. `sistema/boot/` — boot state machine.
4. `interface/` e `servicos/` — por último (mais acoplados).
