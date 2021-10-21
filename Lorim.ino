#include <Arduino.h>
#include "Kbd_8x5_CH450.hpp"

Kbd_8x5_CH450 keyboard(/*sda=*/21, /*scl=*/22, /*freq=1000000?*/5000);

void setup() {

    Serial.begin(115200);

}

void loop() {

}
