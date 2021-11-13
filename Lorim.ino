#include "Lorim_definitions.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <GEM_u8g2.h>
#include <cstring>
#include "Layer1_LoRa.h"
#include "LoRaLayer2.h"
#include <FS.h>
#include <LittleFS.h>
#include "Kbd_8x5_CH450.hpp"
#include "TaskManager.hpp"
#include "Mailbox.hpp"

Layer1Class *Layer1;
LL2Class *LL2;

SPIClass LoRaHSPI(HSPI);
Kbd_8x5_CH450 keyboard(I2C_SDA, I2C_SCL, /*freq=1E6?*/5000);
U8G2_DISPLAY_TYPE u8g2(U8G2_R2, VSPI_CLK, VSPI_DATA, VSPI_CS, VSPI_DC, U8G2_DISPLAY_RESET);
Mailbox mailbox(LittleFS);
TaskManager taskManager(u8g2, keyboard, mailbox);

void initLL2();
void saveSettings();

void setup() {

    Serial.begin(115200);
    u8g2.begin();
    if (!LittleFS.begin(/*FORMAT_LITTLEFS_IF_FAILED*/)) {
        LittleFS.format();
        Serial.printf("Formatted LittleFS\n");
        LittleFS.begin();
    }
    initLL2();
    mailbox.init(Layer1, LL2);
    taskManager.init();

}

void loop() {
    taskManager.tick();
    mailbox.tick();
}

void initLL2() {
    // The following 4 lines are copied from disaster.radio
    char nodeAddress[ADDR_LENGTH*2 + 1] = {'\0'};
    uint8_t mac[6];
    WiFi.macAddress(mac);
    sprintf(nodeAddress, "%02x%02x%02x%02x", mac[2], mac[3], mac[4], mac[5]);
    
    Layer1 = new Layer1Class(LoRaHSPI);
    LL2 = new LL2Class(Layer1);
    Layer1->setPins(LORA_CS, LORA_RST, LORA_DIO);
    Layer1->setLoRaFrequency(LORA_FREQ);
    Layer1->init();
    LL2->setLocalAddress(nodeAddress);
    LL2->init();
}

void saveSettings() {
    
}
