//
// Created by DuongLam on 1/23/2017.
//
#include "../header/motorControl.h"
#include "../header/global.h"
#include "../header/std_include.h"
#include "../header/robotsendnextmsgstep.h"
#include "../header/bluetooth.h"

void robotsendnextmsg_update(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    if(global_params.robot_state == ROBOT_TO_DESTINATION) {
        ToDestination(&global_params);
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}

void robotsendnextmsg_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo){
}

void robotsendnextmsg_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    global_params.robot_state = ROBOT_TO_DESTINATION;
}