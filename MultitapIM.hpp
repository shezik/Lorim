/*
    MultitapIM.hpp: Multi-tap input method for 4x3 matrix keyboards
    Copyright (C) 2022  shezik

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
        "def\nDEF\r3",   "abc\nABC\r2", ".,?!():\"\n1",
        "mno\nMNO\r6",   "jkl\nJKL\r5", "ghi\nGHI\r4",
        "wxyz\nWXYZ\r9", "tuv\nTUV\r8", "pqrs\nPQRS\r7",
        "\x02\n#",       " \n0",        "\x01\n*"
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
    uint16_t tickCount;
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
