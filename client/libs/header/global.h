//
// Created by parallels on 1/18/17.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_GLOBAL_H
#define OS_ROBOT_PROJECT_FALL2016_GLOBAL_H

#include "config.h"
#include "step.h"
#include "motorControl.h"
#define MAX_STEP 30

typedef enum {
    LOOP, ONE_WAY
} RUN_STYLE;

typedef enum {
    ROBOT_RUN_STRAIGHT, ROBOT_STOP_RUNNING, ROBOT_WAIT_FINISH_COMMAND,
    ROBOT_TURN_LEFT, ROBOT_TURN_RIGHT, ROBOT_COMPLETE_STEP, ROBOT_FINISH_PROGRAM
} ROBOT_STATE; // state pool use for all steps

typedef struct global_parameters {
    step* robot_steps;
    int current_step, total_step;
    float calibrated_straight_angle;
    RUN_STYLE run_style;
    ROBOT_STATE robot_state;
} global_parameters;

extern global_parameters global_params; // to be define in main.c
// in main.c this variable must be daclare in the following fashion: global_parameters global_params;
int movetonextstep(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo* sensorInfo);


#endif //OS_ROBOT_PROJECT_FALL2016_GLOBAL_H
