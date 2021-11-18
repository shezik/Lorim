#pragma once

#define GEM_DISABLE_GLCD
#define GEM_DISABLE_ADAFRUIT_GFX
#define U8G2_DISPLAY_TYPE U8G2_ST7565_JLX12864_F_4W_SW_SPI

#define CHARS_PER_LINE 25  // <=254
#define LINES_PER_PAGE 8

#define FORMAT_LITTLEFS_IF_FAILED true

/*
// esp8266 (deprecated)
#define CH450_INT D0
#define I2C_SDA D1
#define I2C_SCL D2
#define SPI_CLK D5
#define SPI_DATA D7
#define SPI_CS D8
#define SPI_DC D6
#define U8G2_DISPLAY_RESET U8X8_PIN_NONE
#define LORA_CS 18
#define LORA_RST 23
#define LORA_DIO 26
#define LORA_FREQ 470E6
*/

// esp32
#define CH450_INT 34
#define I2C_SDA 21
#define I2C_SCL 22
#define VSPI_DC 19    // VSPI_MISO
#define VSPI_DATA 23  // VSPI_MOSI
#define VSPI_CLK 18
#define VSPI_CS 5
#define U8G2_DISPLAY_RESET U8X8_PIN_NONE
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_CLK 14
#define LORA_CS 15  // HSPI_CS0
#define LORA_RST 36
#define LORA_DIO 39
#define LORA_FREQ 470E6

// Task unique IDs
#define ID_DEFAULT ID_TASKGEM
#define ID_NONE    0
#define ID_TASKGEM 1
#define ID_CHATBOX 2

#define ARDUINO_LORA
#define MAX_USERNAME_LENGTH 10
#define HISTORY_PATH "/history.txt"
