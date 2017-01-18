//
// Created by parallels on 1/18/17.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_STEP_H
#define OS_ROBOT_PROJECT_FALL2016_STEP_H

#include "config.h"
#include "motorControl.h"



typedef struct {
    void (*update_all) (MotorInfo*, SensorInfo*);
    void (*run_motor) (MotorInfo*, SensorInfo*);
    void (*init_step) (MotorInfo*, SensorInfo*);
} step;


#endif //OS_ROBOT_PROJECT_FALL2016_STEP_H
