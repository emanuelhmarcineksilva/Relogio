# Chronos Obsidian - Relógio Inteligente ESP32

O **Chronos Obsidian** é um sistema ciberfísico avançado baseado no microcontrolador ESP32, projetado para integrar monitoramento ambiental, sincronização de tempo precisa e uma interface de gerenciamento remota robusta. O projeto foca em alta performance, observabilidade e eficiência energética, utilizando arquitetura RTOS para garantir operação estável e multitarefa.


---

## 📁 Estrutura de Pastas

### `firmware/include/` - Headers (Declarações)
Contém arquivos de cabeçalho (`.h`) com declarações de classes, estruturas, constantes e protótipos de funções.

| Subpasta | Conteúdo |
|----------|----------|
| `atuadores/` | Headers para display OLED, LCD e áudio I2S |
| `config/` | Configurações (Credentials, Pins, Version) |
| `core/models/` | Modelos de dados (Alarm, SensorData, ButtonEvent) |
| `entradas/` | Headers para leitura de botões |
| `interface/` | UI OLED e assets web |
| `observabilidade/` | Logging e métricas de performance |
| `sensores/` | DHT22 e RTC DS3231 |
| `servicos/` | Alarmes, clima, persistência, rede |
| `sistema/` | Boot, energia, tasks RTOS |

### `firmware/src/` - Implementações
Contém os arquivos `.cpp` com a lógica de implementação, sendo cada um correspondente a um header em `include/`.

| Subpasta | Conteúdo |
|----------|----------|
| `atuadores/` | Implementações de OLEDDisplay, LCDDisplay, I2SAudio |
| `core/` | Variáveis globais |
| `entradas/` | Tratamento de botões com ISR |
| `interface/` | WebRoutes, WebAssets, UI OLED |
| `observabilidade/` | AppLog, PerfStorage |
| `sensores/` | DhtService, RtcService |
| `servicos/` | AlarmService, WeatherService, serviços de persistência |
| `sistema/` | BootSequence, MainLoop, SleepManager, Tasks |

### `firmware/tests/` - Testes
Testes automatizados do projeto.

### `firmware/aulas/` - Documentação Acadêmica
Materiais de estudo e documentação didática.

---

## 🔄 Fluxo do Projeto

```
┌─────────────────────────────────────────────────────────────┐
│                    Documentação Principal                   │
│                          (README.md)                        │
│                Apresenta arquitetura, APIs e uso            │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     Código Fonte                            │
│                     firmware/src/                           │
│              • MainLoop.cpp, BootSequence.cpp               │
│              • Services, Tasks, Drivers                     │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    Headers (include/)                       │
│            • Declarações, modelos, interfaces               │
└─────────────────────────────────────────────────────────────┘
```

---

## ❓ Diferença entre `include/` e `src/`

| Aspecto | `include/` | `src/` |
|---------|------------|--------|
| **Extensão** | `.h` (header) | `.cpp` (implementação) |
| **Conteúdo** | Declarações, protótipos, definições | Lógica, código executável |
| **Função** | Interface - o que o módulo expõe | Implementação - como funciona |
| **Exemplo** | `include/sensores/dht22/DhtService.h` | `src/sensores/dht22/DhtService.cpp` |

**Fluxo típico:** O header define a interface (`class DhtService { ... }`) e o `.cpp` implementa os métodos. O `main.cpp` inclui os headers necessários.

---

## 📖 Manual de Uso e Configuração

Siga estes passos para operar o seu Chronos Obsidian:

### 1. Inicialização e Alimentação
- Conecte o ESP32 a uma fonte de alimentação USB de 5V.
- O sistema passará por um **Boot por Fases**, verificando Display, RTC, SPIFFS e Sensores.
- Acompanhe o status no Display OLED e veja o endereço IP no LCD 16x2.

### 2. Acesso à Dashboard Web
- **Pela Rede Local:** Se o Wi-Fi estiver conectado, digite o IP exibido no seu navegador (ex: `http://192.168.1.15`).
- **Modo de Recuperação (AP):** Se o relógio não encontrar uma rede, conecte seu celular/PC ao Wi-Fi **"RelogioConfig"**. O endereço de acesso será `http://192.168.4.1`.

### 3. Operação das Abas
- **Principal:** Ajuste a hora do RTC, configure os 3 alarmes (escolhendo entre 5 melodias) e visualize a telemetria ambiental em tempo real.
- **Monitoramento:** Analise os 7 gráficos de latência para verificar a saúde do sistema. Você também pode exportar/importar backups do histórico de performance.
- **Sobre:** Utilize este campo para trocar o local da previsão do tempo ou atualizar as credenciais do Wi-Fi sem precisar de código.

### 4. Navegação Física (Botões)
- **Botão 1 (GPIO 4):** Clique para rotacionar entre os menus no OLED (Relógio -> Alarme -> Configurações).
- **Botão 2 (GPIO 18):** Clique para confirmar uma seleção ou segure para silenciar um alarme ativo.

---

## 🧪 Estrutura de Testes
O projeto foi validado sob condições de estresse para garantir:
1. **Resiliência de Rede:** Fallback automático para modo AP caso o roteador falhe.
2. **Integridade de Dados:** Validação CRC32 em cada inicialização para prevenir corrupção de arquivos flash.
3. **Estabilidade RTOS:** Monitoramento por Watchdog que reinicia o sistema autonomamente em caso de erro crítico de software.

---

## 🚀 Funcionalidades Implementadas

- **Sincronização Híbrida de Tempo:** Utiliza RTC Físico (DS3231) via I2C e sincronização NTP via Wi-Fi para garantir precisão absoluta.
- **Interface Visual Dupla:** 
  - **Display OLED SSD1306:** Interface gráfica com menus de configuração e ícones climáticos.
  - **Display LCD 16x2:** Exibição rápida de hora, IP de rede e temperatura local.
- **Monitoramento Ambiental:** Leituras em tempo real de temperatura e umidade via sensor DHT22.
- **Alarmes Customizáveis:** Suporte a múltiplos alarmes com sinalização sonora via I2S (MAX98357A) e notas musicais em PROGMEM.
- **Dashboard Web Embarcado:** Interface moderna para:
  - Configuração de alarmes e horário.
  - Ajuste de parâmetros de sensores e rede Wi-Fi.
  - Visualização de logs e métricas de sistema.
- **Gerenciamento de Energia (Light Sleep):** Redução de consumo de até 80% através de ciclos de sono inteligente, com despertamento instantâneo via botões físicos (GPIO) ou temporizador.
- **Persistência de Dados (SPIFFS):** Armazenamento seguro de configurações de rede, logs de sistema e histórico de performance com validação de integridade CRC32.

---

## 📝 Changelog

O projeto conta com um sistema de **Changelog Integrado** acessível diretamente pela interface web e armazenado no sistema de arquivos SPIFFS (`/changelog.txt`). Este mecanismo permite rastrear a evolução do firmware, registrando versões, datas e as principais mudanças implementadas (como a migração para a arquitetura RTOS Dual-Core e a implementação de modos de baixo consumo), garantindo transparência no ciclo de desenvolvimento.

---

## 🛠️ Hardware: Sensores e Atuadores

O sistema utiliza periféricos integrados via protocolos de comunicação industrial (I2C, I2S e Digital), garantindo máxima eficiência:

### **Sensores (Monitoramento)**
- **DHT22:** Sensor digital de alta precisão para medição de temperatura e umidade ambiental. Possui uma rotina de leitura assíncrona para evitar congelamentos no loop principal.
- **RTC DS3231:** Relógio em Tempo Real físico via barramento I2C (0x68). Mantém a contagem de tempo com precisão de quartz, mesmo sem Wi-Fi ou em caso de reinicialização, utilizando uma bateria de backup.

### **Atuadores (Interatividade)**
- **Display OLED SSD1306:** Tela principal de 128x64 pixels que apresenta a interface gráfica, menus de configuração e ícones climáticos customizados (sol, nuvem, chuva, neve).
- **Display LCD 16x2:** Display de redundância via I2C (0x27) que exibe permanentemente o endereço IP (ou modo AP), temperatura local e horário.
- **Módulo Áudio I2S (MAX98357A):** Amplificador digital que recebe dados de áudio serial via interface I2S, reproduzindo melodias musicais complexas armazenadas na memória `PROGMEM` do dispositivo.
- **Botões Físicos (GPIO 4 e 18):** Implementados com resistores de pull-up internos e interrupções de hardware (ISR) para resposta imediata ao toque do usuário.

---

## 📡 Conectividade e Redes

A conectividade é gerida por um sistema de fallback inteligente:
- **Modo STA:** Conecta-se à internet para buscar o horário oficial via NTP e dados climáticos da API **Meteosource**.
- **Modo AP (Access Point):** Caso a rede configurada fique indisponível, o relógio cria sua própria rede Wi-Fi (**RelogioConfig**), permitindo que o usuário acesse a interface e altere as credenciais sem precisar conectar o dispositivo ao computador.
- **Sincronização NTP:** Atualiza o RTC físico automaticamente via internet para garantir precisão de milisegundos.

---

## 🌐 Servidor Web e Dashboard Embarcado

O Chronos Obsidian hospeda um **Servidor Web RESTful** nativo. A interface foi desenvolvida para ser leve e extremamente funcional, eliminando a necessidade de aplicativos externos. A comunicação entre o navegador e o ESP32 ocorre via endpoints JSON, garantindo uma atualização fluida da dashboard. A interface é dividida em abas:
- **Principal:** Controle total de alarmes, hora e visualização rápida do clima.
- **Monitoramento:** Acesso a logs de sistema, visualização serial e ferramentas de backup da flash.
- **Changelog:** Histórico de evolução do firmware acessível via SPIFFS.
- **Sobre:** Informações de hardware (como o MAC Address), uptime e configurações técnicas de intervalos de amostragem.

### **Catálogo de Endpoints (API REST)**
O sistema expõe uma API RESTful para integração com o dashboard e outros serviços:
- **`GET /`**: Entrega a interface web completa (HTML/CSS/JS).
*   **`GET /api/alarmes`**: Retorna JSON com estados dos alarmes, dados do DHT22 e hora do sistema.
*   **`GET /api/hora?h=HH&m=MM`**: Ajusta o horário do RTC físico.
*   **`GET /api/alarme?id=N&h=HH&m=MM&mel=M`**: Configura um alarme específico.
*   **`GET /api/alarme/toggle?id=N`**: Alterna o estado (ON/OFF) de um alarme.
*   **`GET /api/clima`**: Força a atualização dos dados climáticos via API externa.
*   **`GET /api/serial`**: Retorna o log em tempo real do buffer de memória RAM.
*   **`GET /api/flash`**: Retorna os logs históricos persistidos no SPIFFS.
*   **`GET /api/config`**: Retorna os parâmetros operacionais atuais em JSON.
*   **`GET /api/config/set`**: Altera intervalos de leitura e cidade do clima.
*   **`GET /api/wifi/set?ssid=S&pass=P`**: Altera as credenciais de rede e tenta reconexão.
*   **`GET /api/perf`**: Conjunto completo de métricas de telemetria e performance.
*   **`GET /api/download_perf`**: Exporta o histórico de 24h para um arquivo CSV.
*   **`GET /api/changelog`**: Retorna o conteúdo do arquivo de versões do sistema.


---

## ⚙️ Arquitetura Multitarefa (FreeRTOS)

O firmware utiliza o sistema operacional de tempo real **FreeRTOS** para paralelizar tarefas entre os dois núcleos do ESP32, garantindo que a renderização do display ou a reprodução de áudio não interfiram na coleta de dados crítica.

| Task | Core | Prioridade | Stack | Função Principal |
| :--- | :---: | :---: | :---: | :--- |
| **TaskAquisicao** | 0 | 23 (Alta) | 4096B | Coleta de dados DHT22 e requisições HTTP para API de clima. |
| **TaskUI** | 1 | 22 (Média-Alta) | 4096B | Gerenciamento do Web Server e renderização OLED/LCD. |
| **TaskAudio** | 1 | 21 (Média) | 2048B | Streaming de notas musicais para o DAC I2S em tempo real. |
| **TaskPersistencia** | Dinâmico | 17 (Baixa) | 3072B | Validação de integridade CRC e backup periódico no SPIFFS. |

### **Comunicação por Fila (Queues)**
Para evitar condições de corrida (race conditions) entre os núcleos, o sistema utiliza filas seguras:
- **sensorQueue:** Transmite a estrutura `SensorData` da TaskAquisicao (Core 0) para a TaskUI (Core 1). O uso de `xQueueOverwrite` garante que o display sempre exiba a informação mais recente.
- **buttonEventQueue:** Enfileira eventos confirmados das interrupções de hardware, garantindo um processamento lógico limpo e livre de "debounce" mecânico.

---

## ⚡ Pilares Técnicos Avançados

### **1. Interrupções de Hardware com ISR Curta e Debounce por Timer**
O tratamento de botões não utiliza o tradicional (e ineficiente) `digitalRead()` em loop. Em vez disso, implementamos interrupções de hardware (**ISR**) para os pinos GPIO 4 e 18:
- **Latência:** A ISR executa em menos de **1µs**, apenas sinalizando a detecção de borda de descida (FALLING).
- **Debounce de Alta Precisão:** Utilizamos o `esp_timer` (timer de hardware de alta precisão) para aguardar **20ms** após o trigger inicial. Somente após esse intervalo, o pino é validado novamente.
- **Desacoplamento:** O evento confirmado é enviado para a `buttonEventQueue` via `xQueueSendFromISR`, permitindo que o processamento lógico (mudar menus, silenciar alarmes) ocorra fora do contexto crítico da interrupção.

### **2. Persistência com Histórico de 24 Horas e Validação CRC32**
O sistema mantém uma memória temporal da sua performance através de um histórico binário complexo armazenado no SPIFFS:
- **Amostragem:** 288 amostras circulares. Com uma amostragem a cada **5 minutos**, o sistema persiste exatamente **24 horas** de dados.
- **Estrutura de Dados:** Salva latências de 7 funções críticas, garantindo que o comportamento do sistema possa ser auditado retroativamente.
- **Segurança CRC32:** Cada salvamento gera um Checksum de 32 bits. Durante o boot, o sistema valida este código; se houver corrupção física da flash, o erro é logado e o sistema restaura o estado seguro a partir do último backup válido.

### **3. Configuração Remota Sem Reflash**
Diferente de projetos embarcados simples que exigem nova gravação de código para alterar parâmetros, o Chronos Obsidian é 100% configurável via web:
- **Parâmetros Operacionais:** Ajuste de hora, múltiplos alarmes (hora/minuto/melodia), intervalos de leitura DHT, polling de clima e frequência de backup.
- **Provisionamento de Wi-Fi:** Troca de SSID e Senha diretamente pela dashboard. O sistema salva e reinicia a stack de rede automaticamente.
- **Persistência de Configuração:** Todos os ajustes são mapeados para arquivos JSON/Texto no SPIFFS, garantindo que as preferências sobrevivam a quedas de energia.

### **4. Observabilidade Integrada com 7 Métricas em Tempo Real**
A dashboard web não exibe apenas o "status", mas sim a "telemetria" completa do firmware através de 7 séries temporais:
1. **Loop Time:** Latência do ciclo principal (em microssegundos), indicando se o sistema está sobrecarregado.
2. **Clima API:** Monitoramento de tempo de resposta da rede externa.
3. **RTC Read:** Performance do barramento I2C na comunicação com o relógio físico.
4. **Botoes:** Eficiência do debounce e enfileiramento de eventos.
5. **Display:** Tempo de renderização do OLED (crucial para detectar lentidão na UI).
6. **Leitura DHT:** Verificação de saúde do sensor digital (erros de checksum ou timeout).
7. **Lógica Alarme:** Custo computacional de verificar múltiplos gatilhos simultâneos.

---

## 💾 Armazenamento (SPIFFS) e Watchdog

### **SPIFFS (Sistema de Arquivos)**
- **Changelog:** Armazena o histórico de versões diretamente no dispositivo (`/changelog.txt`).
- **Logs:** Buffer circular de 4KB para auditoria de eventos em campo.

### **Watchdog (Tolerância a Falhas)**
Para garantir que o sistema nunca trave (uptime 24/7), implementamos:
- **Task Watchdog:** Cada tarefa deve "reportar" que está viva. Se uma rotina travar o processador por mais de 5 segundos, o Watchdog dispara um reset automático do sistema, garantindo a recuperação autônoma de erro.

---

## 📊 Gráficos de Performance (Observabilidade)

A dashboard web apresenta 7 séries temporais que permitem auditar o desempenho do firmware em microssegundos:
1. **Loop Time:** Indica a carga de processamento total do ciclo principal.
2. **Clima API:** Monitora a latência da rede externa e estabilidade do Wi-Fi.
3. **RTC Read:** Avalia a integridade e velocidade da comunicação I2C.
4. **Botoes:** Tempo gasto no processamento de eventos de entrada do usuário.
5. **Display:** Custo computacional da renderização de pixels no OLED.
6. **Leitura DHT:** Estabilidade física do sensor ambiental e possíveis erros de checksum.
7. **Lógica Alarme:** Eficiência da verificação de gatilhos e agendamentos.

---

## 🔋 Eficiência Energética (Light Sleep)

A gestão de energia é um pilar central do Chronos Obsidian. O sistema utiliza o modo **Light Sleep** do ESP32 para balancear performance e economia:

- **Funcionamento:** O sistema entra em modo de baixo consumo automaticamente após períodos de inatividade. Durante o Light Sleep, o processador principal e periféricos não-críticos são suspensos, reduzindo o consumo em até 80%.
- **Persistência em RAM:** Diferente do Deep Sleep, o estado da aplicação (variáveis, heap, stack) é preservado na RAM, permitindo uma retomada instantânea sem necessidade de reboot.
- **Fontes de Despertar (Wake Sources):**
  - **Temporizador:** O sistema acorda periodicamente para atualizar processos de background.
  - **Interrupções de Hardware (GPIO):** O pressionamento de qualquer um dos botões físicos desperta o dispositivo instantaneamente (< 1ms).
- **Conectividade:** A conexão Wi-Fi é mantida ativa em background, garantindo que o dashboard web permaneça acessível e as notificações funcionem imediatamente após o despertar.
- **RTC_DATA_ATTR:** Dados críticos como contagem de boots e métricas recentes são armazenados na memória lenta do RTC, garantindo integridade mesmo em reinicializações profundas.


## 📊 Observabilidade

O sistema monitora continuamente sua própria performance, registrando latências (em microssegundos) de rotinas críticas como o loop principal, requisições de API, leituras de sensores e atualizações de display. Esses dados são expostos em gráficos em tempo real no Dashboard Web, permitindo auditoria técnica do comportamento do firmware sob carga.

---

