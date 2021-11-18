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

}

void TaskChatbox::freeMem() {

}

void TaskChatbox::init() {
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.clear();
    file = lilFS.open(HISTORY_PATH, "r");
    endPos = printPage(file, startPos);
    file.close();
}

void TaskChatbox::tick(int16_t keycode) {
    switch (keycode) {
        case 0:
            file = lilFS.open(HISTORY_PATH, "r");
            
            file.close();
            break;
        case 1:
            file = lilFS.open(HISTORY_PATH, "r");
            
            file.close();
            break;
        case 7:
            parentManager.switchTo(ID_TASKGEM, true);
            break;
    }
}

uint16_t TaskChatbox::printPage(File &file, uint16_t _startPos) {
    u8g2.clearBuffer();
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = printLine(file, _endPos, (i + 1) * 8);
    }
    u8g2.sendBuffer();
    return _endPos;
}

uint16_t TaskChatbox::printLine(File &file, uint16_t _startPos, uint8_t y) {
    file.seek(_startPos);
    uint8_t buf[CHARS_PER_LINE + 1];
    uint8_t i = 0;

    auto advanceCur = [&file, &buf]()->bool {
        if (file.position() == file.size() || !file.seek(1, SeekCur)) {  // attempt to move cursor forward by 1
            return false;
        }
        return true;
    };

    while (1) {
        if (i == CHARS_PER_LINE) {
            buf[i] = '\0';
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
    u8g2.drawStr(0, y, (const char*)buf);

    return file.position();
}
