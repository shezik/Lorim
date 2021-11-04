#include "TaskGEM.hpp"

TaskGEM *TaskGEM::callbackContext = NULL;  // init static variable and allocate memory otherwise you get linker errors

TaskGEM::TaskGEM(TaskDispatcher &_parentDispatcher, U8G2_DISPLAY_TYPE &_u8g2)
    : parentDispatcher(_parentDispatcher)
    , u8g2(_u8g2)
{
    callbackContext = this;
    allocateMem();
}

TaskGEM::~TaskGEM() {
    callbackContext = NULL;  // set it to null since callbackContext is a static variable.
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
    delete menu;
    delete pageMain;
    delete pageSettings;
    delete pageItemMainSettings;
    delete pageItemSettingsContrast;
    menu = NULL;
    pageMain = NULL;
    pageSettings = NULL;
    pageItemMainSettings = NULL;
    pageItemSettingsContrast = NULL;
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
    if (callbackContext != NULL) {  // this is static and could be called at any given time
        callbackContext->setContrast();
    }
}

void TaskGEM::setContrast() {
    if (displayContrast > 255) {displayContrast = 255;}
    else if (displayContrast < 0) {displayContrast = 0;}
    u8g2.setContrast(displayContrast);
}

void TaskGEM::tick(short keycode) {
    if (menu->readyForKey()) {
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
            case 4:
                menu->registerKeyPress(GEM_KEY_OK);
                break;
            case 5:
                menu->registerKeyPress(GEM_KEY_CANCEL);
                break;
        }
    }
}
