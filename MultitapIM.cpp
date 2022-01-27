#include "MultitapIM.hpp"

// THIS IS SO FUCKING DIFFICULT TO GIVE MAINTENANCE TO

void MultitapIM::tick(int16_t keycode) {

    static auto processPending = [&](bool forceStop = false) {
        if (charPending && (forceStop || (keycode != 0 && lastKeycode != keycode) || (millis() >= lastMillis + SELECTION_TIMEOUT))) {
            charPending = false;
            pressCount = 0;
            if (pos + 1 != strSize - 1) pos++;
        }
    };

    if (!str) return;

    processPending();

    if (keycode != 0) {
        char keyChar = parseKeyMapping(keyMapping, &pressCount, keycode, inputMode);
        Serial.printf("keyChar: %d\n", keyChar);
        switch (keyChar) {
            case '\0':
                processPending(true);  // stop pending prematurely
                break;
            case '\x01':
                processPending(true);
                if (pos > 0) pos--;
                break;
            case '\x02':
                processPending(true);
                if (pos <= maxPos) pos++;
                break;
            default:
                Serial.printf("pos: %d, strSize: %d\n", pos, strSize);
                if (pos < strSize - 1) {
                    Serial.printf("keyChar registered\n");
                    str[pos] = keyChar;
                    if (pos == lastPos) lastChar = keyChar;  // !!
                    if (pos >= maxPos) {
                        if (pos + 2 <= strSize - 1) str[pos + 2] = '\0';
                        maxPos = pos;
                    }
                    lastKeycode = keycode;
                    lastMillis = millis();
                    charPending = true;
                    pressCount++;
                }
        }
    }

    if (showingCursor && pos != lastPos) {
        showingCursor = false;
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
    if (maxPos + 1 <= size - 1) destStr[maxPos + 1] = '\0';
    /*if (maxPos > 0) */ maxPos--;
    destStr[size - 1] = '\0';

    Serial.printf("maxPos: %d\n", maxPos);

    str = destStr; strSize = size;
    pos = maxPos + 1; lastPos = pos;
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

char MultitapIM::parseKeyMapping(char** mapping, uint8_t* count, int16_t key, uint8_t mode) {  // !! awaiting optimization
    if (key > 12 || key < -12 || key == 0) return '?';

    int8_t prevMode = -1;
    if (key < 0) {key = -key; prevMode = mode; mode = 3;}  // !! expand later

    key--;

    int8_t lowercaseLen, uppercaseLen, longpressLen;
    uint8_t pos;
    
    for (lowercaseLen = 0; mapping[key][lowercaseLen] != '\n'; lowercaseLen++) {
        if (mapping[key][lowercaseLen] == '\0') {lowercaseLen = -1; break;}
    }
    for (uppercaseLen = 0; mapping[key][uppercaseLen + lowercaseLen + 1] != '\r'; uppercaseLen++) {
        if (mapping[key][uppercaseLen + lowercaseLen + 1] == '\0') {uppercaseLen = -1; break;}
    }
    for (longpressLen = 0; mapping[key][longpressLen + lowercaseLen + uppercaseLen + 2] != '\0'; longpressLen++);

    Serial.printf("lowercaseLen: %d, uppercaseLen: %d, longpressLen: %d\n", lowercaseLen, uppercaseLen, longpressLen);

    switch (mode) {
        case 0:
            if (lowercaseLen <= 0) {
                *count = 0;
                return '\0';
            } else {
                *count %= lowercaseLen;
                pos = *count;
            }
            break;
        case 1:
            // !! awaiting implementation
        case 2:
            if (uppercaseLen <= 0) {
                *count = 0;
                return '\0';
            } else {
                *count %= uppercaseLen;
                pos = lowercaseLen + 1 + *count;
            }
            break;
        case 3:
            if (longpressLen <= 0) {
                *count = 0;
                return '\0';
            } else {
                *count %= longpressLen;
                pos = lowercaseLen + uppercaseLen + 2 + *count;
                if (!prevMode < 0) mode = prevMode;  // restore to state before long-press
            }
            break;
    }

    return mapping[key][pos];

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
