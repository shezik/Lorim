/*
    Kbd_8x5_CH450.hpp: CH450 custom IIC driver
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

#ifndef __Kbd_8x5_CH450__
#define __Kbd_8x5_CH450__

#include <Arduino.h>
#include <Wire.h>

class Kbd_8x5_CH450 {
    private:
        uint8_t sda;
        uint8_t scl;
        float delayms;
        void startComm();
        void stopComm();
        bool writeByte(uint8_t data);
        uint8_t readByte();
        
    public:
        /*
            @param 'sda_' I2C SDA
            @param 'scl_' I2C SCL
            @param 'freq_' (optional) Comm frequency
            Default 5000, aggressively 1E6
        */
        Kbd_8x5_CH450(uint8_t sda_, uint8_t scl_, unsigned int freq_ = 5000);
        bool keyIsDown = false;
        bool init();
        uint8_t getKeyData();
        static bool toState(uint8_t rawdata);
        static uint8_t toKeycode(uint8_t rawdata);
};

#endif
