#include "MultitapIM.hpp"

void MultitapIM::tick(int16_t keycode) {
    if (!str) return;

    /*
    if (charPending && keycode != 0 && lastKeycode != keycode) {
        Serial.printf("New key pressed, lastKeycode is %d, new keycode is %d\n", lastKeycode, keycode);
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
    */

    // simplified
    if (charPending && ((keycode != 0 && lastKeycode != keycode) || (millis() >= lastMillis + SELECTION_TIMEOUT))) {
        charPending = false;
        pressCount = 0;
        if (pos + 2 != strSize) pos++;
    }

    if (keycode != 0) {
        char keyChar = parseKeyMapping(keyMapping, &pressCount, keycode, inputMode);
        Serial.printf("keyChar: %d\n", keyChar);
        switch (keyChar) {
            case '\0':  // stop pending prematurely
                if (pos + 1 < strSize) {
                    lastKeycode = keycode;
                    lastMillis = millis();
                    charPending = true;
                }
                break;
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
                    lastKeycode = keycode;
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

char MultitapIM::parseKeyMapping(char** mapping, uint8_t* count, int16_t key, uint8_t mode) {  // !! awaiting optimization
    if (key > 12 || key < -12 || key == 0) return '?';

    if (key < 0) {key = -key; mode = 3;}  // !! expand later

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

    switch (inputMode) {
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
