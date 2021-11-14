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
    File file = lilFS.open(HISTORY_PATH, "r");
    uint8_t buf[CHARS_PER_LINE + 1];
    if (!file) {
        Serial.printf("File error!\n");
    }
    file.seek(0, SeekEnd);
    uint16_t availLen = seekToPrevPage(file);
/*debug*/Serial.printf("availLen: %d\n", availLen);
    for (uint8_t j = 0; j < LINES_PER_PAGE && availLen > 0; j++) {
        uint8_t i = 0;
        while (i < CHARS_PER_LINE && availLen > 0) {
            uint8_t peekResult = file.peek();
            if (peekResult != '\n') {
                buf[i] = peekResult;
                availLen--; i++;
/*debug*/       Serial.printf("availLen: %d, i: %d\n", availLen, i);
            }
/*debug*/   Serial.printf("pos: %d\n", file.position());
            file.seek(1, SeekCur);
        }
        buf[i] = '\0';
        u8g2.drawStr(0, (j + 1) * 8, (const char*)buf);
    }
    u8g2.sendBuffer();
    file.close();
}

void TaskChatbox::tick(int16_t keycode) {
    if (keycode != -1) {
        parentManager.switchTo(ID_TASKGEM, true);
    }
}

uint16_t TaskChatbox::seekToPrevPage(File &file) {
    const uint16_t charsPerPage = CHARS_PER_LINE * LINES_PER_PAGE;
    uint16_t i = 0;
    while (i < charsPerPage) {
/*debug*/Serial.printf("Seeking File Pos: %d\n", file.position());
        if (file.position() == 0 || (!file.seek(file.position() - 1)) || file.peek() == -1 /*!! last two are unhandled*/) break;
        if (file.peek() != '\n') i++;
    }
/*debug*/Serial.printf("Seeked File Pos: %d\n", file.position());
    return i;
}
