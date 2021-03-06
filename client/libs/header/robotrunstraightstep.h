//
// Created by parallels on 1/17/17.
//

// this file is implementation to run straight until you meet a black line
#ifndef OS_ROBOT_PROJECT_FALL2016_ROBOTRUNSTRAIGHT_H
#define OS_ROBOT_PROJECT_FALL2016_ROBOTRUNSTRAIGHT_H

#include "motorControl.h"
#include "config.h"

// call this function in the update all function in the loop
void robotrunstraight_update(MotorInfo *motorInfo, SensorInfo *sensorInfo);


// call this function in the run motor function of the loop
void robotrunstraight_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo);

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotrunstraight_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo);

#endif //OS_ROBOT_PROJECT_FALL2016_ROBOTRUNSTRAIGHT_H
