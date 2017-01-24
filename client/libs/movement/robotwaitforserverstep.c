//
// Created by DuongLam on 1/23/2017.
//
#include "../header/motorControl.h"
#include "../header/global.h"
#include "../header/std_include.h"
#include "../header/robotwaitforserverstep.h"
#include "../header/bluetooth.h"

void robotwaitforserver_update(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    if(global_params.robot_state == ROBOT_WAIT_FOR_SERVER){
        ReadServerMsg(&global_params, 58);
        unsigned char msg_type = GetMessageType(&global_params);
        switch (msg_type){
            case MSG_START:
                printf("Receive START message! Initializing robot...\n");
                InitGameInfo(&global_params);
                global_params.robot_state = ROBOT_COMPLETE_STEP;
            case MSG_KICK:
            case MSG_STOP:
                printf("Received stop/kick message! Stopping the robot...\n");
                close(global_params.btObj.socket);
                global_params.robot_state = ROBOT_FINISH_PROGRAM;
            case MSG_NEXT:
                printf("Received Next message.\n");
                global_params.robot_state = ROBOT_COMPLETE_STEP;
            case MSG_BALL:
                printf("Receive message ball position at %02X%02X,%02X%02X \n",
                       global_params.btObj.msg[7], global_params.btObj.msg[6], global_params.btObj.msg[9], global_params.btObj.msg[8]);
            default:
                printf("Ignore message %d\b\n", msg_type);
        }
    }
}

void robotwaitforsever_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo){
}

void robotwaitforserver_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    global_params.robot_state = ROBOT_WAIT_FOR_SERVER;
}
