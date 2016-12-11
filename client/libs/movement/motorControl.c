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

void turnLeft(double degree) {

}

void turnRight(double degree) {

}

void runStraightLine(struct MotorInfo motorInfo, struct SensorInfo sensorInfo) {
    int left_motor_speed, right_motor_speed;
    get_tacho_speed(motorInfo.leftMotor, &left_motor_speed);
    get_tacho_speed(motorInfo.rightMotor, &right_motor_speed);
    if (sensorInfo.diffGyro > 4) { // left tilt
        set_tacho_speed_sp(motorInfo.rightMotor, right_motor_speed - abs(sensorInfo.diffGyro));
    } else if (sensorInfo.diffGyro < -4) { // right tilt
        set_tacho_speed_sp(motorInfo.leftMotor, left_motor_speed - abs(sensorInfo.diffGyro));
    } else {
        set_tacho_speed_sp(motorInfo.leftMotor, motorInfo.speed);
        set_tacho_speed_sp(motorInfo.rightMotor, motorInfo.speed);
    }
    set_tacho_command_inx(motorInfo.leftMotor, motorInfo.command);
    set_tacho_command_inx(motorInfo.leftMotor, motorInfo.command);
}

void stopRobot(struct MotorInfo motorInfo) {
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RESET);
}