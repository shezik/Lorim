TaskA (inherited from Task)
- tick()
- init()
- A()

TaskB (inherited from Task)
- tick()
- init()
- B()

Task (base class)
- tick()
- init()


TaskA taskA();
TaskB taskB();
void *currentTask;

currentTask = &taskA;

(Task*)currentTask->init();
(Task*)currentTask->tick();

IT DOES NOT CALL TICK IN TASKA BUT INSTEAD CALLS TICK IN TASK LIKE WTF??


temporary tasks (menu, chatbox, ime, etc.):
only foreground task is ticked, others are deleted
if no foreground task is present, launch menu
foreground task must yield before it could be deleted and new task created

persistent tasks (notification center, LoRa daemon, hotkey listener, battery monitor, etc.):
always ticked

do not block (go into one's own loop). use status.

**DO NOT DO EXCESSIVE CHECKS.**

# Components and their jobs

## TaskGEM

Should be named "Task Launcher and Settings Manager".  
Compare its function to that of the Settings app on your phone. It manages all settings for system apps.  
And it also works as a springboard to launch apps.

Read and apply settings every time the user goes back to main menu.  
Thus unsaved but already applied settings will be rolled back.

## TaskManager

Manages memory-intensive user interface related stuff. Does not interfere with background tasks e.g. message retrieving and notification popping.
