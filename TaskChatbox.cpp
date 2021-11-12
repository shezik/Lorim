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
    file.seek(0, SeekEnd);
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        uint8_t len = seekPrevLine(file);
        uint32_t lastPos = file.position();
        file.read(buf, len);
        buf[len] = '\0';
        u8g2.drawStr(0, (i + 1) * 8, (const char*)buf);
        file.seek(lastPos + 1, SeekEnd);
    }
    file.close();
}

void TaskChatbox::tick(int16_t keycode) {

}

uint8_t TaskChatbox::seekPrevLine(File &file) {
    uint32_t offset = file.position();
    uint8_t i;
    for (i = 1; i < CHARS_PER_LINE; i++) {
        if (file.peek() != '\n') break;
        if (!file.seek(offset + i, SeekEnd)) break;
    }
    return i;
}
