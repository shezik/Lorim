#include "TaskManager.hpp"

TaskManager::TaskManager(U8G2_DISPLAY_TYPE &_u8g2, Kbd_8x5_CH450 &_keyboard, Mailbox &_mailbox)
    : u8g2(_u8g2)
    , keyboard(_keyboard)
    , mailbox(_mailbox)
{
    allocateMem();
}

TaskManager::~TaskManager() {
    freeMem();
}

void TaskManager::allocateMem() {
    
}

void TaskManager::freeMem() {
   deleteCurrentTask();
}

void TaskManager::init() {
    pinMode(CH450_INT, INPUT);
    keyboard.init();
}

void TaskManager::tick() {
    int16_t keycode = -1;  // pass in -1 if no key is down

    if (!digitalRead(CH450_INT)) {  // active low
        keycode = Kbd_8x5_CH450::toKeycode(keyboard.getKeyData());
    }

    if (currentTask) {
        currentTask->tick(keycode);
    } else {
        launchTask(ID_DEFAULT);
    }
}

void TaskManager::launchTask(uint8_t taskID) {  // register new tasks here
    switch (taskID) {
        case ID_TASKGEM:
            currentTask = new TaskGEM(*this, u8g2, mailbox);
            currentTask->init();
            break;
    }
}

void TaskManager::deleteCurrentTask() {
    if (currentTask) {
        delete currentTask; currentTask = nullptr;
    }
}

uint8_t TaskManager::getCurrentTaskID() {
    if (currentTask) {
        return currentTask->getTaskID();
    }
    return ID_NONE;
}

void TaskManager::switchTo(uint8_t taskID) {
    deleteCurrentTask();
    launchTask(taskID);
}
