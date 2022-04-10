/*
    Mailbox.hpp
    Copyright (C) 2022  shezik

    This program is licensed under GNU General Public License version 3, and
    is part of project Lorim.
    Please refer to Lorim.ino and LICENSE for details.
*/

#pragma once

#include "Lorim_definitions.hpp"
#include "LoRaLayer2.h"
#include <FS.h>

class Mailbox {
    private:
        FS &lilFS;
        Layer1Class *Layer1 = nullptr;
        LL2Class *LL2 = nullptr;
        uint16_t newMsgCount = 0;
        void appendToHistory(char *message, uint8_t *address, char *username);
        uint8_t *nodeShortMac = nullptr;

    public:
        Mailbox(FS &_littleFS);
        ~Mailbox();
        void init(Layer1Class *_Layer1, LL2Class *_LL2, uint8_t *_nodeShortMac);
        void tick();
        void sendMessage(char* message, uint8_t *myAddress, char *myUsername, uint8_t *destination = BROADCAST);
        uint8_t* getNodeShortMac();
        uint16_t getNewMsgCount();
        void clearNewMsgCount();
        FS& getFSReference();
};
