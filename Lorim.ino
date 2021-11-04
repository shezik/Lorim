#include "Lorim_definitions.hpp"
#include <Arduino.h>
#include <GEM_u8g2.h>
#include "Kbd_8x5_CH450.hpp"
#include "TaskDispatcher.hpp"

Kbd_8x5_CH450 keyboard(/*sda=*/D1, /*scl=*/D2, /*freq=1E6?*/5000);
U8G2_DISPLAY_TYPE u8g2(U8G2_R2, /*clock=*/D5, /*data=*/D7, /*cs=*/D8, /*dc=*/D6, /*reset=*/U8X8_PIN_NONE);
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
