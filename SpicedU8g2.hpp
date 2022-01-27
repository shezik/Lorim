#pragma once

#include "Lorim_definitions.hpp"
#include <U8g2lib.h>
#include "Mailbox.hpp"
#include "TaskManager.hpp"

enum DrawType {
    StatusBar, MsgCount, All
};

class SpicedU8g2 : public U8G2_DISPLAY_TYPE {
    public:
        using U8G2_DISPLAY_TYPE::U8G2_DISPLAY_TYPE;
        void proxyInit(Mailbox* _mailbox, TaskManager* _taskman);
        void tick();
        void drawElements(DrawType type, bool doUpdate = true);
        void drawElements(DrawType type, char* _str, bool doUpdate = true);

    private:
        Mailbox* mailbox = nullptr;  // to get message count
        TaskManager* taskman = nullptr;  // to tell task to refresh screen

        uint8_t tickCount = 50;
        uint16_t newMsgCount = 0;
        uint16_t prevMsgCount = newMsgCount;

        void fetchData();
        void drawStatusBar(char* str);
        void drawMsgCount();
};
