#pragma once

#include "Lorim_definitions.hpp"
#include "Kbd_8x5_CH450.hpp"
#include "TaskGEM.hpp"
#include "Mailbox.hpp"

class TaskManager {
    private:
        U8G2_DISPLAY_TYPE &u8g2;
        Kbd_8x5_CH450 &keyboard;
        Mailbox &mailbox;

        bool taskDeletionScheduled = false;
        
        void allocateMem();
        void freeMem();
        void launchTask(uint8_t taskID);
        void deleteCurrentTask();
        
        TaskBase *currentTask = nullptr;

    public:
        TaskManager(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard, Mailbox &_mailbox);
        ~TaskManager();
        void init();
        void tick();
        void switchTo(uint8_t taskID);
        uint8_t getCurrentTaskID();
};
