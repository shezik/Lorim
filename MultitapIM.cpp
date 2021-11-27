#include "MultitapIM.hpp"

void MultitapIM::tick(int8_t keycode) {
    if (!str) return;

    if (charPending && keycode != 0 && lastKeycode != keycode) {
        charPending = false;
        pressCount = 0;
        pos++;
        lastKeycode = keycode;
    }

    if (charPending && millis() >= lastMillis + SELECTION_TIMEOUT) {
        charPending = false;
        pressCount = 0;
        pos++;
    }

    if (keycode != 0) {
        char keyChar = parseKeyMapping(keyMapping, &pressCount, keycode, inputMode);

        switch (keyChar) {
            case '\x01':
                break;
            case '\x02':
                break;
            default:
                if (pos < strSize - 1) {
                    str[pos] = keyChar;
                    if (pos > maxPos) {
                        str[pos + 1] = '\0';
                        maxPos = pos;
                    }
                    lastMillis = millis();
                    charPending = true;
                    pressCount++;
                }
        }
    }

    if (pos != lastPos) {
        str[lastPos] = lastChar;
        lastPos = pos;
        lastChar = str[pos];
        tickCount = 0;
    }    

    if (tickCount == 10) {
        str[pos] = '_';
        tickCount++;
    } else if (tickCount == 20) {
        tickCount = 0;
        str[pos] = lastChar;
    }

}

bool MultitapIM::bind(char* destStr, uint8_t size) {
    unbind();
    if (!destStr || size < 3) return false;

    for (maxPos = 0; destStr[maxPos] == '\0' || maxPos == size - 1; maxPos++);
    if (maxPos > 0) maxPos--;
    destStr[size - 1] = '\0';

    str = destStr; strSize = size;
    pos = maxPos; lastPos = pos; lastChar = str[pos];
    pressCount = 0;
    lastMillis = 0; lastKeycode = 0;
    charPending = false;
    tickCount = 0;

    return true;
}

void MultitapIM::unbind() {
    if (!str) return;
    str[lastPos] = lastChar;
    str = nullptr;
}

char MultitapIM::parseKeyMapping(char* (*mapping)[3], uint8_t* count, int8_t key, uint8_t mode) {
    return 'x';  // !! awaiting implementation
}

char* MultitapIM::getInputModeAsStr() {
    static char buf[4];
    switch (inputMode) {
        case 0:
            memcpy(buf, "abc", 4);
            break;
        case 1:
            memcpy(buf, "Abc", 4);
            break;
        case 2:
            memcpy(buf, "ABC", 4);
            break;
        case 3:
            memcpy(buf, "123", 4);
            break;
    }
    return buf;
}
