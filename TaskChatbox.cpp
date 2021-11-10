#include "TaskChatbox.hpp"

TaskChatbox::TaskChatbox(TaskManager &_parentManager, U8G2_DISPLAY_TYPE &_u8g2, Mailbox &_mailbox)
    : parentManager(_parentManager)
    , u8g2(_u8g2)
    , mailbox(_mailbox)
{
    allocateMem();
}

TaskChatbox::~TaskChatbox() {
    freeMem();
}

void TaskChatbox::allocateMem() {

}

void TaskChatbox::freeMem() {

}

void TaskChatbox::init() {

}

void TaskChatbox::tick(int16_t keycode) {
    
}
