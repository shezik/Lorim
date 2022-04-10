/*
    TaskBase.hpp
    Copyright (C) 2022  shezik

    This program is licensed under GNU General Public License version 3, and
    is part of project Lorim.
    Please refer to Lorim.ino and LICENSE for details.
*/

#pragma once

#include <Arduino.h>

class TaskBase {
    public:
        virtual ~TaskBase() {};
        virtual void init() = 0;
        virtual void tick(int16_t keycode) = 0;
        virtual void refreshDisplay() = 0;
        virtual uint8_t getTaskID() = 0;
};
