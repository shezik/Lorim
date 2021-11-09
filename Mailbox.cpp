#include "Mailbox.hpp"

Mailbox::Mailbox(FS &_littleFS)
    : lilFS(_littleFS)
{
    // do nothing
}

Mailbox::~Mailbox() {
    // do nothing
}

void Mailbox::init(Layer1Class *_Layer1, LL2Class *_LL2) {
    Layer1 = _Layer1; LL2 = _LL2;
    (lilFS.open("/History.txt", "w+")).close();
}

void Mailbox::tick() {
    LL2->daemon();
    struct Packet packet = LL2->readData();
    if (packet.totalLength > HEADER_LENGTH) {
        newMsgCount++;
        char username[MAX_USERNAME_LENGTH + 1];  // save one for terminator
        char message[MESSAGE_LENGTH];
        uint8_t usernameLen;
        for (usernameLen = 0; usernameLen < MAX_USERNAME_LENGTH; usernameLen++) {
            if (packet.datagram.message[usernameLen] == '\0') break;
        }
        int16_t realMsgLen = packet.totalLength - HEADER_LENGTH - DATAGRAM_HEADER - (usernameLen + 1);
        if (realMsgLen > 0) {
            memcpy(username, packet.datagram.message, usernameLen + 1);  // +1 to preserve terminator
            memcpy(message, packet.datagram.message + usernameLen + 1, realMsgLen);
            message[realMsgLen] = '\0';
            appendToHistory(message, packet.source, username);
        }
    }
}

void Mailbox::appendToHistory(char *message, uint8_t *address, char *username) {
    File historyFile = lilFS.open("/History.txt", "a");

    historyFile.write('\n');

    for (uint8_t i = 0; i < MAX_USERNAME_LENGTH + 1; i++) {
        if (username[i] == '\0') break;
        historyFile.write(username[i]);
    }

    historyFile.write('\t');

    char addrString[ADDR_LENGTH*2 + 1];
    sprintf(addrString, "%02x%02x%02x%02x", address[2], address[3], address[4], address[5]);
    for (uint8_t i = 0; i < ADDR_LENGTH*2 + 1; i++) {
        if (addrString[i] == '\0') break;  // !! useless?
        historyFile.write(addrString[i]);
    }

    historyFile.write('\t');

    for (uint8_t i = 0; i < MESSAGE_LENGTH; i++) {
        if (message[i] == '\0') break;
        historyFile.write(message[i]);
    }

    historyFile.flush();
    historyFile.close();
}

void Mailbox::sendMessage(char* message, uint8_t *myAddress, char *myUsername, uint8_t *destination) {
    appendToHistory(message, myAddress, myUsername);
    
}

uint16_t Mailbox::getNewMsgCount() {
    return newMsgCount;
}

void Mailbox::clearNewMsgCount() {
    newMsgCount = 0;
}
