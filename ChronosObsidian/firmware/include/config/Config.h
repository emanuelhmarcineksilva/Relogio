#pragma once
#include "config/Version.h"

#define NUM_ALARMES 3
#define LOG_BUF_SIZE 4000
#define PERF_HIST_SIZE 288
#define PERF_FUNCOES 7
#define PERF_SAMPLE_MS 5000
#define LIGHT_SLEEP_IDLE_MS 10000
#define DURACAO_MSG_CLIMA 10000
#define BOOT_MSG_INTERVAL 80
#define WIFI_TIMEOUT_MS   15000
#define TASK_AQUISICAO_INTERVAL 5000
#define TASK_UI_INTERVAL 500
#define TASK_AQUISICAO_PRIORITY (configMAX_PRIORITIES - 2)
#define TASK_UI_PRIORITY (configMAX_PRIORITIES - 3)

// Notas musicais
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
#define N_C6 1047
#define N_SIL   0

#define TOTAL_MELODIAS 5
