#include "MultitapIM.hpp"

void MultitapIM::tick(int16_t keycode) {
    if (!str) return;

    if (charPending && keycode != 0 && lastKeycode != keycode) {
        Serial.printf("New key pressed\n");
        lastKeycode = keycode;
        charPending = false;
        pressCount = 0;
        if (pos + 2 != strSize) pos++;
    }

    if (charPending && millis() >= lastMillis + SELECTION_TIMEOUT) {
        Serial.printf("Time exceeded\n");
        charPending = false;
        pressCount = 0;
        if (pos + 2 != strSize) pos++;
    }

    if (keycode != 0) {
        char keyChar = parseKeyMapping(keyMapping, &pressCount, keycode, inputMode);
        Serial.printf("keyChar: %d\n", keyChar);
        switch (keyChar) {
            case '\x01':
                break;
            case '\x02':
                break;
            default:
                Serial.printf("pos: %d, strSize: %d\n", pos, strSize);
                if (pos + 1 < strSize) {
                    Serial.printf("keyChar registered\n");
                    str[pos] = keyChar;
                    if (pos == lastPos) lastChar = keyChar;  // !!
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

    if (showingCursor && pos != lastPos) {
        str[lastPos] = lastChar;
        lastPos = pos;
        tickCount = 0;
    }

    tickCount++;

    if (tickCount == 10) {
        lastChar = str[lastPos];
        str[lastPos] = '_';
        showingCursor = true;
    } else if (tickCount == 20) {
        tickCount = 0;
        str[lastPos] = lastChar;
        showingCursor = false;
    }

}

bool MultitapIM::bind(char* destStr, uint8_t size) {
    unbind();
    if (!destStr || size < 3) return false;

    for (maxPos = 0; destStr[maxPos] != '\0' && maxPos <= size - 2; maxPos++);
    if (maxPos > 0) maxPos--;
    destStr[size - 1] = '\0';

    Serial.printf("maxPos: %d\n", maxPos);

    str = destStr; strSize = size;
    pos = maxPos; lastPos = pos;
    showingCursor = false;
    pressCount = 0;
    lastMillis = 0; lastKeycode = 0;
    charPending = false;
    tickCount = 0;

    return true;
}

void MultitapIM::unbind() {
    if (!str) return;
    if (showingCursor) str[lastPos] = lastChar;
    str = nullptr;
}

char MultitapIM::parseKeyMapping(char* (*mapping)[3], uint8_t* count, int16_t key, uint8_t mode) {
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
