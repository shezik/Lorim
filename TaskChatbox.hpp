#pragma once

#include "Lorim_definitions.hpp"
#include "TaskBase.hpp"
#include "Mailbox.hpp"
#include "TaskManager.hpp"
#include "MultitapIM.hpp"

class TaskChatbox : public TaskBase {
    private:
        TaskManager &parentManager;
        U8G2_DISPLAY_TYPE &u8g2;
        Mailbox &mailbox;
        FS &lilFS;
        File file;
        MultitapIM *multitapIM;

        void allocateMem();
        void freeMem();

        // left and right border of printPage file position range
        uint16_t startPos = 0;
        uint16_t endPos;

        char inputBuffer[MAX_INPUT_LENGTH + 1] = {'\0'};
        bool editMode = false;

        uint16_t printPage(File &file, uint16_t _startPos);
        uint16_t printLine(File &file, uint16_t _startPos, uint8_t y, bool doPrint);
        uint16_t findPrevLine(File &file, uint16_t _startPos);
        uint16_t findNextLine(File &file, uint16_t _startPos);
        uint16_t findPrevPage(File &file, uint16_t _startPos);

    public:
        TaskChatbox(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox);
        ~TaskChatbox() override;
        void init() override;
        void tick(int16_t keycode) override;
        void refreshDisplay() override;
        uint8_t getTaskID() override {return ID_CHATBOX;}

};
