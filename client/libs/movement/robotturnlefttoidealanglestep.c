//
// Created by parallels on 1/20/17.
//

#include "../header/robotturnlefttoidealanglestep.h"
#include "../header/std_include.h"

// call this function in the update all function in the loop
void robotturnlefttoidealangle_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    float adjust = (global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle > 0)? -3.0f:3.0f;
    if (global_params.robot_state == ROBOT_TURN_LEFT) {
        //printf("update: %f\n", (float)fabsf(fabsf(sensorInfo->diffGyro) - global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle));
        if (sensorInfo->currentGyro < global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle + adjust){
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // move to next step();
        // movetonextstep(&global_params, motorInfo, sensorInfo);
        // for individual step testing comment this
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}


// call this function in the run motor function of the loop
void robotturnlefttoidealangle_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if(global_params.robot_state == ROBOT_TURN_LEFT) {
        //printf("running: %d\n", TURN_TIME);
        set_tacho_speed_sp(motorInfo->leftMotor, -60);
        set_tacho_speed_sp(motorInfo->rightMotor, 60);
        set_tacho_time_sp(motorInfo->leftMotor, TURN_TIME);
        set_tacho_time_sp(motorInfo->rightMotor, TURN_TIME);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_RUN_TIMED);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_RUN_TIMED);
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING || global_params.robot_state == ROBOT_COMPLETE_STEP) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_STOP);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_STOP);
    }
}



// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotturnlefttoidealangle_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    update_sensor_value(sensorInfo);
    // calculate ideal angle and fix
//    printf("ideal straight: %f\n", global_params.ideal_straight_angle);
//    printf("current gyro: %f\n", sensorInfo->currentGyro);
//    printf("init angle to turn: %f\n", global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle);
//    printf("ideal gyro angle: %f\n", get_ideal_angle(sensorInfo->currentGyro - global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle));
    global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle = get_ideal_angle(sensorInfo->currentGyro - global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle);
    while (global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle > sensorInfo->currentGyro) {
        global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle -= 360;
    }

    global_params.ideal_target_angle = global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle;

    printf("Turning to ideal\n");

    global_params.robot_state = ROBOT_TURN_LEFT;
}



