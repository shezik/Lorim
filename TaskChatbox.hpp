#pragma once

#include "Lorim_definitions.hpp"
#include "TaskBase.hpp"

class TaskChatbox : public TaskBase {
    private:
        TaskManager &parentManager;
        U8G2_DISPLAY_TYPE &u8g2;
        Mailbox &mailbox;

        void allocateMem();
        void freeMem();

    public:
        TaskChatbox(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox);
        ~TaskChatbox() override;
        void init() override;
        void tick(int16_t keycode) override;
        uint8_t getTaskID() override {return ID_CHATBOX;}

};
