#ifndef __LORIM_GEM_PAGES__
#define __LORIM_GEM_PAGES__

#include "Lorim_header.hpp"

// Main page and its items
GEMPage pageMain("Lorim v0.1");
GEMItem pageItemMainSettings("Settings");

// Settings page and its items
GEMPage pageSettings("Settings");
GEMItem pageItemSettingsContrast("Contrast:", displayContrast, setContrastCallback);

void setupMenu() {
    
    // setup main page
    pageMain.addMenuItem(pageItemMainSettings);
    menu.setMenuPageCurrent(pageMain);

    // setup settings page
    pageSettings.addMenuItem(pageItemSettingsContrast);
    pageSettings.setParentMenuPage(pageMain);

}

#endif
