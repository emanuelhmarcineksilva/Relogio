# Explicação dos Gráficos de Performance - Chronos Obsidian

Este documento detalha o que cada gráfico está medindo, a origem desses valores no código-fonte e o motivo de alguns números parecerem "imensos".

## 1. Unidade de Medida: Microsegundos (us)
Todos os gráficos de performance (aba Monitoramento) utilizam a unidade **us** (microssegundos).
*   **1.000.000 us** = 1 segundo.
*   **1.000 us** = 1 milissegundo.

### Por que existem números "imensos"?
Números como `2.538.246` no Loop significam que o sistema levou **2,5 segundos** para completar um ciclo. Isso acontece geralmente quando o ESP32 está realizando tarefas de rede (como buscar o clima na Internet ou processar uma requisição web complexa), que são naturally lentas comparadas à velocidade da CPU.

## 2. Mapeamento dos Gráficos
Cada gráfico captura a latência (tempo de execução) de uma parte específica do sistema.

| Gráfico | O que mede? | Origem no Código (C++) | Por que medir? |
| :--- | :--- | :--- | :--- |
| **Loop** | Tempo total de uma volta no `loop()` principal. | `tempoLoopUs` em `MainLoop.cpp` | Identificar se o sistema está "travando" ou ficando lento no geral. |
| **Clima API** | Tempo de resposta da requisição HTTP ao servidor Meteosource. | `tempoPegarClimaUs` em `WeatherService.cpp` | Monitorar a qualidade da conexão e lentidão do servidor externo. |
| **RTC** | Tempo de leitura da hora no módulo físico DS3231 (I2C). | `tempoRtcNowUs` em `MainLoop.cpp` | Validar a estabilidade da comunicação com o hardware do relógio. |
| **Botoes** | Tempo de processamento da entrada dos botões (ISR + Fila). | `tempoLeituraBotoesUs` em `MainLoop.cpp` | Garantir que a interface física responda rapidamente (latência baixa). |
| **Display** | Tempo de desenho e atualização dos pixels no painel OLED. | `tempoAtualizarDisplayUs` em `MainLoop.cpp` | Otimizar a taxa de quadros (FPS) da interface visual. |
| **Leitura DHT** | **(Novo)** Tempo de execução da leitura do sensor DHT22. | `tempoPegarDHTUS` em `SensorTask.cpp` | Observar a carga que a biblioteca do sensor gera na CPU (Core 0). |
| **Lógica Alarme**| Tempo de verificação dos 3 alarmes no sistema. | `tempoLogicaAlarmeUs` em `MainLoop.cpp` | Garantir que a lógica de disparo não atrapalhe outras funções. |

## 3. Mudança nos Gráficos do DHT
Anteriormente, os gráficos do DHT mostravam os **valores do clima** (ex: 21°C, 69%). Conforme solicitado, essa visualização foi removida da aba de Performance para dar lugar à **Latência de Execução**, seguindo o material da aula **A4**.

### O que mudou no código?
Seguindo o exemplo de `micros()` da aula:
1.  No arquivo `SensorTask.cpp`, adicionei um cronômetro `esp_timer_get_time()` ao redor da função `dht.readHumidity()` e `dht.readTemperature()`.
2.  O resultado (em microssegundos) é salvo na variável global `tempoPegarDHTUS`.
3.  Essa variável agora é enviada para o Dashboard em vez do valor da temperatura/umidade.

**Nota:** Os valores reais de Clima (Temperatura e Umidade) continuam disponíveis na aba **Principal** para consulta.

---
*Documento gerado para auxiliar no entendimento de monitoramento de sistemas ciberfísicos.*
