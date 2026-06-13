#!/usr/bin/env python3
"""
Divide relogio7.ino em módulos ChronosObsidian.
Garante que todo código executável e dados globais sejam preservados.
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "relogio7.ino"
FW = ROOT / "ChronosObsidian" / "firmware"

# Blocos de implementação: (arquivo_relativo, linha_ini, linha_fim) 1-based inclusive
IMPL_BLOCKS = [
    ("src/observabilidade/logging/AppLog.cpp", 280, 343),
    ("src/servicos/persistencia/WifiStorage.cpp", 350, 381),
    ("src/servicos/persistencia/ConfigStorage.cpp", 385, 414),
    ("src/servicos/persistencia/LogStorage.cpp", 417, 438),
    ("src/observabilidade/performance/PerfStorage.cpp", 477, 558),
    ("src/entradas/botoes/Buttons.cpp", 629, 685),
    ("src/sistema/energia/SleepManager.cpp", 721, 737),
    ("src/sistema/tasks/SensorTask.cpp", 858, 914),
    ("src/sistema/tasks/DisplayTask.cpp", 930, 965),
    ("src/interface/web/assets/WebAssets.cpp", 979, 1057),
    ("src/atuadores/audio_i2s/I2SAudio.cpp", 1062, 1154),
    ("src/atuadores/oled/OLEDDisplay.cpp", 1161, 1182),
    ("src/atuadores/lcd/LCDDisplay.cpp", 1185, 1281),
    ("src/interface/oled_ui/icones_clima/WeatherIcons.cpp", 1286, 4704),
    ("src/servicos/clima/WeatherService.cpp", 4709, 4826),
    ("src/interface/web/rotas/WebRoutes.cpp", 4831, 5236),
    ("src/sistema/boot/BootSequence.cpp", 5241, 5436),
    ("src/sistema/MainLoop.cpp", 5440, 5920),
]

CPP_HDR = '#include "ChronosObsidian.h"\n\n'


def read_src():
    return SRC.read_text(encoding="utf-8").splitlines(keepends=True)


def write_file(path: Path, content: str, append=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    mode = "a" if append else "w"
    with open(path, mode, encoding="utf-8") as f:
        if not append:
            f.write(content)
        else:
            f.write("\n" + content)


def extract_impl_blocks(lines):
    covered = set()
    for rel, start, end in IMPL_BLOCKS:
        chunk = "".join(lines[start - 1 : end])
        path = FW / rel
        if path.exists():
            write_file(path, chunk, append=True)
        else:
            write_file(path, CPP_HDR + chunk)
        for i in range(start, end + 1):
            covered.add(i)
    return covered


def write_lcd_callback(lines):
    """lcd_rotation_callback é static — precisa ficar no LCDDisplay.cpp"""
    path = FW / "src/atuadores/lcd/LCDDisplay.cpp"
    cb = "".join(lines[767:771])
    if path.exists():
        content = path.read_text(encoding="utf-8")
        if "lcd_rotation_callback" not in content:
            # Insere antes de iniciarLCD
            content = content.replace(
                "void iniciarLCD()",
                cb + "\nvoid iniciarLCD()",
            )
            path.write_text(content, encoding="utf-8")


def main():
    lines = read_src()
    covered = extract_impl_blocks(lines)
    write_lcd_callback(lines)

    # Verificação: linhas com código real fora dos blocos
    skip_prefixes = ("//", "/*", "*", "#include", "#pragma", "#define")
    code_outside = []
    for i, line in enumerate(lines, 1):
        if i in covered:
            continue
        s = line.strip()
        if not s:
            continue
        if s.startswith("//"):
            continue
        if any(s.startswith(p) for p in skip_prefixes):
            continue
        code_outside.append((i, s[:80]))

    report = []
    report.append(f"Origem: {len(lines)} linhas")
    report.append(f"Implementações extraídas: {len(covered)} linhas")
    report.append(f"Código restante (em ChronosObsidian.h + Globals.cpp): {len(code_outside)} linhas")
    if len(code_outside) > 80:
        report.append("AVISO: muitas linhas restantes — revisar ChronosObsidian.h / Globals.cpp")
    for i, s in code_outside[:30]:
        report.append(f"  L{i}: {s}")

    rpt = FW / "SPLIT_REPORT.txt"
    rpt.write_text("\n".join(report), encoding="utf-8")
    print("\n".join(report))


if __name__ == "__main__":
    main()
