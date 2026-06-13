#pragma once

// --- Display OLED ---
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_ADDR    0x3C
#define SDA_PIN        21
#define SCL_PIN        22

// --- Audio I2S (MAX98357A) ---
#define I2S_BCLK       26
#define I2S_LRC        25
#define I2S_DIN        27
#define I2S_PORT   I2S_NUM_0
#define SAMPLE_RATE 22050

// --- DHT22 ---
#define DHTPIN 23
#define DHTTYPE DHT22

// --- Botões ---
#define PIN_BUTTON1 4
#define PIN_BUTTON2 18
