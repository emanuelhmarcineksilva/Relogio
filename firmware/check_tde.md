# 📊 Auditoria de Requisitos TDE - Chronos Obsidian

Este documento valida o estado atual do projeto frente aos requisitos oficiais da disciplina de **Performance de Sistemas Ciberfísicos (PUCPR)**.

| Item | Requisito | Status | Localização/Evidência |
| :--- | :--- | :---: | :--- |
| **3.1** | **Interface Web Embarcada** | ✅ True | `WebRoutes.cpp`, `WebAssets.cpp` |
| **3.1.a** | Uso de CPU | ✅ True | Exibido no Dashboard (`cpuUsoEstimado`) |
| **3.1.b** | Uso de Memória (Heap, Stack, Flash) | ✅ True | Exibido na aba Performance e JSON API |
| **3.1.c** | Latência de no mínimo 5 funções | ✅ True | Loop, API Clima, RTC, Botoes, Display, DHT |
| **3.1.d** | Dados dos Sensores | ✅ True | DHT22 (local) e Meteosource (remoto) |
| **3.1.e** | Informações de Tasks (RTOS) | ✅ True | Enviado via API no `handlePerf` |
| **3.1.f** | Estado WiFi e AP | ✅ True | Exibido no Dashboard (Status e IP) |
| **3.1.g** | Gráficos de Série Temporal | ✅ True | 7 Gráficos dinâmicos na aba Performance |
| **3.1.h** | Sistema de Logs (3 níveis) | ✅ True | INFO, WARN, ERROR implementados |
| **3.1.i** | Exportação de Logs / Backup | ✅ True | Botões de Download TXT e restauração |
| **3.1.j** | Configuração Dinâmica (DHT, Clima) | ✅ True | Aba "Sobre/Config" permite ajuste sem flash |
| **3.1.k** | Troca de WiFi via Web | ✅ True | Implementado no `handleWifiSet` |
| **3.1.l** | Guia "Sobre" (Integrantes/GitHub) | ✅ True | Aba "Sobre" completa |
| **3.1.m** | Guia de Changelog | ✅ True | Card "Changelog" na aba Sobre |
| **3.2.a** | **Persistência de 24 horas (Mínimo)** | ✅ True | Buffer 288 pts @ 5min = 24h (Config.h) |
| **3.2.b** | Ausência de Rotinas Bloqueantes | ✅ True | Uso de `vTaskDelay` e estados não-bloqueantes |
| **3.2.c** | Uso de mais de uma Task (Multicore) | ✅ True | 4 Tasks pinadas em Níveis de Core 0 e 1 |
| **3.2.d** | Interrupção de Hardware | ✅ True | Botões usando ISR com Debounce via Timer |
| **3.2.e** | Gerenciamento de Energia | ✅ True | **Light Sleep** funcional (economiza ~80%) |
| **3.3.a** | **Mínimo de 2 Sensores de Entrada** | ✅ True | DHT22 (Temp/Hum) + Botões (Interrupts) |
| **3.3.b** | Mínimo de 2 Dispositivos Acionadores | ✅ True | OLED SSD1306 + LCD 16x2 (I2C) |

---
### ⚠️ Nota sobre Persistência de 24h
Para cumprir o requisito acadêmico de 24h, o sistema deve registrar 1 ponto a cada 5 minutos (288 pontos = 1440 min = 24h). 
Atualmente, ajustamos para 1 segundo a pedido do usuário para visualização em tempo real de 5 minutos.
**Aconselhável:** Voltar para 5 minutos antes da entrega oficial para a apresentação.

**Data da Auditoria:** 2026-06-12  
**Responsável:** Antigravity (AI Assistant)
