# Graph Report - firmware  (2026-06-12)

## Corpus Check
- 33 files · ~43,068 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 93 nodes · 151 edges · 11 communities
- Extraction: 52% EXTRACTED · 48% INFERRED · 0% AMBIGUOUS · INFERRED: 72 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- [[_COMMUNITY_Community 0|Community 0]]
- [[_COMMUNITY_Community 1|Community 1]]
- [[_COMMUNITY_Community 2|Community 2]]
- [[_COMMUNITY_Community 3|Community 3]]
- [[_COMMUNITY_Community 4|Community 4]]
- [[_COMMUNITY_Community 6|Community 6]]

## God Nodes (most connected - your core abstractions)
1. `loop()` - 18 edges
2. `bootStep()` - 14 edges
3. `logPrintln()` - 13 edges
4. `logInfo()` - 12 edges
5. `logPrintf()` - 11 edges
6. `logWarn()` - 7 edges
7. `salvarPerfFlash()` - 7 edges
8. `pegarClima()` - 7 edges
9. `logPrint()` - 6 edges
10. `logError()` - 6 edges

## Surprising Connections (you probably didn't know these)
- `bootStep()` --calls--> `iniciarI2S()`  [INFERRED]
  src/sistema/boot/BootSequence.cpp → src/atuadores/audio_i2s/I2SAudio.cpp
- `loop()` --calls--> `tocarMelodiaStep()`  [INFERRED]
  src/sistema/MainLoop.cpp → src/atuadores/audio_i2s/I2SAudio.cpp
- `iniciarLCD()` --calls--> `logError()`  [INFERRED]
  src/atuadores/lcd/LCDDisplay.cpp → src/observabilidade/logging/AppLog.cpp
- `iniciarLCD()` --calls--> `logPrintln()`  [INFERRED]
  src/atuadores/lcd/LCDDisplay.cpp → src/observabilidade/logging/AppLog.cpp
- `loop()` --calls--> `atualizarLCD()`  [INFERRED]
  src/sistema/MainLoop.cpp → src/atuadores/lcd/LCDDisplay.cpp

## Import Cycles
- None detected.

## Communities (11 total, 0 thin omitted)

### Community 0 - "Community 0"
Cohesion: 0.18
Nodes (11): btn1_debounce_callback(), btn2_debounce_callback(), pegarClima(), logError(), logInfo(), logPrint(), logWarn(), carregarWifiConfig() (+3 more)

### Community 1 - "Community 1"
Cohesion: 0.14
Nodes (12): getLogContent(), logPrintf(), salvarConfigParams(), estadoToStr(), handleConfigSet(), handleLog(), handleSerial(), handleSetAlarme() (+4 more)

### Community 2 - "Community 2"
Cohesion: 0.15
Nodes (8): bootStep(), DateTime, atualizarLCD(), iniciarLCD(), oledMsg(), carregarConfigParams(), handleSetHora(), iniciarServidorWeb()

### Community 3 - "Community 3"
Cohesion: 0.23
Nodes (10): iniciarI2S(), tocarMelodiaStep(), gerarMsgClima(), desenharNeve(), desenharNuvem(), desenharNuvemDeChuva(), desenharSol(), desenharTermometro() (+2 more)

### Community 4 - "Community 4"
Cohesion: 0.24
Nodes (9): enterLightSleep(), logPrintln(), carregarPerfFlash(), crc32_simple(), salvarPerfFlash(), salvarLogFlash(), handleSalvar(), handleUploadPerf() (+1 more)

### Community 6 - "Community 6"
Cohesion: 0.50
Nodes (3): carregarLogFlash(), handleFlash(), String

## Knowledge Gaps
- **3 isolated node(s):** `String`, `String`, `String`
  These have ≤1 connection - possible missing edges or undocumented components.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `loop()` connect `Community 3` to `Community 0`, `Community 1`, `Community 2`, `Community 4`?**
  _High betweenness centrality (0.236) - this node is a cross-community bridge._
- **Why does `bootStep()` connect `Community 2` to `Community 0`, `Community 1`, `Community 3`, `Community 4`?**
  _High betweenness centrality (0.151) - this node is a cross-community bridge._
- **Why does `logInfo()` connect `Community 0` to `Community 1`, `Community 2`, `Community 3`, `Community 4`?**
  _High betweenness centrality (0.140) - this node is a cross-community bridge._
- **Are the 17 inferred relationships involving `loop()` (e.g. with `tocarMelodiaStep()` and `bootStep()`) actually correct?**
  _`loop()` has 17 INFERRED edges - model-reasoned connections that need verification._
- **Are the 13 inferred relationships involving `bootStep()` (e.g. with `iniciarI2S()` and `DateTime`) actually correct?**
  _`bootStep()` has 13 INFERRED edges - model-reasoned connections that need verification._
- **Are the 11 inferred relationships involving `logPrintln()` (e.g. with `bootStep()` and `enterLightSleep()`) actually correct?**
  _`logPrintln()` has 11 INFERRED edges - model-reasoned connections that need verification._
- **Are the 10 inferred relationships involving `logInfo()` (e.g. with `btn1_debounce_callback()` and `btn2_debounce_callback()`) actually correct?**
  _`logInfo()` has 10 INFERRED edges - model-reasoned connections that need verification._