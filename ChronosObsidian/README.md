# Chronos Obsidian — Firmware Modular

Código dividido a partir de `relogio7.ino` v4.0.0 (5921 linhas → 5897 linhas modulares).  
**55 funções verificadas** — nenhuma perdida (`tools/verify_split.py`).

## Compilar

```bash
cd ChronosObsidian/firmware
pio run          # PlatformIO
pio run -t upload
```

Requer [PlatformIO](https://platformio.org/). Bibliotecas declaradas em `platformio.ini`.

## Estrutura

```
firmware/
├── platformio.ini
├── include/
│   ├── ChronosObsidian.h      ← header central (includes + externs + protótipos)
│   ├── config/                ← Pins, Config, Credentials, Version
│   ├── core/models/           ← Alarm, Nota, WeatherData, SensorData, ButtonEvent
│   └── sistema/boot/          ← BootState enum
└── src/
    ├── main.cpp               ← pragma GCC O2
    ├── core/Globals.cpp       ← todas as variáveis globais + objetos hardware
    ├── sensores/              ← (leitura via tasks)
    ├── entradas/botoes/       ← Buttons.cpp (ISR + debounce)
    ├── atuadores/
    │   ├── oled/OLEDDisplay.cpp
    │   ├── lcd/LCDDisplay.cpp
    │   └── audio_i2s/         ← I2SAudio.cpp + Melodias.cpp
    ├── servicos/
    │   ├── alarmes/           ← lógica no MainLoop.cpp
    │   ├── clima/WeatherService.cpp
    │   ├── rede/              ← WiFi/NTP no BootSequence.cpp
    │   └── persistencia/      ← Wifi, Config, Log storage
    ├── interface/
    │   ├── oled_ui/icones_clima/WeatherIcons.cpp  (~3400 linhas ícones)
    │   └── web/
    │       ├── assets/WebAssets.cpp   ← INDEX_HTML PROGMEM
    │       └── rotas/WebRoutes.cpp    ← /api/*
    ├── observabilidade/
    │   ├── logging/AppLog.cpp
    │   └── performance/PerfStorage.cpp
    └── sistema/
        ├── boot/BootSequence.cpp  ← setup() + bootStep()
        ├── tasks/                 ← SensorTask + DisplayTask (RTOS)
        ├── energia/SleepManager.cpp
        └── MainLoop.cpp           ← loop() + máquina de estados OLED
```

## Mapa módulo → código original

| Arquivo | Conteúdo (relogio7.ino) |
|---------|-------------------------|
| `Globals.cpp` | Objetos hardware, alarmes, estado UI, filas RTOS |
| `Melodias.cpp` | PARTE 4 — notas PROGMEM |
| `AppLog.cpp` | PARTE 6 — ring buffer de log |
| `WifiStorage.cpp` | WiFi persistente SPIFFS |
| `ConfigStorage.cpp` | Parâmetros configuráveis |
| `LogStorage.cpp` | Backup de log na flash |
| `PerfStorage.cpp` | PARTE 7 — histórico 24h + CRC |
| `Buttons.cpp` | FASE 2.2 — ISR + debounce |
| `SleepManager.cpp` | FASE 2.3 — Light Sleep |
| `SensorTask.cpp` | TaskAquisicao (Core 0) |
| `DisplayTask.cpp` | TaskUI (Core 1) |
| `WebAssets.cpp` | PARTE 9 — HTML/CSS/JS |
| `I2SAudio.cpp` | Player I2S não bloqueante |
| `OLEDDisplay.cpp` | oledMsg (boot) |
| `LCDDisplay.cpp` | LCD 16x2 + timer 5s |
| `WeatherIcons.cpp` | Ícones clima + getTipoClima |
| `WeatherService.cpp` | Meteosource API + gerarMsgClima |
| `WebRoutes.cpp` | Todas as rotas HTTP |
| `BootSequence.cpp` | Boot 9 etapas + WiFi + NTP |
| `MainLoop.cpp` | loop + alarmes + menus OLED + métricas |

## Verificação

```bash
python3 ChronosObsidian/tools/verify_split.py
```

## Próximos passos (opcional)

- Mover credenciais WiFi/API para `secrets.h` (não versionar)
- Testes unitários em `firmware/tests/`
- Separar `MainLoop.cpp` em `interface/oled_ui/StateMachine.cpp`
