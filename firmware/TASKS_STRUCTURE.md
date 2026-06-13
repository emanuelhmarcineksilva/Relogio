# Estrutura de Tasks do Projeto Chronos Obsidian

## Resumo Geral
O projeto utiliza **4 Tasks (tarefas) paralelas** no FreeRTOS, rodando em dual-core no ESP32, em vez de 6-7 tasks como era a confusão inicial. Os **7 gráficos de performance** são **métricas**, não tasks separadas.

---

## 1. TaskAquisicao (Core 0 - Sensor Data Collection)

### Arquivo
`src/sistema/tasks/SensorTask.cpp`

### Propósito
- Coleta dados do sensor **DHT22** (temperatura e umidade locais)
- Busca dados da **API Meteosource** (previsão de clima remota)
- Enfileira dados em fila FIFO para consumo por TaskUI

### Configuração
```cpp
#define TASK_AQUISICAO_PRIORITY (configMAX_PRIORITIES - 2)  // Prioridade: ALTA
#define TASK_AQUISICAO_INTERVAL 5000  // Executada a cada 5 segundos
```

### Core de Execução
- **Core 0** (WiFi/rede) — sempre disponível para I2C, HTTP, conexões

### Intervalo
- Leitura DHT: ~5ms (1 vez a cada 5s)
- API Clima: ~500ms (1 vez a cada 10 minutos, configurável)

### Comunicação
- **Entrada**: Nada
- **Saída**: `sensorQueue` (fila com SensorData)
  - `dht_temp` (float): temperatura do DHT
  - `dht_humidity` (float): umidade do DHT
  - `api_temp` (float): temperatura da API
  - `weather_code` (int): código do clima
  - `valid` (bool): dados válidos?

### Métrica Coletada
- `perfHist[1]` = `tempoPegarClimaUs` (tempo de requisição HTTP)

---

## 2. TaskUI (Core 1 - User Interface + Web)

### Arquivo
`src/sistema/tasks/DisplayTask.cpp`

### Propósito
- **Renderização de UI** em OLED/LCD
- **Servidor HTTP** embutido para interface web
- **Máquina de estados** para menus locais (RELOGIO → MENU → ALARMES)
- **Consumo de dados da fila** de sensores

### Configuração
```cpp
#define TASK_UI_PRIORITY (configMAX_PRIORITIES - 3)  // Prioridade: MÉDIA-ALTA
#define TASK_UI_INTERVAL 500  // Executada a cada 500ms (~2 Hz)
```

### Core de Execução
- **Core 1** (aplicação) — dedicado à UI e display

### Intervalo
- Atualização de tela: ~500ms
- Processamento HTTP: sub-10ms (não-bloqueante)

### Comunicação
- **Entrada**: `sensorQueue` (lê dados do DHT + clima)
- **Saída**: Renderização de tela, respostas HTTP

### Responsabilidades
1. Ler fila de sensores (sem bloquear)
2. Renderizar displays OLED + LCD
3. Processar requisições HTTP (GET/POST para rotas web)
4. Atualizar variáveis globais (`dhtTemperature`, `dhtHumidity`, `temperatura`)

### Métricas Coletadas
- `perfHist[5]` = `tempoAtualizarDisplayUs`
- `perfHist[0]` = `tempoLoopUs` (tempo total de ciclo)

---

## 3. TaskAudio (Core 1 - Sound Reproduction) [NOVO]

### Arquivo
`src/sistema/tasks/AudioTask.cpp`

### Propósito
- Reprodução **não-bloqueante** de melodias de alarme
- Geração de ondas sonoras em **I2S** (saída para amplificador MAX98357A)
- Controle de timbrado sem bloquear UI ou aquisição

### Configuração
```cpp
#define TASK_AUDIO_PRIORITY (configMAX_PRIORITIES - 4)  // Prioridade: MÉDIA
#define TASK_AUDIO_INTERVAL 50  // Executada a cada 50ms
```

### Core de Execução
- **Core 1** (aplicação) — junto com UI, mas prioridade menor

### Intervalo
- Atualização de nota: ~50ms (20 Hz)
- Permite transições suave entre notas

### Comunicação
- **Entrada**: Estado global (`alarmeDisparo`, `melodiaAtualIdx`, `notaAtualIdx`)
- **Saída**: Sinal I2S enviado ao amplificador

### Função Chamada
```cpp
tocarMelodiaStep();  // Toca uma nota por vez
```

### Métricas
- Tempo de geração de onda (em andamento)

---

## 4. TaskPersistencia (Core Dinâmico - Data Persistence) [NOVO]

### Arquivo
`src/sistema/tasks/PersistenceTask.cpp`

### Propósito
- **Backup periódico** de logs em SPIFFS
- **Histórico de performance** (salva dados em arquivo binário com CRC)
- **Validação de integridade** de dados na flash
- **Limpeza e manutenção** de arquivos antigos

### Configuração
```cpp
#define TASK_PERSISTENCIA_PRIORITY (configMAX_PRIORITIES - 8)  // Prioridade: MUITO BAIXA
#define TASK_PERSISTENCIA_INTERVAL 300000  // Executada a cada 5 minutos
```

### Core de Execução
- **Sem afinidade** (`tskNO_AFFINITY`) — executa em qualquer core que estiver livre
- Não interfere com críticas (boot, aquisição, UI, áudio)

### Intervalos de Operação
- Backup de **logs**: a cada 30 minutos
- Backup de **performance**: a cada 2 horas
- Validação de **integridade**: a cada 6 horas

### Comunicação
- **Entrada**: `logBuffer`, `perfHist[]`, estado global
- **Saída**: Arquivos em SPIFFS:
  - `/log_backup.txt` (logs em texto)
  - `/perf_backup.bin` (histórico binário com CRC)
  - `/wifi_config.bin` (credenciais)

### Funções Principais
```cpp
salvarLogFlash();              // Backup de logs
salvarPerfFlash();             // Backup de performance (com CRC)
validarIntegridadeFlash();     // Verifica arquivos críticos
```

---

## OS 7 GRÁFICOS (Métricas, não Tasks)

| # | Métrica | Índice Array | Origem | Unidade |
|---|---------|-------------|--------|---------|
| 1 | Tempo de Loop | `perfHist[0]` | `MainLoop.cpp` | µs (microsegundos) |
| 2 | Tempo de Clima (API) | `perfHist[1]` | `TaskAquisicao` | µs |
| 3 | Tempo de RTC | `perfHist[2]` | `MainLoop.cpp` | µs |
| 4 | Tempo de Botões | `perfHist[3]` | `MainLoop.cpp` | µs |
| 5 | Temperatura DHT | `perfHist[4]` | `TaskAquisicao` | °C × 10 |
| 6 | Tempo de Display | `perfHist[5]` | `MainLoop.cpp` | µs |
| 7 | Umidade DHT | `perfHist[6]` | `TaskAquisicao` | % × 10 |

---

## Comunicação Inter-Tasks (FreeRTOS)

### Fila: sensorQueue
```
TaskAquisicao  → xQueueOverwrite()  →  sensorQueue  →  TaskUI (xQueueReceive)
```
- Tamanho: 1 elemento
- Estrutura: `SensorData` (100 bytes)
- Tipo: **Sempre sobrescreve** (nunca bloqueia)

### Fila: buttonEventQueue
```
btn1_isr() → [timer] → btn1_debounce_callback()  →  buttonEventQueue
MainLoop() consome                                      (xQueueReceive)
```

---

## Prioridades (FreeRTOS)

```
configMAX_PRIORITIES = 25 (no ESP32)

TaskAquisicao   → Priority 23 (máxima - 2)      [Core 0]
TaskUI          → Priority 22 (máxima - 3)      [Core 1]
TaskAudio       → Priority 21 (máxima - 4)      [Core 1]
TaskPersistencia → Priority 17 (máxima - 8)     [Dinâmico]

Tarefas de Sistema → 0-10 (WiFi, NTP, etc)
```

---

## Tamanho de Pilha (Stack)

| Task | Stack (bytes) | Justificativa |
|------|---------------|---------------|
| TaskAquisicao | 4096 | Sensor + HTTP (pode alocar buffer JSON ~2KB) |
| TaskUI | 4096 | Display rendering + processamento web |
| TaskAudio | 2048 | Apenas geração de onda (uso mínimo) |
| TaskPersistencia | 3072 | File I/O, validação CRC |

---

## Estrutura de Pastas

```
src/sistema/tasks/
├── SensorTask.cpp       → TaskAquisicao
├── DisplayTask.cpp      → TaskUI
├── AudioTask.cpp        → TaskAudio [NOVO]
└── PersistenceTask.cpp  → TaskPersistencia [NOVO]
```

---

## Próximas Melhorias

1. **TaskAudio**: Implementar controle de volume, fade in/out
2. **TaskPersistencia**: Compressão de histórico, sincronização em nuvem
3. **TaskPersistencia**: Recuperação automática de corrupção
4. **Monitoramento**: Dashboard web com CPU % em tempo real
5. **Distribuição**: Considerar separar TaskWebServer como task independente

---

## Compilação e Testes

### Constantes adicionadas em `include/config/Config.h`
```cpp
#define TASK_AUDIO_INTERVAL 50
#define TASK_PERSISTENCIA_INTERVAL 300000
#define TASK_AUDIO_PRIORITY (configMAX_PRIORITIES - 4)
#define TASK_PERSISTENCIA_PRIORITY (configMAX_PRIORITIES - 8)
```

### Protótipos adicionados em `include/ChronosObsidian.h`
```cpp
void TaskAudio(void* pvParameters);
void TaskPersistencia(void* pvParameters);
void validarIntegridadeFlash();
```

### Variáveis globais adicionadas em `src/core/Globals.cpp`
```cpp
TaskHandle_t taskAudioHandle = NULL;
TaskHandle_t taskPersistenciaHandle = NULL;
unsigned long taskAudioLastRun = 0;
unsigned long taskPersistenciaLastRun = 0;
```

### Boot adicionado em `src/sistema/boot/BootSequence.cpp`
```cpp
xTaskCreatePinnedToCore(TaskAudio, "TaskAudio", 2048, NULL, 
                        TASK_AUDIO_PRIORITY, &taskAudioHandle, 1);
xTaskCreatePinnedToCore(TaskPersistencia, "TaskPersistencia", 3072, NULL, 
                        TASK_PERSISTENCIA_PRIORITY, &taskPersistenciaHandle, tskNO_AFFINITY);
```

---

## Data: 2026-06-12
**Status**: ✅ Implementação Completa das 4 Tasks
