#include "TaskChatbox.hpp"

TaskChatbox::TaskChatbox(TaskManager &_parentManager, SpicedU8g2 &_u8g2, Mailbox &_mailbox)
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
    u8g2.enableAutoRefresh(true);
}

void TaskChatbox::setDrawingStyle() {
    u8g2.setFont(CHATBOX_FONT);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
}

void TaskChatbox::quitEditMode(bool sendMsg) {
    multitapIM->unbind();
    editMode = false;
    if (sendMsg)
        mailbox.sendMessage(inputBuffer, mailbox.getNodeShortMac(), "Prototype");
    inputBuffer[0] = '\0';
    drawDisplay(End, true, true);
}

void TaskChatbox::init() {
    u8g2.enableAutoRefresh(false);  // we will handle updates by ourselves
    mailbox.clearNewMsgCount();
    u8g2.clear();
    setDrawingStyle();
    drawDisplay(End, true, true);
}

void TaskChatbox::tick(int16_t keycode) {
    bool shouldUpdate = false;
    static bool browsingHistory = false;
    static uint16_t prevMsgCount = 0;
    uint16_t msgCount = mailbox.getNewMsgCount();

    if (!browsingHistory && (pageEndPointer < fileSizeBuf)) {
        browsingHistory = true;
        prevMsgCount = 0;
    } else if (browsingHistory && (pageEndPointer >= fileSizeBuf)) {
        browsingHistory = false;
        if (prevMsgCount)
            shouldUpdate = true;
    }

    if (browsingHistory) {
        if (prevMsgCount != msgCount) {
            prevMsgCount = msgCount;
            shouldUpdate = true;  // not scroll and draw message count
        }
    } else if (msgCount) {
        mailbox.clearNewMsgCount();
        shouldUpdate = true;  // go to bottom and not draw message count
    }

    if (editMode) {
        switch (keycode) {
            case 14:
                Serial.printf("Edit cancelled\n");
                quitEditMode(false);
                break;
            case 13:
                Serial.printf("Edit confirmed\n");
                quitEditMode(true);
                break;
            default:
                if (keycode) Serial.printf("multitapIM->tick(%d) executed\n", keycode);  // debug
                multitapIM->tick(keycode);

                for (uint8_t i = 0; i < MAX_INPUT_LENGTH + 1; i++) {  // last byte is definitely '\0' though... doesn't matter.
                    if (oldInputBuffer[i] != inputBuffer[i]) {
                        oldInputBuffer[i] = inputBuffer[i];
                        Serial.printf("TaskChatbox: inputBuffer difference found!\n");  // debug
                        shouldUpdate = true;
                    }
                }

                if (shouldUpdate) {
                    //Serial.printf("inputBuffer: %s\n", inputBuffer);
                    drawDisplay(End, true, false);  // don't send buffer yet
                    u8g2.setDrawColor(0);
                    u8g2.drawBox(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE, u8g2.getDisplayWidth(), CHATBOX_VERTICAL_PACE);
                    u8g2.setDrawColor(1);
                    u8g2.drawHLine(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE - 1, u8g2.getDisplayWidth());
                    u8g2.drawStr(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE, inputBuffer);
                    u8g2.sendBuffer();
                }
        }
    } else {
        switch (keycode) {
            case 2:
                drawDisplay(PrevLine, true, true, browsingHistory);
                break;
            case 8:
                drawDisplay(NextLine, true, true, browsingHistory);
                break;
            case 6:
                drawDisplay(PrevPage, true, true, browsingHistory);
                break;
            case 4:
                drawDisplay(NextPage, true, true, browsingHistory);
                break;
            case 3:
                (lilFS.open(HISTORY_PATH, "w+")).close();  // clear history data
                drawDisplay(Beginning, true, true);
                break;
            case 5:
                Serial.printf("Entering edit mode: multitapIM->bind() returned %d\n", multitapIM->bind(inputBuffer, MAX_INPUT_LENGTH + 1));  // debug
                editMode = true;
                drawDisplay(End, true, false);
                break;
            case 1:
                parentManager.switchTo(ID_TASKGEM, true);
                break;
            default:
                //Serial.printf("TaskChatbox: shouldUpdate = %d, browsingHistory = %d\n", shouldUpdate, browsingHistory);  // debug
                if (shouldUpdate) {
                    /*
                    if (browsingHistory) {
                        drawDisplay(None, true, true, true);  // show new message count
                    } else {
                        drawDisplay(End, true, true);
                    }
                    */
                    drawDisplay(browsingHistory? None: End, true, true, browsingHistory);
                }
        }
    }
}

#pragma region

void TaskChatbox::refreshDisplay() {
    drawDisplay(None, true, true);
}

void TaskChatbox::drawDisplay(scrollDirection direction, bool doDrawElements, bool doSendBuffer, bool showMsgCount) {
    file = lilFS.open(HISTORY_PATH, "r");
    switch (direction) {
        case None:
            break;
        case PrevLine:
            filePointer = findPrevLine(file, filePointer);
            break;
        case NextLine:
            filePointer = findNextLine(file, filePointer);
            break;
        case PrevPage:
            filePointer = findPrevPage(file, filePointer);
            break;
        case NextPage:
            filePointer = findNextPage(file, filePointer);
            break;
        case Beginning:
            filePointer = 0;
            break;
        case End:
            filePointer = findPrevPage(file, file.size());
            break;
    }
    u8g2.clearBuffer();
    pageEndPointer = printPage(file, filePointer, true);

    if (direction == End) {
        fileSizeBuf = pageEndPointer;
    } else {
        fileSizeBuf = printPage(file, findPrevPage(file, file.size()), false);
    }

    if (doDrawElements) {
        u8g2.drawElements(showMsgCount? All: StatusBar);
        setDrawingStyle();  // after calling drawElements()
    }
    if (doSendBuffer)
        u8g2.sendBuffer();
    file.close();

    Serial.printf("TaskChatbox: direction: %d, fileSizeBuf = %d, pageEndPointer = %d\n", direction, fileSizeBuf, pageEndPointer);  // debug
}

uint16_t TaskChatbox::printPage(File &file, uint16_t _startPos, bool doPrint) {
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = printLine(file, _endPos, i * CHATBOX_VERTICAL_PACE, doPrint);
    }
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
    //file.seek(_startPos);
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = findPrevLine(file, _endPos);
    }
    return _endPos;
}

uint16_t TaskChatbox::findNextPage(File &file, uint16_t _startPos) {
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = findNextLine(file, _startPos);
    }
    return _endPos;
}

#pragma endregion
