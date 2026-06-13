#include "ChronosObsidian.h"

void logPrint(const char* msg) {
  Serial.print(msg);  // Aparece no monitor serial do Arduino IDE
  int len = strlen(msg);  // strlen conta quantos caracteres tem na mensagem
  for (int i = 0; i < len; i++) {
    logBuffer[logPos] = msg[i];  // Copia um caractere para o buffer
    logPos++;
    if (logPos >= LOG_BUF_SIZE) {  // Chegou no fim? Volta ao inicio
      logPos = 0;
      logWrapped = true;  // Marca que o buffer ja deu a volta
    }
  }
}
// Versao com quebra de linha
void logPrintln(const char* msg) { logPrint(msg); logPrint("\n"); }
// Versao formatada (igual ao printf do C)
// "..." significa que aceita qualquer quantidade de argumentos extras
void logPrintf(const char* fmt, ...) {
  char tmp[256];
  va_list args;           // Lista de argumentos variaveis
  va_start(args, fmt);    // Inicia a lista
  vsnprintf(tmp, sizeof(tmp), fmt, args);  // Monta a string formatada
  va_end(args);           // Encerra a lista
  logPrint(tmp);
}

// === LOGS COM NIVEIS ===
// Em sistemas profissionais, logs tem niveis de severidade:
//   [INFO]  = informacao normal (tudo certo)
//   [WARN]  = aviso (algo estranho mas nao critico)
//   [ERROR] = erro (algo deu errado)
// Isso facilita filtrar problemas na aba de logs da web.
void logInfo(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[INFO] "); logPrint(tmp);
}
void logWarn(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[WARN] "); logPrint(tmp);
}
void logError(const char* fmt, ...) {
  char tmp[256];
  va_list args; va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp), fmt, args);
  va_end(args);
  logPrint("[ERROR] "); logPrint(tmp);
}

// Retorna todo o log em ordem cronologica
String getLogContent() {
  String result;
  result.reserve(LOG_BUF_SIZE);
  // Se o buffer deu a volta, os dados mais antigos estao apos a posicao atual
  if (logWrapped) {
    for (int i = logPos; i < LOG_BUF_SIZE; i++) result += logBuffer[i];
  }
  for (int i = 0; i < logPos; i++) result += logBuffer[i];
  return result;
}
