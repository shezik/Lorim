/*
    TaskManager.hpp
    Copyright (C) 2022  shezik

    This program is licensed under GNU General Public License version 3, and
    is part of project Lorim.
    Please refer to Lorim.ino and LICENSE for details.
*/

#pragma once

#include "Lorim_definitions.hpp"
#include "Kbd_8x5_CH450.hpp"
#include "Mailbox.hpp"
#include "TaskBase.hpp"

class SpicedU8g2;
class TaskGEM;
class TaskChatbox;

class TaskManager {
    private:
        SpicedU8g2 &u8g2;
        Kbd_8x5_CH450 &keyboard;
        Mailbox &mailbox;
        
        void launchTask(uint8_t taskID, bool nextTick);
        void deleteCurrentTask(bool nextTick);
        
        bool deleteTaskNextTick = false;
        bool launchTaskNextTick = false;
        uint8_t launchingTask;

        TaskBase *currentTask = nullptr;

    public:
        TaskManager(SpicedU8g2 &_u8g2, Kbd_8x5_CH450 &_keyboard, Mailbox &_mailbox);
        ~TaskManager();
        void init();
        void tick();
        void switchTo(uint8_t taskID, bool nextTick);
        uint8_t getCurrentTaskID();
        void refreshDisplay();
};
