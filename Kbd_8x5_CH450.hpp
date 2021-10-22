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
        bool keyIsDown = false;
        Kbd_8x5_CH450(uint8_t sda_, uint8_t scl_, unsigned int freq_ = 5000);
        bool init();
        uint8_t getKeyData();
        static bool toState(uint8_t rawdata);
        static uint8_t toKeycode(uint8_t rawdata);
};

#endif
