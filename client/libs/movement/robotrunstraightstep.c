//
// Created by parallels on 1/17/17.
//
// header include
#include "../header/motorControl.h"
#include "../header/robotrunstraightstep.h"
#include "../header/global.h"
// libs include
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
// Update the sensor information
// Calculate condition to change ROBOT_STATE and ROBOT step
// Update the ROBOT_STATE and the step
void robotrunstraight_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
}

void robotrunstraight_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {

    update_sensor_value(sensorInfo);
    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if (sensorInfo->currentColor < 10) {
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // move to next step();
        // movetonextstep(&global_params, motorInfo, sensorInfo);
        // for individual step testing comment this
    }
}


void robotrunstraight_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    int left_motor_speed, right_motor_speed;
    get_tacho_speed(motorInfo->leftMotor, &left_motor_speed);
    get_tacho_speed(motorInfo->rightMotor, &right_motor_speed);

    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if (sensorInfo->diffGyro > 0) {// && left_motor_speed < right_motor_speed + 30) { // left tilt
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            printf("Left tilt: %f \n", sensorInfo->diffGyro);
        } else if (sensorInfo->diffGyro < 0) {// && left_motor_speed + 30 > right_motor_speed) { // right tilt
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            printf("Right tilt: %f \n", sensorInfo->diffGyro);
        } else {
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
        }
        set_tacho_time_sp(motorInfo->leftMotor, motorInfo->time);
        set_tacho_time_sp(motorInfo->rightMotor, motorInfo->time);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    }
}