#pragma once

#define LORIM_VERSION "v0.1"
#define FORMAT_LITTLEFS_IF_FAILED true

// TaskGEM
#define GEM_DISABLE_GLCD
#define GEM_DISABLE_ADAFRUIT_GFX
#define U8G2_DISPLAY_TYPE U8G2_ST7565_JLX12864_F_4W_SW_SPI

// TaskManager
#define LONG_PRESS_DURATION 800/*ms*/
// Task unique IDs (!! opt to enumeration instead?)
#define ID_DEFAULT ID_TASKGEM
#define ID_NONE    0
#define ID_TASKGEM 1
#define ID_CHATBOX 2

// TaskChatbox
#define CHARS_PER_LINE 25  // <=254
#define LINES_PER_PAGE 8
#define CHATBOX_FONT u8g2_font_5x7_tr
#define CHATBOX_VERTICAL_PACE 8  // font height + 1

// MultitapIM
#define MAX_INPUT_LENGTH 128  // size stored as uint8_t
#define SELECTION_TIMEOUT 2000/*ms*/
#define CURSOR_TICK_INTERVAL 20000/*ticks*/

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
#define LORA_CS 15   // HSPI_CS0
#define LORA_RST 36
#define LORA_DIO 39
#define LORA_FREQ 470E6  // depends on your antenna setup

// Mailbox
#define ARDUINO_LORA
#define MAX_USERNAME_LENGTH 10
#define HISTORY_PATH "/history.txt"

// SpicedU8g2
#define STATUSBAR_FONT u8g2_font_micro_tr
#define STATUSBAR_FONTHEIGHT 5


// If you change this file, be sure to change the hyperlink to here in Pinouts.md to match line '// esp32'.
// If you change this file, be sure to change the hyperlink to here in Pinouts.md to match line '// esp32'.
// If you change this file, be sure to change the hyperlink to here in Pinouts.md to match line '// esp32'.
