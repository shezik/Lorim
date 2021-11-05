#pragma once

#define GEM_DISABLE_GLCD
#define GEM_DISABLE_ADAFRUIT_GFX
#define U8G2_DISPLAY_TYPE U8G2_ST7565_JLX12864_F_4W_SW_SPI

/*
// esp8266
#define CH450_INT D0
#define I2C_SDA D1
#define I2C_SCL D2
#define SPI_CLK D5
#define SPI_DATA D7
#define SPI_CS D8
#define SPI_DC D6
#define U8G2_DISPLAY_RESET U8X8_PIN_NONE
*/

// esp32
#define CH450_INT 34
#define I2C_SDA 21
#define I2C_SCL 22
#define SPI_CLK 18
#define SPI_DATA 23
#define SPI_CS 5
#define SPI_DC 19
#define U8G2_DISPLAY_RESET U8X8_PIN_NONE

// Task IDs (this is not even close to elegance)
#define DEFAULT_TASK ID_TASKGEM
#define ID_NONE    0
#define ID_TASKGEM 1
