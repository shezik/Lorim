/*
    TaskManager.cpp
    Copyright (C) 2022  shezik

    This program is licensed under GNU General Public License version 3, and
    is part of project Lorim.
    Please refer to Lorim.ino and LICENSE for details.
*/

#include "TaskManager.hpp"
#include "SpicedU8g2.hpp"
#include "TaskGEM.hpp"
#include "TaskChatbox.hpp"

TaskManager::TaskManager(SpicedU8g2 &_u8g2, Kbd_8x5_CH450 &_keyboard, Mailbox &_mailbox)
    : u8g2(_u8g2)
    , keyboard(_keyboard)
    , mailbox(_mailbox)
{
    // do nothing
}

TaskManager::~TaskManager() {
    deleteCurrentTask(false);
}

void TaskManager::init() {
    pinMode(CH450_INT, INPUT);
    keyboard.init();
}

void TaskManager::tick() {

    int16_t keycode = 0;  // pass in 0 if no key is down

    static bool processKey = false;
    static uint32_t lastMillis = millis();

    if (!digitalRead(CH450_INT)) {  // active low
        keyboard.getKeyData();  // clear CH450 interrupt state
        Serial.printf("new key pressed\n");  // debug
        lastMillis = millis();
        processKey = true;
    } else if (processKey) {
        if (millis() - lastMillis >= LONG_PRESS_DURATION) {
            keycode = -(Kbd_8x5_CH450::toKeycode(keyboard.getKeyData()));
            processKey = false;
            Serial.printf("long press registered, keycode is %d\n", keycode);  // debug
        } else {
            uint8_t keyData = keyboard.getKeyData();
            if (!Kbd_8x5_CH450::toState(keyData)) {
                keycode = Kbd_8x5_CH450::toKeycode(keyData);
                processKey = false;
                Serial.printf("short press registered, keycode is %d\n", keycode);  // debug
            }
        }
    }

    if (deleteTaskNextTick) {
        deleteCurrentTask(false);
        deleteTaskNextTick = false;
    }
    if (launchTaskNextTick) {
        launchTask(launchingTask, false);
        launchTaskNextTick = false;
    }

    if (currentTask) {
        currentTask->tick(keycode);
    } else {
        launchTask(ID_DEFAULT, false);
    }

}

void TaskManager::launchTask(uint8_t taskID, bool nextTick) {  // register new tasks here
    if (nextTick) {
        launchTaskNextTick = true;
        launchingTask = taskID;
        return;
    }
    switch (taskID) {
        case ID_TASKGEM:
            currentTask = new TaskGEM(*this, u8g2, mailbox);
            currentTask->init();
            break;
        case ID_CHATBOX:
            currentTask = new TaskChatbox(*this, u8g2, mailbox);
            currentTask->init();
            break;
    }
}

void TaskManager::deleteCurrentTask(bool nextTick) {
    if (nextTick) {
        deleteTaskNextTick = true;
        return;
    }
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

void TaskManager::switchTo(uint8_t taskID, bool nextTick) {
    deleteCurrentTask(nextTick);
    launchTask(taskID, nextTick);
}

void TaskManager::refreshDisplay() {
    if (currentTask) {
        currentTask->refreshDisplay();
    }
}
