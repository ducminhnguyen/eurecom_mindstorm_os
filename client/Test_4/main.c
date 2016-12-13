//
// Created by parallels on 12/6/16.
//


#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "../libs/header/config.h"
#include "../libs/header/motorControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

Robot_State robotState = ROBOT_STOP;

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

void initRobot() {
    while (ev3_tacho_init() < 1 ) Sleep( 1000 );
    ev3_sensor_init();
}

static bool _check_pressed( uint8_t sn )
{
    int val;

    if ( sn == SENSOR__NONE_ ) {
        return ( ev3_read_keys((uint8_t*) &val ) && ( val & EV3_KEY_UP ));
    }
    return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

float getInitialGyroValue() {
    uint8_t sn_gyro;
    bool stop = false;
    float value = 0;
    while (!stop) {
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0)) {
            stop = true;
            if (!get_sensor_value0(sn_gyro, &value)) {
                value = 0;
            }
            return value;
        }
    }
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


void UpdateSensorInfo(struct SensorInfo* info) { // update
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
    info->currentColor = getColorSensorValue(*info);
    if (info->currentColor < 15 && info->currentColor > 0) {
        sleep(1);
        robotState = ROBOT_STOP;
    }
    return;
}

void SteerRobot(struct SensorInfo sensorInfo, struct MotorInfo motorInfo) { // draw
    //runStraightLine(motorInfo, sensorInfo);
    //printf("Steering\n");
    if (robotState == ROBOT_TURN_LEFT) {
        turnLeft(motorInfo, motorInfo.turnDegree);
        robotState = ROBOT_IDLE;
    } else if (robotState == ROBOT_STOP) {
        stopRobot(motorInfo);
    } else if (robotState == ROBOT_TURN_RIGHT) {
        turnRight(motorInfo, motorInfo.turnDegree);
        robotState = ROBOT_IDLE;
    } else if (robotState == ROBOT_GO_STRAIGHT) {
        runStraightLine(motorInfo, sensorInfo);
    } else if (robotState == ROBOT_GO_TIMED) {
        // To be implement
    } else if (robotState == ROBOT_GRAB) {

    } else if (robotState == ROBOT_SCAN) {

    }
    return;
}

void ResetSensors(struct SensorInfo sensorInfo) {

}

void StartRunning(struct MotorInfo motorInfo) {
    int speed = motorInfo.speed;
    set_tacho_command_inx(motorInfo.leftMotor, TACHO_RESET);
    set_tacho_command_inx(motorInfo.rightMotor, TACHO_RESET);
    set_tacho_speed_sp(motorInfo.leftMotor, speed);
    set_tacho_speed_sp(motorInfo.rightMotor, speed);
    set_tacho_time_sp(motorInfo.leftMotor, motorInfo.time);
    set_tacho_time_sp(motorInfo.rightMotor, motorInfo.time);
    set_tacho_command_inx(motorInfo.leftMotor, motorInfo.command);
    set_tacho_command_inx(motorInfo.rightMotor, motorInfo.command);

    return;
}

int main( void ) {

#ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "192.168.0.204";

#endif
    if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
    printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
    printf( "Waiting tacho is plugged...\n" );

#endif
    printf("Start program\n");
    initRobot();
    int i;
    uint8_t sn;
    FLAGS_T state;
    uint8_t sn_touch;
    uint8_t sn_color;
    uint8_t sn_compass;
    uint8_t sn_sonar;
    uint8_t sn_mag;
    uint8_t sn_gyro;
    char s[ 256 ];
    int val, color_val;
    float value;
    uint32_t n, ii;

    uint8_t tacho_left_motor = getTacho(PORT_B);    // left  wheel id
    uint8_t tacho_right_motor = getTacho(PORT_C);   // right wheel id
    uint8_t tacho_graber_motor = getTacho(PORT_D);
    struct MotorInfo motorInfo;
    motorInfo.leftMotor = tacho_left_motor;
    motorInfo.rightMotor = tacho_right_motor;
    motorInfo.graberMotor = tacho_graber_motor;
    motorInfo.speed = 500;
    motorInfo.time = 5000;
    motorInfo.command = TACHO_RUN_TIMED;
    motorInfo.turnDegree = 90;

    float initialGyro = getInitialGyroValue();      // initial value of gyro
    struct SensorInfo info;
    info.initialGyro = initialGyro;
    info.diffGyro = initialGyro;
    StartRunning(motorInfo);

    // robotState = ROBOT_GO_TIMED;

    // run straight for a second, to running out of starting position
    printf("start running\n");
    motorInfo.time = 4400;
    StartRunning(motorInfo);
    runStraightLine(motorInfo, info);
    Sleep(4600);
    stopRobot(motorInfo);
    sleep(2);

    //turn right
    printf("turn right\n");
    motorInfo.turnDegree = 100;
    StartRunning(motorInfo);
    //turnRight(motorInfo, motorInfo.turnDegree);
    turn_robot(motorInfo, info, -90);
    Sleep(500);
    stopRobot(motorInfo);
    sleep(2);

    // run straight
    printf("run straight\n");
    motorInfo.time = 3200;
    StartRunning(motorInfo);
    runStraightLine(motorInfo, info);
    Sleep(2400);
    stopRobot(motorInfo);
    sleep(2);

    // turn left
    printf("turn left\n");
    motorInfo.turnDegree = 90;
    StartRunning(motorInfo);
    //turnLeft(motorInfo, motorInfo.turnDegree);
    turn_robot(motorInfo, -90);
    Sleep(500);
    stopRobot(motorInfo);
    sleep(2);

    //run straight
    printf("run straight\n");
    motorInfo.time = 3400;
    StartRunning(motorInfo);
    runStraightLine(motorInfo, info);
    Sleep(2600);
    stopRobot(motorInfo);
    sleep(2);

    // turn left
    printf("turn left\n");
    StartRunning(motorInfo);
    //turnLeft(motorInfo, motorInfo.turnDegree);
    //turn_robot(motorInfo, info, motorInfo.turnDegree);

    turn_robot(motorInfo, sensor, -90);
    Sleep(500);
    stopRobot(motorInfo);
    sleep(2);


    color_val = getColorSensorValue(info);
    while (true) { // run until see black
        printf("sensor color %d\n", color_val); 
        if ( (color_val = getColorSensorValue(info)) < 3) {
            printf("Get into black area\n");
            break; 
        } 
        motorInfo.command = TACHO_RUN_TIMED;
        StartRunning(motorInfo);
        runStraightLine(motorInfo, info);
        //Sleep(30);
    }
    // release object
    motorInfo.time = 200;
    motorInfo.command = TACHO_RUN_TIMED;
    StartRunning(motorInfo);
    runStraightLine(motorInfo, info);
    Sleep(200);
    stopRobot(motorInfo);
    Sleep(1500);
    StartRunning(motorInfo);
    releaseObject(motorInfo);
    Sleep(1000);
    printf("Here 231");
    // run backward
    struct MotorInfo b_motor_info = motorInfo;
    b_motor_info.speed = - b_motor_info.speed;
    b_motor_info.speed = -200;
    b_motor_info.time = 1500;
    b_motor_info.command = TACHO_RUN_TIMED;
    stopRobot(motorInfo);
    StartRunning(b_motor_info);
    runStraightLine(b_motor_info, info);
    Sleep(3000);
    stopRobot(b_motor_info);

    ev3_uninit();
    printf( "*** ( EV3 ) Bye! ***\n" );
    return 0;
}

