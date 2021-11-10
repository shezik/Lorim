#include "TaskGEM.hpp"

TaskGEM *TaskGEM::callbackContext = nullptr;  // init static variable and allocate memory otherwise you get linker errors

TaskGEM::TaskGEM(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox)
    : parentManager(_parentManager)
    , u8g2(_u8g2)
    , mailbox(_mailbox)
{
    callbackContext = this;
    allocateMem();
}

TaskGEM::~TaskGEM() {
    callbackContext = nullptr;  // set it to nullptr since callbackContext is a static variable.
    freeMem();
    u8g2.clear();
}

void TaskGEM::allocateMem() {
    menu = new GEM_u8g2(u8g2, /*menuPointerType=*/GEM_POINTER_DASH, /*menuItemsPerScreen=*/7, /*menuItemsPerScreen*/8, /*menuPageScreenTopOffset*/8, /*menuValuesLeftOffset*/80);
    pageMain = new GEMPage("Lorim v0.1");
    pageSettings = new GEMPage("Settings");
    pageItemMainSettings = new GEMItem("Settings", pageSettings);
    pageItemSettingsContrast = new GEMItem("Contrast:", displayContrast, setContrast_Callback);
}

void TaskGEM::freeMem() {
    delete menu; menu = nullptr;
    delete pageMain; pageMain = nullptr;
    delete pageSettings; pageSettings = nullptr;
    delete pageItemMainSettings; pageItemMainSettings = nullptr;
    delete pageItemSettingsContrast; pageItemSettingsContrast = nullptr;
}

void TaskGEM::init() {
    menu->init();

    // setup main page
    pageMain->addMenuItem(*pageItemMainSettings);
    menu->setMenuPageCurrent(*pageMain);
    // setup settings page
    pageSettings->addMenuItem(*pageItemSettingsContrast);
    pageSettings->setParentMenuPage(*pageMain);

    menu->drawMenu();
}

void TaskGEM::setContrast_Callback() {
    if (callbackContext) {  // this is static and could be called at any given time
        callbackContext->setContrast();
    }
}

void TaskGEM::setContrast() {
    if (displayContrast > 255) {displayContrast = 255;}
    else if (displayContrast < 0) {displayContrast = 0;}
    u8g2.setContrast(displayContrast);
}

void TaskGEM::tick(int16_t keycode) {

    bool validKeyPressed = false;

    if (menu->readyForKey()) {
        validKeyPressed = true;
        if (keycode != -1) {  // debug
            Serial.printf("Registered keycode: %d\n", keycode);
        }
        switch (keycode) {
            case 0:
                menu->registerKeyPress(GEM_KEY_UP);
                break;
            case 1:
                menu->registerKeyPress(GEM_KEY_DOWN);
                break;
            case 2:
                menu->registerKeyPress(GEM_KEY_LEFT);
                break;
            case 3:
                menu->registerKeyPress(GEM_KEY_RIGHT);
                break;
            case 6:
                menu->registerKeyPress(GEM_KEY_OK);
                break;
            case 7:
                menu->registerKeyPress(GEM_KEY_CANCEL);
                break;
            default:
                validKeyPressed = false;
                break;
        }
    }

    static uint8_t newMsgCount = 0;
    static uint8_t prevCount = 0;
    static uint8_t tickCount = 11;  // triggers update on first launch

    if (tickCount > 10) {  // Update every 10 ticks
        newMsgCount = mailbox.getNewMsgCount();
        tickCount = 1;
    } else {
        tickCount++;
    }
    
    if (newMsgCount && (validKeyPressed || (prevCount != newMsgCount))) {
        prevCount = newMsgCount;

        char countStr[3] = "??";
        if (newMsgCount < 99) {
            sprintf(countStr, "%d", newMsgCount);
        }
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.setFontMode(1);
        u8g2.setDrawColor(1);  // do not use XOR, its behavior weird in this case :) try it and you'll see why.

        // adjust right margin according to digits
        uint16_t width = u8g2.getDisplayWidth() - 11;
        if (newMsgCount > 9) {
            width = width - 3;
        }

        u8g2.drawButtonUTF8(width, u8g2.getDisplayHeight() - 13, U8G2_BTN_BW1, 0, 1, 1, countStr);  // !!
        u8g2.sendBuffer();
    }
}
