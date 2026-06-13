#!/usr/bin/env python3
"""Verifica se todas as funções públicas de relogio7.ino existem no firmware modular."""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ORIG = ROOT / "relogio7.ino"
FW = ROOT / "ChronosObsidian" / "firmware"

FUNCS = [
    "logPrint", "logPrintln", "logPrintf", "logInfo", "logWarn", "logError", "getLogContent",
    "salvarWifiConfig", "carregarWifiConfig", "salvarConfigParams", "carregarConfigParams",
    "salvarLogFlash", "carregarLogFlash", "crc32_simple", "salvarPerfFlash", "carregarPerfFlash",
    "enterLightSleep", "TaskAquisicao", "TaskUI", "iniciarI2S", "tocarMelodiaStep",
    "oledMsg", "iniciarLCD", "atualizarLCD", "desenharNuvem", "desenharSol",
    "desenharTermometro", "desenharNeve", "desenharNuvemDeChuva", "getTipoClima",
    "gerarMsgClima", "pegarClima", "estadoToStr", "handleRoot", "handleLog",
    "handleAlarmes", "handleSetHora", "handleSetAlarme", "handleToggleAlarme",
    "handleClima", "handleSerial", "handleFlash", "handleConfigGet", "handleConfigSet",
    "handleWifiSet", "handlePerf", "handleSalvar", "handleDownloadPerf", "handleUploadPerf",
    "iniciarServidorWeb", "setup", "bootStep", "loop", "btn1_isr", "btn2_isr",
]

def all_fw_text():
    parts = []
    for p in FW.rglob("*.cpp"):
        parts.append(p.read_text(encoding="utf-8"))
    for p in FW.rglob("*.h"):
        if p.name != "ChronosObsidian.h":
            parts.append(p.read_text(encoding="utf-8"))
    parts.append((FW / "include" / "ChronosObsidian.h").read_text(encoding="utf-8"))
    return "\n".join(parts)

def main():
    fw = all_fw_text()
    missing = [f for f in FUNCS if f" {f}(" not in fw and f"{f}(" not in fw]
    lines_orig = len(ORIG.read_text().splitlines())
    lines_fw = sum(len(p.read_text().splitlines()) for p in FW.rglob("*.cpp"))
    lines_fw += sum(len(p.read_text().splitlines()) for p in FW.rglob("*.h"))

    print(f"Linhas relogio7.ino:     {lines_orig}")
    print(f"Linhas firmware modular: {lines_fw}")
    print(f"Funções verificadas:     {len(FUNCS)}")
    if missing:
        print(f"FALTANDO ({len(missing)}): {', '.join(missing)}")
        return 1
    print("OK — todas as funções encontradas no firmware modular.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
