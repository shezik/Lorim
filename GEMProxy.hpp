#pragma once

#include <GEM_u8g2.h>
#include "SpicedU8g2.hpp"

class GEMProxy : public GEM_u8g2 {
    public:
        using GEM_u8g2::GEM_u8g2;
        //GEMProxy(U8G2& u8g2_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86) : GEM_u8g2(u8g2_, menuPointerType_, menuItemsPerScreen_, menuItemHeight_, menuPageScreenTopOffset_, menuValuesLeftOffset_), _u8g2(u8g2_) {};
        
        // I changed access control of GEM_u8g2 private members to protected, and declared drawMenu as virtual, for that I had no other choices;
        // nasty enough, this is the slightest change I could possibly think of.
        // Other attempts failed miserably, including attempting to wrap U8G2::sendBuffer() and U8G2::nextPage()
        void drawMenu() override {
            Serial.printf("GEMProxy: drawMenu() called!\n");  // debug
            #if 0  // ram shortage on neither ESP8266 nor ESP32
                _u8g2.firstPage();
                do {
                    drawTitleBar();
                    printMenuItems();
                    drawMenuPointer();
                    drawScrollbar();
                    static_cast<SpicedU8g2&>(_u8g2).drawElements(All, false);
                } while (_u8g2.nextPage());
            #else
                _u8g2.clearBuffer();
                drawTitleBar();
                printMenuItems();
                drawMenuPointer();
                drawScrollbar();
                static_cast<SpicedU8g2&>(_u8g2).drawElements(MsgCount, true);
                _u8g2.setDrawColor(1);  // revert back
                _u8g2.sendBuffer();
            #endif
        }
};
