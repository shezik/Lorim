#include "SpicedU8g2.hpp"

void SpicedU8g2::proxyInit(Mailbox* _mailbox, TaskManager* _taskman) {
    mailbox = _mailbox;
    taskman = _taskman;
}

void SpicedU8g2::drawElements(DrawType type, bool doUpdate) {
    Serial.printf("SpicedU8g2: drawElements() called!\n");  // debug

    tickCount = 0;
    if (doUpdate) fetchData();
    switch (type) {
        case StatusBar:
            drawStatusBar();
            break;
        case MsgCount:
            drawMsgCount();
            break;
        case All:
            drawStatusBar();
            drawMsgCount();
            break;
    }
}

void SpicedU8g2::tick() {
    if (tickCount > 99) {  // update every 100 ticks
        fetchData();
        tickCount = 0;
    } else tickCount++;

    if (newMsgCount != prevMsgCount) {
        Serial.printf("SpicedU8g2: newMsgCount updated to %d, previously %d\n", newMsgCount, prevMsgCount);  // debug
        prevMsgCount = newMsgCount;
        taskman->refreshDisplay();  // let task itself trigger drawElements()
    }
}

void SpicedU8g2::fetchData() {
    newMsgCount = mailbox->getNewMsgCount();
}

void SpicedU8g2::drawStatusBar() {
    //setFontMode(0);
    setDrawColor(1);
    drawBox(1, 1, getDisplayWidth() - 2, STATUSBAR_FONTHEIGHT + 2);
    setFont(STATUSBAR_FONT);
    //setFontMode(1);
    setDrawColor(0);
    drawStr(2, 1, "Lorim " LORIM_VERSION);
    setDrawColor(1);  // revert back
}

void SpicedU8g2::drawMsgCount() {
    if (!newMsgCount) return;  // nobody wants a zero on screen, right?

    char countStr[3] = "++";
    if (newMsgCount < 99) {
        sprintf(countStr, "%d", newMsgCount);
    }

    uint16_t x = getDisplayWidth() - 8;
    uint16_t y = getDisplayHeight() - 13;

    // adjust right margin according to digits
    if (newMsgCount > 9) x -= 6;

    setFont(u8g2_font_5x7_mn);
    //setFontMode(1);
    setDrawColor(1);
    drawButtonUTF8(x, y, U8G2_BTN_BW1, 0, 1, 1, countStr);
}
