#include "Lorim_header.hpp"
#include "Lorim_GEM_Pages.hpp"

void setup() {

    pinMode(CH450_INT, INPUT);

    Serial.begin(115200);
    u8g2.begin();

    keyboard.init();
    menu.init();
    setupMenu();
    menu.drawMenu();

}

void loop() {
    menuKeyboardTick();
}

void setContrastCallback() {
    u8g2.setContrast(displayContrast);
    saveSettings();
}

void saveSettings() {

}

void menuKeyboardTick() {
    if (menu.readyForKey() && !digitalRead(CH450_INT)) {  // active low
        uint8_t keycode = Kbd_8x5_CH450::toKeycode(keyboard.getKeyData());
        switch (keycode) {
            case 0:
                menu.registerKeyPress(GEM_KEY_UP);
                break;
            case 1:
                menu.registerKeyPress(GEM_KEY_DOWN);
                break;
            case 2:
                menu.registerKeyPress(GEM_KEY_LEFT);
                break;
            case 3:
                menu.registerKeyPress(GEM_KEY_RIGHT);
                break;
            case 4:
                menu.registerKeyPress(GEM_KEY_OK);
                break;
            case 5:
                menu.registerKeyPress(GEM_KEY_CANCEL);
                break;
        }
    }
}
