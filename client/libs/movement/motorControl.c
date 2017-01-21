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

void turn_robot( MotorInfo motor_info,  SensorInfo sensor, double degree) {
    stopRobot(motor_info);
    int turn_speed = 120;
    int turn_time = 100;
    float diff_degree, init_degree, sign, desire_degree;    
     SensorInfo nsensor;

    update_sensor_info(&sensor);
    init_degree = sensor.currentGyro;
    desire_degree = init_degree + degree;

    while (true) {
        update_sensor_info(&nsensor);
        diff_degree = nsensor.currentGyro - desire_degree;

        if (abs(diff_degree) <= 5) {
            stopRobot(motor_info);
            break;
        } 
        if (diff_degree < 0)
        { // turn right
            set_tacho_speed_sp(motor_info.leftMotor, turn_speed);
            set_tacho_speed_sp(motor_info.rightMotor, -turn_speed);
            set_tacho_time_sp(motor_info.leftMotor, turn_time);
            set_tacho_time_sp(motor_info.rightMotor, turn_time);
            set_tacho_command_inx(motor_info.leftMotor, TACHO_RUN_TIMED);
            set_tacho_command_inx(motor_info.rightMotor, TACHO_RUN_TIMED); 
        }
        else { // turn left
            set_tacho_speed_sp(motor_info.leftMotor, -turn_speed);
            set_tacho_speed_sp(motor_info.rightMotor, turn_speed);
            set_tacho_time_sp(motor_info.leftMotor, turn_time);
            set_tacho_time_sp(motor_info.rightMotor, turn_time);
            set_tacho_command_inx(motor_info.leftMotor, TACHO_RUN_TIMED);
            set_tacho_command_inx(motor_info.rightMotor, TACHO_RUN_TIMED); 

        }

    }
}

void turnLeft( MotorInfo motorInfo, double degree) {
    stopRobot(motorInfo);
    int turnSpeed = TURN_SPEED;
    int turnTime = (int)(degree * (1300.0f / 90)); // calculate based on degree
    set_tacho_speed_sp(motorInfo.leftMotor, -turnSpeed);
    set_tacho_speed_sp(motorInfo.rightMotor, turnSpeed);
    set_tacho_time_sp(motorInfo.leftMotor, turnTime);
    set_tacho_time_sp(motorInfo.rightMotor, turnTime);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RUN_TIMED);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RUN_TIMED);
    //usleep(turnTime);
}

void turnRight( MotorInfo motorInfo, double degree) {
    stopRobot(motorInfo);
    int turnSpeed = TURN_SPEED;
    int turnTime = (int)(degree * (1300.0f / 90)); // calculate based on degree
    set_tacho_speed_sp(motorInfo.leftMotor, turnSpeed);
    set_tacho_speed_sp(motorInfo.rightMotor, -turnSpeed);
    set_tacho_time_sp(motorInfo.leftMotor, turnTime);
    set_tacho_time_sp(motorInfo.rightMotor, turnTime);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RUN_TIMED);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RUN_TIMED);
    //usleep(turnTime);
}

void runStraightLine( MotorInfo motorInfo,  SensorInfo sensorInfo) {
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

void stopRobot( MotorInfo motorInfo) {
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RESET);
}

void grabObject( MotorInfo motorInfo) {
    stopRobot(motorInfo);
    set_tacho_speed_sp(motorInfo.graberMotor, 200);
    set_tacho_time_sp(motorInfo.graberMotor, 700);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RUN_TIMED);
    usleep(2000000);
}

void releaseObject( MotorInfo motorInfo) {
    stopRobot(motorInfo);
    set_tacho_speed_sp(motorInfo.graberMotor, -200);
    set_tacho_time_sp(motorInfo.graberMotor, 700);
    set_tacho_command_inx(motorInfo.graberMotor, TACHO_RUN_TIMED);
    usleep(2000000);
}

/////////////////////////////////sensor code/////////////////////////////////////////
int getColorSensorValue(SensorInfo* sensorInfo) {
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

float get_us_sensor_value() {
    uint8_t sn_us;
    bool stop = false;
    float value = 0;
    while (!stop) {
        if (ev3_search_sensor(LEGO_EV3_US, &sn_us, 0)) {
            stop = true;
            if (!get_sensor_value0(sn_us, &value)) {
                value = 0;
            }
            //printf("%f\n", value);
            return value / 10.0;
        }
        else {
            printf("Can not find US sensor\n");
        }
    }
}
int get_gyro_sensor_value() {
    uint8_t sn_us;
    bool stop = false;
    float value = 0;
    while (!stop) {
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_us, 0)) {
            stop = true;
            if (!get_sensor_value0(sn_us, &value)) {
                value = 0;
            }
            return value;
        } else {
            printf("Can not find GYRO sensor\n");
        }
    }
}

void update_sensor_info(SensorInfo* info) { // update
    //printf("Update\n");
    uint8_t sn_gyro;
    bool stop = false;
    float value = 0;
    while (!stop) {
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0)) {
            stop = true;
            if (!get_sensor_value0(sn_gyro, &value)) {
                value = 0;
            }
            info->currentGyro = value;
            info->diffGyro = info->initialGyro - info->currentGyro;
        }
    }
    info->currentColor = getColorSensorValue(info);
    //printf("color: %d", info->currentColor);
    return;
}

void update_sensor_value(SensorInfo* sensorInfo) {
    sensorInfo->currentColor = getColorSensorValue(sensorInfo);
    sensorInfo->diffColor = sensorInfo->initialColor - sensorInfo->currentColor;

    sensorInfo->currentGyro = get_gyro_sensor_value();
    sensorInfo->diffGyro = sensorInfo->initialGyro - sensorInfo->currentGyro;

    sensorInfo->currentDistance = get_us_sensor_value();
    sensorInfo->diffDistance = sensorInfo->initialDistance - sensorInfo->currentDistance;

}

void set_sensor_initial_values(SensorInfo* sensorInfo) {
    sensorInfo->initialColor = getColorSensorValue(sensorInfo);
    sensorInfo->initialGyro = get_gyro_sensor_value();
    sensorInfo->initialDistance = get_us_sensor_value();
    sensorInfo->diffGyro = 0;
    sensorInfo->diffColor = 0;
    sensorInfo->diffDistance = 0;
}


uint8_t getTacho(int portNum) {
    uint8_t tachoSeqNum;
    bool stop = false;
    while (!stop) {
        if (ev3_search_tacho_plugged_in(portNum, 0, &tachoSeqNum, 0)) {
            return tachoSeqNum;
        }
    }
}


void init_motor_info(MotorInfo* motorInfo) {
    uint8_t tacho_left_motor = getTacho(PORT_B);    // left  wheel id
    uint8_t tacho_right_motor = getTacho(PORT_C);   // right wheel id
    uint8_t tacho_graber_motor = getTacho(PORT_D);  // graber id
    motorInfo->leftMotor = tacho_left_motor;        // motor port position constant should not be changed
    motorInfo->rightMotor = tacho_right_motor;
    motorInfo->graberMotor = tacho_graber_motor;


    motorInfo->speed = 250;                         // dummy number may be change later
    motorInfo->time = 3000;
    motorInfo->command = TACHO_RUN_TIMED;
    motorInfo->turnDegree = 0;
}
