#pragma once

#include <Arduino.h>
#include "Lorim_definitions.hpp"

class MultitapIM {
    private:
    char* str = nullptr;
    uint8_t strSize;
    uint8_t pos;
    int16_t maxPos;  // pos of last non-\0 character
    char* keyMapping[12] = {    // syntax:  
                                // 1. (lowercase)\n(uppercase)\r(long-press);
                                // 2. (lowercase)\n(long-press);
                                // 3. (uppercase)\r(long-press);
                                // 4. long-presses can be removed too.
        ".,?!():\"\n1",  "abc\nABC\r2", "def\nDEF\r3",
        "ghi\nGHI\r4",   "jkl\nJKL\r5", "mno\nMNO\r6",
        "pqrs\nPQRS\r7", "tuv\nTUV\r8", "wxyz\nWXYZ\r9",
        "\x01\n*",       " \n0",        "\x02\n#"
    };
    uint8_t inputMode = 0;  // 0 - abc
                            // 1 - Abc
                            // 2 - ABC
                            // 3 - 123
    bool charPending;  // pending selection or not
    uint8_t pressCount;
    uint32_t lastMillis;
    int16_t lastKeycode;
    uint8_t lastPos;
    uint8_t tickCount;
    char lastChar;
    bool showingCursor;  // !! necessary?

    char parseKeyMapping(char** mapping, uint8_t* count, int16_t key, uint8_t mode);

    public:
    void tick(int16_t keycode);
    bool bind(char* destStr, uint8_t size);
    void unbind();
    char* getInputModeAsStr();
    // uint8_t getInputLength();
};
