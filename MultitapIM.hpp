#pragma once

#include <Arduino.h>
#include "Lorim_definitions.hpp"

class MultitapIM {
    private:
    //char inputString[MAX_INPUT_LENGTH + 2];  // cursor and terminator
    char* str = nullptr;
    uint8_t strSize;
    uint8_t pos;
    uint8_t maxPos;  // maxPos + 1 = inputLength
    char* keyMapping[4][3] = {  // syntax: (lowercase)\n(uppercase)\r(long-press)
        {".,?!():\"\r1",  "abc\nABC\r2", "def\nDEF\r3"},
        {"ghi\nGHI\r4",   "jkl\nJKL\r5", "mno\nMNO\r6"},
        {"pqrs\nPQRS\r7", "tuv\nTUV\r8", "wxyz\nWXYZ\r9"},
        {"\x01\r*",       " \r0",        "\x02\r#"}
    };
    uint8_t inputMode = 0;  // 0 - abc
                            // 1 - Abc
                            // 2 - ABC
                            // 3 - 123
    bool charPending;  // pending selection or not
    uint8_t pressCount;
    uint32_t lastMillis;
    int8_t lastKeycode;
    uint8_t lastPos;
    uint8_t tickCount;
    char lastChar;

    char parseKeyMapping(char* (*mapping)[3], uint8_t* count, int8_t key, uint8_t mode);

    public:
    void tick(int8_t keycode);
    bool bind(char* destStr, uint8_t size);
    void unbind();
    char* getInputModeAsStr();
    // void getInputString(char *dest, uint8_t size);
    // uint8_t getInputLength();
};
