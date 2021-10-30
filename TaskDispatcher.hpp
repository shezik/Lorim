#ifndef __LORIM_TASKDPTCHR__
#define __LORIM_TASKDPTCHR__

#include "Lorim_definitions.hpp"
#include "Kbd_8x5_CH450.hpp"
#include "TaskGEM.hpp"

class TaskDispatcher {
    private:
        U8G2_DISPLAY_TYPE &u8g2;
        Kbd_8x5_CH450 &keyboard;
        TaskGEM *taskGEM;
    public:
        TaskDispatcher(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard);
        void tick();
};

#endif
