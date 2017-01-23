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

void InitBtObject(global_parameters *glo_param, char *serverAddr);
void InitGameInfo(global_parameters *glo_param, unsigned char stadium);
int ConnectBtServer(global_parameters *glo_param);
int ReadServerMsg(global_parameters *glo_param, size_t maxSize);
void ToDestination(global_parameters *glo_param);
void SendRobotPosition(global_parameters *glo_param);
void SendBallMessage(global_parameters *glo_param, int act);
unsigned char GetMessageType(global_parameters *glo_param);

#endif //ROBOT_BUNBO_BLUETOOTH_H
