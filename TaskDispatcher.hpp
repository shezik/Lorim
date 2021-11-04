#pragma once

#include "Lorim_definitions.hpp"
#include "Kbd_8x5_CH450.hpp"
#include "TaskGEM.hpp"

class TaskDispatcher {
    private:
        U8G2_DISPLAY_TYPE &u8g2;
        Kbd_8x5_CH450 &keyboard;
        TaskGEM *taskGEM;
        
        void allocateMem();
        void freeMem();
        void launchTask(uint8_t taskID);
        void deleteCurrentTask();

        uint8_t currentTask = ID_NONE;

    public:
        TaskDispatcher(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard);
        ~TaskDispatcher();
        void init();
        void tick();
        void switchTo(uint8_t taskID);
        uint8_t getCurrentTask();
};
