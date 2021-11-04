#pragma once

#include "Lorim_definitions.hpp"
#include <GEM_u8g2.h>

class TaskDispatcher;

class TaskGEM {
    private:
        uint8_t displayContrast = 127;  // Fallback value
        static TaskGEM *callbackContext;  // Save the memory address of (the only, if not less) instance
                                         // so that static callback methods could locate it.

        TaskDispatcher &parentDispatcher;
        U8G2_DISPLAY_TYPE &u8g2;

        GEM_u8g2 *menu;
        // Pages
        GEMPage *pageMain;
        GEMPage *pageSettings;
        // Items for pageMain
        GEMItem *pageItemMainSettings;
        // Items for pageSettings
        GEMItem *pageItemSettingsContrast;

        void allocateMem();
        void freeMem();
        
        void setContrast();
        
    public:
        TaskGEM(TaskDispatcher &_parentDispatcher, U8G2_DISPLAY_TYPE &_u8g2);
        ~TaskGEM();
        void init();
        void tick(short keycode);

        static void setContrast_Callback();  // static foobar_Callback() and foobar() come in pairs.
        
};
