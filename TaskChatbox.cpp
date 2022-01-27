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
}

void TaskChatbox::setDrawingStyle() {
    u8g2.setFont(CHATBOX_FONT);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
}

void TaskChatbox::init() {
    mailbox.clearNewMsgCount();
    u8g2.clear();
    setDrawingStyle();
    refreshDisplay(End);
}

void TaskChatbox::tick(int16_t keycode) {
    if (mailbox.getNewMsgCount()) {
        mailbox.clearNewMsgCount();
        refreshDisplay(End);
    }

    if (editMode) {
        switch (keycode) {
            case 13:
                Serial.printf("Edit cancelled\n");
                multitapIM->unbind();
                editMode = false;
                inputBuffer[0] = '\0';
                refreshDisplay(End);
                break;
            case 14:
                Serial.printf("Edit confirmed\n");
                multitapIM->unbind();
                editMode = false;
                mailbox.sendMessage(inputBuffer, mailbox.getNodeShortMac(), "Prototype");
                inputBuffer[0] = '\0';
                refreshDisplay(End);
                break;
            default:
                if (keycode != 0) Serial.printf("multitapIM->tick(%d)\n", keycode);
                multitapIM->tick(keycode);

                u8g2.setDrawColor(0);
                u8g2.drawBox(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE, u8g2.getDisplayWidth(), CHATBOX_VERTICAL_PACE);
                
                u8g2.setDrawColor(1);
                u8g2.drawHLine(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE - 1, u8g2.getDisplayWidth());
                u8g2.drawStr(0, u8g2.getDisplayHeight() - 1 - CHATBOX_VERTICAL_PACE, inputBuffer);

                u8g2.drawElements(StatusBar);
                setDrawingStyle();
                
                bool shouldUpdate = false;
                for (uint8_t i = 0; i < MAX_INPUT_LENGTH + 1; i++) {  // last byte is definitely '\0' though... doesn't matter.
                    if (oldInputBuffer[i] != inputBuffer[i]) {
                        oldInputBuffer[i] = inputBuffer[i];
                        shouldUpdate = true;
                    }
                }

                if (shouldUpdate)
                    u8g2.sendBuffer();
        }
        //Serial.printf("inputBuffer: %s\n", inputBuffer);
    } else {
        switch (keycode) {
            case 1:
                refreshDisplay(PrevLine);
                break;
            case 2:
                refreshDisplay(NextLine);
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

void TaskChatbox::refreshDisplay() {
    refreshDisplay(None);
}

void TaskChatbox::refreshDisplay(scrollDirection direction) {
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
        case Beginning:
            filePointer = 0;
            break;
        case End:
            filePointer = findPrevPage(file, file.size());
            break;
    }
    u8g2.clearBuffer();
    printPage(file, filePointer);
    u8g2.drawElements(StatusBar);
    setDrawingStyle();  // after calling drawElements()
    u8g2.sendBuffer();
    file.close();
}

uint16_t TaskChatbox::printPage(File &file, uint16_t _startPos) {
    uint16_t _endPos = _startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        _endPos = printLine(file, _endPos, i * CHATBOX_VERTICAL_PACE, true);
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
