#include "TaskDispatcher.hpp"

TaskDispatcher::TaskDispatcher(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard)
    : u8g2(_u8g2)
    , keyboard(_keyboard)
{
    allocateMem();
}

TaskDispatcher::~TaskDispatcher() {
    freeMem();
    deleteCurrentTask();
}

void TaskDispatcher::allocateMem() {
    //taskGEM = new TaskGEM(*this, u8g2);  // misplaced!!
}

void TaskDispatcher::freeMem() {
    /*
    if (taskGEM != NULL) {
        delete taskGEM;
        taskGEM = NULL;
    }
    */
}

void TaskDispatcher::init() {
    pinMode(CH450_INT, INPUT);
    keyboard.init();
}

void TaskDispatcher::tick() {
    short keycode = -1;  // pass in -1 if no key is down
    if (!digitalRead(CH450_INT)) {
        keycode = Kbd_8x5_CH450::toKeycode(keyboard.getKeyData());
    }

    switch (currentTask) {
        case ID_NONE:  // if no match in launchTask
            launchTask(DEFAULT_TASK);
            break;
        case ID_TASKGEM:
            //if (taskGEM != NULL) {
                taskGEM->tick(keycode);
            //}
            break;
    }
}

void TaskDispatcher::launchTask(uint8_t taskID) {
    switch (taskID) {
        case ID_TASKGEM:
            if (currentTask == ID_NONE /*&& taskGEM == NULL*/) {
                taskGEM = new TaskGEM(*this, u8g2);
                taskGEM->init();
                currentTask = ID_TASKGEM;
            }
            break;
    }
}

void TaskDispatcher::deleteCurrentTask() {
    switch (currentTask) {
        case ID_TASKGEM:
            //if (taskGEM != NULL) {
                delete taskGEM;
                taskGEM = NULL;
                currentTask = ID_NONE;
            //}
            break;
    }
}

uint8_t TaskDispatcher::getCurrentTask() {
    return currentTask;
}

void TaskDispatcher::switchTo(uint8_t taskID) {
    deleteCurrentTask();
    launchTask(taskID);
}
