#include "Lorim_definitions.hpp"
#include <Arduino.h>
#include <GEM_u8g2.h>
#include "Kbd_8x5_CH450.hpp"
#include "TaskDispatcher.hpp"

Kbd_8x5_CH450 keyboard(I2C_SDA, I2C_SCL, /*freq=1E6?*/5000);
U8G2_DISPLAY_TYPE u8g2(U8G2_R2, SPI_CLK, SPI_DATA, SPI_CS, SPI_DC, U8G2_DISPLAY_RESET);
TaskDispatcher taskDispatcher(u8g2, keyboard);

void saveSettings();

void setup() {

    Serial.begin(115200);
    u8g2.begin();
    taskDispatcher.init();

}

void loop() {
    taskDispatcher.tick();
}

void saveSettings() {
    
}
