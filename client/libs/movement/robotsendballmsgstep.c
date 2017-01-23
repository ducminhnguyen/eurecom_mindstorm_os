//
// Created by DuongLam on 1/23/2017.
//

#include "../header/motorControl.h"
#include "../header/global.h"
#include "../header/std_include.h"
#include "../header/robotwaitforserverstep.h"
#include "../header/bluetooth.h"

void robotsendballmsg_update(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    if(global_params.robot_state == ROBOT_OPEN_GRABBER) {
        SendBallMessage(&global_params);
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}

void robotsendballmsg_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo){
}

void robotsendballmsg_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo){
    if(global_params.btObj.havingBall){
        global_params.btObj.havingBall = false;
    }
    else{
        global_params.btObj.havingBall = true;
    }
    global_params.robot_state = ROBOT_OPEN_GRABBER;
}

