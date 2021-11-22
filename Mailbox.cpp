#include "Mailbox.hpp"

Mailbox::Mailbox(FS &_littleFS)
    : lilFS(_littleFS)
{
    // do nothing
}

Mailbox::~Mailbox() {
    // do nothing
}

void Mailbox::init(Layer1Class *_Layer1, LL2Class *_LL2, uint8_t *_nodeShortMac) {
    Layer1 = _Layer1; LL2 = _LL2; nodeShortMac = _nodeShortMac;
    (lilFS.open(HISTORY_PATH, "a")).close();  // create file if not exist
}

void Mailbox::tick() {
    LL2->daemon();
    struct Packet packet = LL2->readData();
    if (packet.totalLength > HEADER_LENGTH) {
        char username[MAX_USERNAME_LENGTH + 1];  // save one for terminator
        char message[MESSAGE_LENGTH];
        uint8_t usernameLen;
        for (usernameLen = 0; usernameLen < MAX_USERNAME_LENGTH; usernameLen++) {
            if (packet.datagram.message[usernameLen] == '\t') break;
        }
        int16_t realMsgLen = packet.totalLength - HEADER_LENGTH - DATAGRAM_HEADER - (usernameLen + 1);
        
        // debug
        //Serial.printf("\ntotalLength: %d\nusernameLen: %d\nrealMsgLen: %d\n", packet.totalLength, usernameLen, realMsgLen);
        //char tempchar[234];
        //snprintf(tempchar, 234, "%s", packet.datagram.message);
        //Serial.printf("\npacket.datagram.message: %s\n", tempchar);
        
        if (realMsgLen > 0) {
            memcpy(username, packet.datagram.message, usernameLen);
            username[usernameLen] = '\0';
            memcpy(message, packet.datagram.message + usernameLen + 1, realMsgLen);
            message[realMsgLen] = '\0';
            appendToHistory(message, packet.source, username);
            newMsgCount++;
        }
    }
}

void Mailbox::appendToHistory(char *message, uint8_t *address, char *username) {
    File historyFile = lilFS.open(HISTORY_PATH, "a");

    historyFile.printf("\n");

    for (uint8_t i = 0; i < MAX_USERNAME_LENGTH + 1; i++) {
        if (username[i] == '\0') break;
        historyFile.write(username[i]);
    }

    historyFile.printf(" (");

    char addrString[ADDR_LENGTH*2 + 1];
    sprintf(addrString, "%02x%02x%02x%02x", address[2], address[3], address[4], address[5]);
    for (uint8_t i = 0; i < ADDR_LENGTH*2 + 1; i++) {
        if (addrString[i] == '\0') break;  // !! useless?
        historyFile.write(addrString[i]);
    }

    historyFile.printf("): \n");

    for (uint8_t i = 0; i < MESSAGE_LENGTH; i++) {
        if (message[i] == '\0') break;
        historyFile.write(message[i]);
    }

    historyFile.close();
}

void Mailbox::sendMessage(char* message, uint8_t *myAddress, char *myUsername, uint8_t *destination) {
    appendToHistory(message, myAddress, myUsername);
    struct Datagram datagram;
    uint8_t msgLen = sprintf((char*)datagram.message, "%s\t%s", myUsername, message);
    //Serial.printf("\nSent datagram.message: %s\nmsgLen: %d\n", datagram.message, msgLen);  // debug
    memcpy(datagram.destination, destination, ADDR_LENGTH);
    datagram.type = 'c';
    LL2->writeData(datagram, msgLen + DATAGRAM_HEADER);
}

uint8_t* Mailbox::getNodeShortMac() {
    return nodeShortMac;
}

uint16_t Mailbox::getNewMsgCount() {
    return newMsgCount;
}

void Mailbox::clearNewMsgCount() {
    newMsgCount = 0;
}

FS& Mailbox::getFSReference() {
    return lilFS;
}
