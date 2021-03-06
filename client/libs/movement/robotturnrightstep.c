//
// Created by parallels on 1/20/17.
//

#include "../header/robotturnrightstep.h"
#include "../header/std_include.h"

#ifndef GYRO_ERROR
#define GYRO_ERROR 3.0f
#endif

static float turn_angle;
static bool robotturnright_running = FALSE;

// call this function in the update all function in the loop
void robotturnright_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    if (global_params.robot_state == ROBOT_TURN_RIGHT) {
        //if((float)fabsf(sensorInfo->diffGyro) >= (global_params.robot_steps[global_params.current_step].robot_turn_left_degree) - 5.0f){
        if((float)fabsf(sensorInfo->diffGyro) >= (turn_angle - GYRO_ERROR) ){
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // move to next step();
        // movetonextstep(&global_params, motorInfo, sensorInfo);
        // for individual step testing comment this
        printf("Complete turn right\n");
        robotturnright_running = FALSE;
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}


// call this function in the run motor function of the loop
void robotturnright_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if(global_params.robot_state == ROBOT_TURN_RIGHT){
        set_tacho_speed_sp(motorInfo->leftMotor, 40);
        set_tacho_speed_sp(motorInfo->rightMotor, -40);
        set_tacho_time_sp(motorInfo->leftMotor, TURN_TIME);
        set_tacho_time_sp(motorInfo->rightMotor, TURN_TIME);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_RUN_TIMED);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_RUN_TIMED);
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING || global_params.robot_state == ROBOT_COMPLETE_STEP) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotturnright_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    if (robotturnright_running == FALSE)
        robotturnright_running = TRUE;
    else
        return;
    global_params.ideal_target_angle += global_params.robot_steps[global_params.current_step].robot_turn_right_degree;
    printf("Turning right: turn angle: %f, ideal angle, current angle: %f, %f\n",
        global_params.robot_steps[global_params.current_step].robot_turn_right_degree, global_params.ideal_target_angle, sensorInfo->initialGyro);
    turn_angle = fabsf(global_params.ideal_target_angle - sensorInfo->initialGyro);
    global_params.robot_state = ROBOT_TURN_RIGHT;
}
