#pragma once

#include "Lorim_definitions.hpp"
#include "Kbd_8x5_CH450.hpp"
#include <U8g2lib.h>
#include "Mailbox.hpp"
#include "TaskBase.hpp"

class TaskGEM;
class TaskChatbox;

class TaskManager {
    private:
        U8G2_DISPLAY_TYPE &u8g2;
        Kbd_8x5_CH450 &keyboard;
        Mailbox &mailbox;
        
        void launchTask(uint8_t taskID, bool nextTick);
        void deleteCurrentTask(bool nextTick);
        
        bool deleteTaskNextTick = false;
        bool launchTaskNextTick = false;
        uint8_t launchingTask;

        TaskBase *currentTask = nullptr;

    public:
        TaskManager(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard, Mailbox &_mailbox);
        ~TaskManager();
        void init();
        void tick();
        void switchTo(uint8_t taskID, bool nextTick);
        uint8_t getCurrentTaskID();
        void refreshDisplay();
};
