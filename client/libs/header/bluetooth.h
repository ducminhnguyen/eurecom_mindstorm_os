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
#include "global.h"

// Position to send to the server
typedef struct position{
    int x;   /* Horizontal position*/
    int y;   /* Vertical position */
} position;

// Bluetooth object to communicate with server
typedef struct BT_Object{
    uint16_t msgId;
    position pos;
    uint16_t ally;
    int socket;
    char serverAddr[20];
}BT_Object;

void InitBtObject(char *serverAddr);
int ConnectBtServer();
int ReadServerMsg(char *buffer, size_t maxSize);
void ToDestination();
void SendRobotPosition();
void SendBallMessage();

#endif //ROBOT_BUNBO_BLUETOOTH_H
