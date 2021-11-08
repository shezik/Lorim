#pragma once

class TaskBase {
    public:
        virtual ~TaskBase() {};
        virtual void init() = 0;
        virtual void tick(short keycode) = 0;
        virtual uint8_t getTaskID() = 0;
};
