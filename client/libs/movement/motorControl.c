//
// Created by parallels on 12/4/16.
//
#include "../header/motorControl.h"
#include "../header/config.h"
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


void initTachoData(uint8_t sn,int max_speed){
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    set_tacho_speed_sp( sn, max_speed * 2 / 3 );
    set_tacho_time_sp( sn, 5000 );
    set_tacho_ramp_up_sp( sn, 2000 );
    set_tacho_ramp_down_sp( sn, 2000 );
}


void runStraight(){
    uint8_t snl, snr;
    FLAGS_T state;
    if ( ev3_search_tacho_plugged_in(PORT_B, 0, &snl, 0 ) &&
            ev3_search_tacho_plugged_in(PORT_C, 0, &snr, 0 )) {
        int max_speed;

        printf( "TESTING GOING STRAIGHT , run for 5 sec...\n" );
        get_tacho_max_speed( snr, &max_speed );
        printf("  max speed = %d\n", max_speed );
        initTachoData(snr, max_speed);
        initTachoData(snl, max_speed);
        set_tacho_command_inx( snr, TACHO_RUN_TIMED );
        set_tacho_command_inx( snl, TACHO_RUN_TIMED );
        /* Wait tacho stop */
        sleep( 100 );
        do {
            get_tacho_state_flags( snr, &state );
        } while ( state );
//        printf( "run to relative position...\n" );
//        set_tacho_speed_sp( sn, max_speed / 2 );
//        set_tacho_ramp_up_sp( sn, 0 );
//        set_tacho_ramp_down_sp( sn, 0 );
//        set_tacho_position_sp( sn, 90 );
//        for ( i = 0; i < 8; i++ ) {
//            set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
//            Sleep( 500 );
//        }

    } else {
        printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
}


void turnLeft(struct MotorInfo motorInfo, double degree) {
    stopRobot(motorInfo);
    int turnSpeed = TURN_SPEED;
    int turnTime = (int)(degree * ((float)(900) / 90)); // calculate based on degree
    set_tacho_speed_sp(motorInfo.leftMotor, -turnSpeed);
    set_tacho_speed_sp(motorInfo.rightMotor, turnSpeed);
    set_tacho_time_sp(motorInfo.leftMotor, turnTime);
    set_tacho_time_sp(motorInfo.rightMotor, turnTime);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RUN_TIMED);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RUN_TIMED);
    //usleep(turnTime);
}

void turnRight(struct MotorInfo motorInfo, double degree) {
    stopRobot(motorInfo);
    int turnSpeed = TURN_SPEED;
    int turnTime = (int)(degree * ((float)(900) / 90)); // calculate based on degree
    set_tacho_speed_sp(motorInfo.leftMotor, turnSpeed);
    set_tacho_speed_sp(motorInfo.rightMotor, -turnSpeed);
    set_tacho_time_sp(motorInfo.leftMotor, turnTime);
    set_tacho_time_sp(motorInfo.rightMotor, turnTime);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RUN_TIMED);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RUN_TIMED);
    //usleep(turnTime);
}

void runStraightLine(struct MotorInfo motorInfo, struct SensorInfo sensorInfo) {
    int left_motor_speed, right_motor_speed;
    get_tacho_speed(motorInfo.leftMotor, &left_motor_speed);
    get_tacho_speed(motorInfo.rightMotor, &right_motor_speed);
    if (sensorInfo.diffGyro > 0) {// && left_motor_speed < right_motor_speed + 30) { // left tilt
        set_tacho_speed_sp(motorInfo.rightMotor, motorInfo.speed - 40*abs(sensorInfo.diffGyro));
        set_tacho_speed_sp(motorInfo.leftMotor, motorInfo.speed);
        printf("Left tilt: %f \n", sensorInfo.diffGyro);
    } else if (sensorInfo.diffGyro < 0) {// && left_motor_speed + 30 > right_motor_speed) { // right tilt
        set_tacho_speed_sp(motorInfo.rightMotor, motorInfo.speed);
        set_tacho_speed_sp(motorInfo.leftMotor, motorInfo.speed - 40*abs(sensorInfo.diffGyro));
        printf("Right tilt: %f \n", sensorInfo.diffGyro);
    } else {
        set_tacho_speed_sp(motorInfo.leftMotor, motorInfo.speed);
        set_tacho_speed_sp(motorInfo.rightMotor, motorInfo.speed);
    }
    //printf("%d %d \n", left_motor_speed, right_motor_speed);
    set_tacho_time_sp(motorInfo.leftMotor, motorInfo.time);
    set_tacho_time_sp(motorInfo.rightMotor, motorInfo.time);
    set_tacho_command_inx(motorInfo.leftMotor, motorInfo.command);
    set_tacho_command_inx(motorInfo.rightMotor, motorInfo.command);
}

void stopRobot(struct MotorInfo motorInfo) {
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RESET);
}

void grabObject(struct MotorInfo motorInfo) {
    stopRobot(motorInfo);
    set_tacho_speed_sp(motorInfo.graberMotor, -200);
    set_tacho_time_sp(motorInfo.graberMotor, 700);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RUN_TIMED);
    usleep(200000);
}

void releaseObject(struct MotorInfo motorInfo) {
    stopRobot(motorInfo);
    set_tacho_speed_sp(motorInfo.graberMotor, 200);
    set_tacho_time_sp(motorInfo.graberMotor, 700);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RUN_TIMED);
    usleep(200000);
}

/////////////////////////////////sensor code/////////////////////////////////////////
int getColorSensorValue(struct SensorInfo sensorInfo) {
    const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
    int COLOR_COUNT = ( int )( sizeof( color ) / sizeof( color[ 0 ]));
    int val = 0;
    uint8_t sn_color;
    if (ev3_search_sensor(LEGO_EV3_COLOR, &sn_color, 0 )) {
        if ( ! get_sensor_value( 0, sn_color, &val )) {
            val = 0;
        } else {
            //printf("sn_color: %d  %d \n", sn_color, val);
            return val;
        }
    }
}

