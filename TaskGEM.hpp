#pragma once

#include "Lorim_definitions.hpp"
#include <GEM_u8g2.h>
#include "TaskBase.hpp"
#include "Mailbox.hpp"
#include "TaskManager.hpp"

class TaskGEM : public TaskBase {
    private:
        int displayContrast = 191;  // Fallback value
        static TaskGEM *callbackContext;  // Save the memory address of (the only, if not less) instance
                                          // so that static callback methods could locate it.

        TaskManager &parentManager;
        U8G2_DISPLAY_TYPE &u8g2;
        Mailbox &mailbox;

        GEM_u8g2 *menu;
        // Pages
        GEMPage *pageMain;
        GEMPage *pageSettings;
        // Items for pageMain
        GEMItem *pageItemMainChatbox;
        GEMItem *pageItemMainSettings;
        // Items for pageSettings
        GEMItem *pageItemSettingsContrast;

        void allocateMem();
        void freeMem();
        
        void setContrast();

    public:
        TaskGEM(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox);
        ~TaskGEM() override;
        void init() override;
        void tick(int16_t keycode) override;
        uint8_t getTaskID() override {return ID_TASKGEM;}

        static void setContrast_Callback();  // static foobar_Callback() and foobar() come in pairs.
        static void switchToChatbox_Callback();

};
