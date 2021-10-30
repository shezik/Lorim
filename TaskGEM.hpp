#ifndef __LORIM_TASKGEM__
#define __LORIM_TASKGEM__

#include "Lorim_definitions.hpp"
#include <GEM_u8g2.h>
#include "TaskDispatcher.hpp"

class TaskGEM {
    private:
        uint8_t displayContrast = 127;  // Fallback value

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
        void setContrastCallback();
        
    public:
        TaskGEM(TaskDispatcher &_parentDispatcher, U8G2_DISPLAY_TYPE &_u8g2);
        ~TaskGEM();
        void init();
        void tick(short keycode);
};

#endif