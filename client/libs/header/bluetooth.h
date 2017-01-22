//
// Created by DuongLam on 1/22/2017.
//

#ifndef ROBOT_BUNBO_BLUETOOTH_H
#define ROBOT_BUNBO_BLUETOOTH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

// Bluetooth object to communicate with server
typedef struct bluetooth_object{
    uint16_t msgId;
    position pos;
    int socket;
} bluetooth_object;

// Read the server's returning message
int readfromserver(bluetooth_object bt_obj, char *buffer, size_t maxSize);

#endif //ROBOT_BUNBO_BLUETOOTH_H
