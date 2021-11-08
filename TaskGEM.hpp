#pragma once

#include "Lorim_definitions.hpp"
#include <GEM_u8g2.h>
#include "TaskBase.hpp"

class TaskManager;

class TaskGEM : public TaskBase {
    private:
        int displayContrast = 191;  // Fallback value
        static TaskGEM *callbackContext;  // Save the memory address of (the only, if not less) instance
                                          // so that static callback methods could locate it.

        TaskManager &parentManager;
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
        TaskGEM(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2);
        ~TaskGEM();
        void init();
        void tick(short keycode);
        uint8_t getTaskID() {return ID_TASKGEM;}

        static void setContrast_Callback();  // static foobar_Callback() and foobar() come in pairs.

};
