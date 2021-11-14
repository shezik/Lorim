#pragma once

#include "Lorim_definitions.hpp"
#include <GEM_u8g2.h>
#include "TaskBase.hpp"
#include "Mailbox.hpp"
#include "TaskManager.hpp"

class TaskChatbox : public TaskBase {
    private:
        TaskManager &parentManager;
        U8G2_DISPLAY_TYPE &u8g2;
        Mailbox &mailbox;
        FS &lilFS;
        File file;

        void allocateMem();
        void freeMem();

        uint16_t filePos = 0;  // counting from the end of file

        void displayPage(File &file, uint16_t offset);
        uint16_t seekToPrevPage(File &file);
        uint16_t seekToPrevLine(File &file, uint16_t lastPos, bool direction);

    public:
        TaskChatbox(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox);
        ~TaskChatbox() override;
        void init() override;
        void tick(int16_t keycode) override;
        uint8_t getTaskID() override {return ID_CHATBOX;}

};
