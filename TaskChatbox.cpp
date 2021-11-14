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
    file = lilFS.open(HISTORY_PATH, "r");
    displayPage(file, filePos);
    file.close();
}

void TaskChatbox::tick(int16_t keycode) {
    switch (keycode) {
        case 0:
            file = lilFS.open(HISTORY_PATH, "r");
            filePos = seekToPrevLine(file, filePos, true);
            displayPage(file, filePos);
            file.close();
            break;
        case 1:
            file = lilFS.open(HISTORY_PATH, "r");
            filePos = seekToPrevLine(file, filePos, false);
            displayPage(file, filePos);
            file.close();
            break;
        case 7:
            parentManager.switchTo(ID_TASKGEM, true);
            break;
    }
}

void TaskChatbox::displayPage(File &file, uint16_t offset) {
    u8g2.clearBuffer();
    if (!file) {
        Serial.printf("File error!\n");
        return;
    }
    if (!file.seek(-offset, SeekEnd)) {
        Serial.printf("Failed to seek to offset %d!\n", offset);
        return;
    }
    Serial.printf("displayPage: Seeked to offset %d, offset: %d\n", file.position(), offset);
    uint8_t buf[CHARS_PER_LINE + 1];
    uint16_t availLen = seekToPrevPage(file);
    Serial.printf("availLen: %d\n", availLen);  // debug
    for (uint8_t j = 0; j < LINES_PER_PAGE && availLen > 0; j++) {
        uint8_t i = 0;
        while (i < CHARS_PER_LINE && availLen > 0) {
            uint8_t peekResult = file.peek();
            file.seek(1, SeekCur);
            if (peekResult != '\n') {
                buf[i] = peekResult;
                availLen--; i++;
            } else {
                break;
            }
            //Serial.printf("displayPage: pos: %d, availLen: %d, i: %d\n", file.position(), availLen, i);  // debug
        }
        buf[i] = '\0';
        u8g2.drawStr(0, j * 8, (const char*)buf);
    }
    file.seek(-offset, SeekEnd);  // go back to original position, necessary?
    u8g2.sendBuffer();
}

uint16_t TaskChatbox::seekToPrevPage(File &file) {  // returns printable char count, a subroutine of displayPage, file.position() dependent
    const uint16_t charsPerPage = CHARS_PER_LINE * LINES_PER_PAGE;
    uint16_t i = 0;
    while (i < charsPerPage) {
        //Serial.printf("seekToPrevPage: Seeking File Pos: %d\n", file.position());  // debug
        if (file.position() == 0 || (!file.seek(file.position() - 1)) || file.peek() == -1 /*!! last two are unhandled*/) break;
        if (file.peek() != '\n') i++;
    }
    Serial.printf("seekToPrevPage: Seeked File Pos: %d\n", file.position());  // debug
    return i;
}

uint16_t TaskChatbox::seekToPrevLine(File &file, uint16_t lastPos, bool direction) {  // returns position from the end of file
    if (!file.seek(-lastPos, SeekEnd)) {
        Serial.printf("Failed to seek to lastPos %d!\n", lastPos);
        return lastPos;
    }
    if (direction) {  // AT THIS POINT I DON'T REALLY CARE. I'LL BE REFACTORING VERY VERY SOON. THANK YOU FOR YOUR PATIENCE.
        for (uint16_t i = 0; i < CHARS_PER_LINE; i++) {
            Serial.printf("seekToPrevLine: Seeking File Pos: %d\n", file.position());  // debug
            if (file.position() == 0 || (!file.seek(file.position() - 1)) || file.peek() == -1) break;
            if (file.peek() == '\n') i = 0;
        }
    } else {
        for (uint16_t i = 0; i < CHARS_PER_LINE; i++) {
            Serial.printf("seekToPrevLine: Seeking File Pos: %d\n", file.position());  // debug
            if (file.position() == 0 || (!file.seek(1, SeekCur)) || file.peek() == -1) break;
            if (file.peek() == '\n') i = 0;
        }
    }
    Serial.printf("seekToPrevLine: Seeked File Pos: %d, File Size: %d, Return value: %d\n", file.position(), file.size(), (file.size() - file.position() - 1));  // debug
    return file.size() - file.position();
}
