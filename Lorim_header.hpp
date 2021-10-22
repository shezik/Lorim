#ifndef __LORIM_HEADER__
#define __LORIM_HEADER__

#include <Arduino.h>
#include <GEM_u8g2.h>
#include "Kbd_8x5_CH450.hpp"

// Keyboard section
#define CH450_INT D0
Kbd_8x5_CH450 keyboard(/*sda=*/D1, /*scl=*/D2, /*freq=1000000?*/5000);
void menuKeyboardTick();

// Display section
uint8_t displayContrast = 127;
U8G2_ST7565_JLX12864_F_4W_SW_SPI u8g2(U8G2_R2, /*clock=*/D5, /*data=*/D7, /*cs=*/D8, /*dc=*/D6, /*reset=*/U8X8_PIN_NONE);
#define GEM_DISABLE_GLCD
#define GEM_DISABLE_ADAFRUIT_GFX
GEM_u8g2 menu(u8g2, /*menuPointerType=*/GEM_POINTER_DASH, /*menuItemsPerScreen=*/7, /*menuItemsPerScreen*/8, /*menuPageScreenTopOffset*/8, /*menuValuesLeftOffset*/80);
void setContrastCallback();

// Misc. section
void saveSettings();

#endif
