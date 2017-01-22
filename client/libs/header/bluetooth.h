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

void InitBtObject(char *serverAddr);
int ConnectBtServer();
int ReadServerMsg(char *buffer, size_t maxSize);
void ToDestination();
void SendRobotPosition();
void SendBallMessage(int act);

#endif //ROBOT_BUNBO_BLUETOOTH_H
