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
    // params for different steps

    // robotrunstraightuntilwall: distance until hit wall threshold to stop
    float robot_run_straight_until_wall_distance_to_stop;

    // robotrunstraightstep: color read threshold
    int robot_run_straight_color_threshold; // color threshold to stop

    // robotturnleftstep:
    float robot_turn_left_degree;

    // robotturnrightstep:
    float robot_turn_right_degree;

    // robotturnaroundstep:

    // robotruntimed:
    float robot_run_timed_time_to_run; // time to run in second
    int robot_run_timed_speed;

    // robotturnlefttoidealangle
    float robot_turn_left_to_ideal_angle;

    // robotturnrighttoidealangle
    float robot_turn_right_to_ideal_angle;


} step;


#endif //OS_ROBOT_PROJECT_FALL2016_STEP_H
