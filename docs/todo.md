# Sketchpad.

```
uint16_t startPos = 0;
uint16_t endPos;

void printPage() {
    endPos = startPos;
    for (uint8_t i = 0; i < LINES_PER_PAGE; i++) {
        endPos = printOneLine(Forward, endPos);
    }
}

uint16_t printOneLine(enum direction, uint16_t _startPos) {
    // some code
    return _endPos;
}
```

```
static int8_t lastKeycode;
static uint32_t lastMillis;

int8_t keycode = getKeycode();

if (keycode != -1) {
    if (lastKeycode != keycode) {
        lastKeycode = keycode;
        lastMillis = millis();
        handleKey(short, keycode);
    } else {
        if (millis() - lastMillis >= LONGPRESS_DURATION) {
            handleKey(long, keycode);
            lastMillis = millis();
        }
    }
} else {
    lastKeycode = -1;
}
```

## Depreciated

temporary tasks (menu, chatbox, ime, etc.):
only foreground task is ticked, others are deleted
if no foreground task is present, launch menu
foreground task must yield before it could be deleted and new task created

persistent tasks (notification center, LoRa daemon, hotkey listener, battery monitor, etc.):
always ticked

do not block (go into one's own loop). use status.

**DO NOT DO EXCESSIVE CHECKS.**

## README sketch underneath

# Components and their jobs

## TaskGEM

Should be named "Task Launcher and Settings Manager".  
Compare its function to that of the Settings app on your phone. It manages all settings for system apps.  
And it also works as a springboard to launch apps.

Read and apply settings every time the user goes back to main menu.  
Thus unsaved but already applied settings will be rolled back.

## TaskManager

Manages memory-intensive user interface related stuff. Does not interfere with background tasks e.g. message retrieving and notification popping.
