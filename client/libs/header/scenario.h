//
// Created by parallels on 1/21/17.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_SCENARIO_H
#define OS_ROBOT_PROJECT_FALL2016_SCENARIO_H

#include "std_include.h"

// init steps for all scenario and implement them
void scenario_small_stadium_beginner_init(MotorInfo* motorInfo, SensorInfo *sensorInfo);

void scenario_small_stadium_finisher_init(MotorInfo* motorInfo, SensorInfo *sensorInfo);

void scenario_large_stadium_left_beginner(MotorInfo* motorInfo, SensorInfo *sensorInfo);

void scenario_large_stadium_left_finisher(MotorInfo* motorInfo, SensorInfo *sensorInfo);

void scenario_large_stadium_right_beginner(MotorInfo* motorInfo, SensorInfo *sensorInfo);

void scenario_large_stadium_right_finisher(MotorInfo* motorInfo, SensorInfo *sensorInfo);
#endif //OS_ROBOT_PROJECT_FALL2016_SCENARIO_H
