#include "TaskChatbox.hpp"

TaskChatbox::TaskChatbox(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox)
    : parentManager(_parentManager)
    , u8g2(_u8g2)
    , mailbox(_mailbox)
    , lilFS(mailbox.getFSReference())
{
    allocateMem();
}

TaskChatbox::~TaskChatbox() {
    freeMem();
}

void TaskChatbox::allocateMem() {
    multitapIM = new MultitapIM();
}

void TaskChatbox::freeMem() {
    delete multitapIM; multitapIM = nullptr;
}

void TaskChatbox::init() {
    mailbox.clearNewMsgCount();
    u8g2.setFont(CHATBOX_FONT);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.clear();
    file = lilFS.open(HISTORY_PATH, "r");
    startPos = findPrevPage(file, file.size());
    endPos = printPage(file, startPos);
    file.close();
}

void TaskChatbox::tick(int16_t keycode) {
    if (editMode) {
        switch (keycode) {
            case 13:
                Serial.printf("Exiting to normal mode\n");
                multitapIM->unbind();
                editMode = false;
                break;
            default:
                if (keycode != 0) Serial.printf("multitapIM->tick(%d)\n", keycode);
                multitapIM->tick(keycode);
        }
        Serial.printf("inputBuffer: %s\n", inputBuffer);
    } else {
        switch (keycode) {
            case 1:
                file = lilFS.open(HISTORY_PATH, "r");
                startPos = findPrevLine(file, startPos);
                endPos = printPage(file, startPos);
                file.close();
                break;
            case 2:
                file = lilFS.open(HISTORY_PATH, "r");
                startPos = findNextLine(file, startPos);
                endPos = printPage(file, startPos);
                file.close();
                break;
            case 3:
                (lilFS.open(HISTORY_PATH, "w+")).close();  // clear history data
                break;
            case 4:
                Serial.printf("Entering edit mode\nmultitapIM->bind() returned %d\n", multitapIM->bind(inputBuffer, MAX_INPUT_LENGTH + 1));
                editMode = true;
                break;
            case 8:
                parentManager.switchTo(ID_TASKGEM, true);
                break;
        }
    }
}

uint16_t TaskChatbox::printPage(File &file, uint16_t _startPos) {
    u8g2.clearBuffer();
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        /*
        uint8_t lineNum[2];
        sprintf((char*)lineNum, "%d", i + 1);
        u8g2.drawStr(0, i * 8, (const char*)lineNum);
        */
        _endPos = printLine(file, _endPos, i * CHATBOX_VERTICAL_PACE, true);
    }
    u8g2.sendBuffer();
    return _endPos;
}

uint16_t TaskChatbox::printLine(File &file, uint16_t _startPos, uint8_t y, bool doPrint) {
    file.seek(_startPos);
    uint8_t buf[CHARS_PER_LINE + 1];
    uint8_t i = 0;

    static auto advanceCur = [&file]()->bool {
        if (file.position() == file.size() || !file.seek(1, SeekCur)) {  // attempt to move cursor forward by 1
            return false;
        }
        return true;
    };

    while (1) {
        if (i == CHARS_PER_LINE) {
            buf[i] = '\0';
            if (file.peek() == '\n') advanceCur();
            break;
        }
        if (file.peek() != '\n') {
            buf[i] = file.peek();
            if (!advanceCur()) {buf[i + 1] = '\0'; break;}
        } else {
            advanceCur();
            buf[i] = '\0';
            break;
        }
        i++;
    }

    if (doPrint) u8g2.drawStr(0, y, (const char*)buf);

    return file.position();
}

uint16_t TaskChatbox::findPrevLine(File &file, uint16_t _startPos) {
    file.seek(_startPos);
    uint16_t i = 0;  // variable i is unused for now

    static auto decreaseCur = [&file]()->bool {
        if (file.position() == 0 || !file.seek(-1, SeekCur)) {  // attempt to move cursor backwards by 1
            return false;
        }
        return true;
    };

    if (decreaseCur()) i++;  // skip a character in case it is a return 

    while (decreaseCur()) {  // get to prev return or beginning of file
        if (file.peek() == '\n') {
            file.seek(1, SeekCur);
            break;
        }
        i++;
    }

    uint16_t _endPos = file.position();
    uint16_t prevEndPos = _endPos;
    while (1) {
        _endPos = findNextLine(file, _endPos);
        if (_endPos >= _startPos) break;
        prevEndPos = _endPos;
    }

    // file.seek(i < CHARS_PER_LINE ? i : CHARS_PER_LINE, SeekCur);
    return prevEndPos;
}

uint16_t TaskChatbox::findNextLine(File &file, uint16_t _startPos) {
    return printLine(file, _startPos, 0, false);
}

uint16_t TaskChatbox::findPrevPage(File &file, uint16_t _startPos) {
    file.seek(_startPos);
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = findPrevLine(file, _endPos);
    }
    return _endPos;
}
