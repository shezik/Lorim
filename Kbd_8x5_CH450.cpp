/*
    Kbd_8x5_CH450.cpp: CH450 custom IIC driver
    Copyright (C) 2022  shezik

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Kbd_8x5_CH450.hpp"

Kbd_8x5_CH450::Kbd_8x5_CH450(uint8_t sda_, uint8_t scl_, unsigned int freq_)
    : sda(sda_)
    , scl(scl_)
    , delayms(500 / freq_)
{
    // do nothing
}

void Kbd_8x5_CH450::startComm() {
    digitalWrite(sda, HIGH);
    digitalWrite(scl, HIGH);
    digitalWrite(sda, LOW);
    delay(delayms);
    digitalWrite(scl, LOW);
    delay(delayms);
}

void Kbd_8x5_CH450::stopComm() {
    //digitalWrite(scl, LOW);  // if scl is set to low after each operation
    //delay(10);               // we won't be needing this part
    digitalWrite(sda, LOW);
    digitalWrite(scl, HIGH);  // stays high during inactive?
    delay(delayms);
    digitalWrite(sda, HIGH);
    delay(delayms);
}

bool Kbd_8x5_CH450::writeByte(uint8_t data) {
    for (int8_t i = 7; i > -1; i--) {
        if (data & (1 << i)) {
            digitalWrite(sda, HIGH);
        } else {
            digitalWrite(sda, LOW);
        }
        digitalWrite(scl, HIGH);
        delay(delayms);
        digitalWrite(scl, LOW);
        delay(delayms);
    }
    pinMode(sda, INPUT_PULLUP);
    digitalWrite(scl, HIGH);
    delay(delayms);
    bool result = digitalRead(sda);
    digitalWrite(scl, LOW);
    pinMode(sda, OUTPUT);
    digitalWrite(sda, LOW);  // terminal state: sda = low, scl = low
    //Serial.printf("writeByte result is %d\n", result); //debug
    delay(delayms);  // !!
    return result;
}

uint8_t Kbd_8x5_CH450::readByte() {
    uint8_t data = 0;
    pinMode(sda, INPUT_PULLUP);
    for (int8_t i = 7; i > -1; i--) {
        digitalWrite(scl, HIGH);
        delay(delayms);
        if (digitalRead(sda)) {
            //Serial.println("We've got something"); //debug
            data |= (1 << i);
        }
        digitalWrite(scl, LOW);
        delay(delayms);
    }
    pinMode(sda, OUTPUT);
    digitalWrite(sda, HIGH);
    //delay(10);
    digitalWrite(scl, HIGH);
    delay(delayms);
    digitalWrite(scl, LOW);
    delay(delayms);
    return data;
}

bool Kbd_8x5_CH450::init() {

    pinMode(sda, OUTPUT);  // Always change pinMode back to OUTPUT state after fiddling 
    pinMode(scl, OUTPUT);  // with it (changing its mode). We assume it is at OUTPUT state by default.
    
    startComm();
    bool resultA = writeByte(0b01001000);  // magic byte to change settings
    bool resultB = writeByte(0b00000010);  // 0: disable sleep; 00: full intensity (useless here);
                                           // 000: separator; 1: enable keyboard scanner; 0: disable LED segments driver
    stopComm();
    
    return resultA && resultB;

}

uint8_t Kbd_8x5_CH450::getKeyData() {
    startComm();
    bool resultA = writeByte(0b01001111);  // magic byte to request key data
    uint8_t resultB = readByte();
    stopComm();
    return resultA ? resultB : 0;   
}

bool Kbd_8x5_CH450::toState(uint8_t rawdata) {
    return rawdata & 0b01000000;
}

uint8_t Kbd_8x5_CH450::toKeycode(uint8_t rawdata) {

    // !! Edit your key bindings here
    static const int8_t keycodeArray[8][6] = {
        { 1,  2,  3, 13, 0, 0},
        { 4,  5,  6, 14, 0, 0},
        { 7,  8,  9, 15, 0, 0},
        {10, 11, 12, 16, 0, 0},
        { 0,  0,  0,  0, 0, 0},
        { 0,  0,  0,  0, 0, 0},
        { 0,  0,  0,  0, 0, 0},
        { 0,  0,  0,  0, 0, 0}
    };

    uint8_t row = (rawdata & 0b00111000) >> 3;  // SEG
    uint8_t col = (rawdata & 0b00000111) - 2;   // DIG

    if (row < 8 && col < 6) {
        // return row * 6 + col + 1;
        return keycodeArray[row][col];
    } else {
        return 0;
    }

}
