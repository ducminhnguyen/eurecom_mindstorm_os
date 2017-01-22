//
// Created by parallels on 12/4/16.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
#define OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H

#include <stdint.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "config.h"

typedef struct SensorInfo {
    float currentGyro, initialGyro, diffGyro;
    int currentColor, initialColor, diffColor;
    float currentDistance, initialDistance, diffDistance;
} SensorInfo;

typedef struct MotorInfo {
    uint8_t leftMotor, rightMotor, graberMotor;
    int speed;
    int command;
    int time;
    double turnDegree;
    int graberState;
} MotorInfo;

//typedef struct CommandInfo {
//    Robot_State cmdState;
//    struct SensorInfo sensorInfo;
//    struct MotorInfo motorInfo;
//} CommandInfo;

//////////////////////////// motor function declaration /////////////////////
void runStraight();
void turn_robot(MotorInfo motor_info, SensorInfo sensor, double degree);
void turnLeft(MotorInfo motorInfo, double degree);
void turnRight(MotorInfo motorInfo, double degree);
void runStraightLine(MotorInfo motorInfo, SensorInfo sensorInfo);
void stopRobot(MotorInfo motorInfo);
void grabObject(MotorInfo motorInfo);
void releaseObject(MotorInfo motorInfo);



/////////////////////////// sensor function declaration ///////////////////
int getColorSensorValue(SensorInfo* sensorInfo);
float get_us_sensor_value();
float get_gyro_sensor_value();
void update_sensor_info(SensorInfo* info);
void update_sensor_value(SensorInfo* sensorInfo);



/////////////////////////// initialize variables ////////////////////////
uint8_t getTacho(int portNum);
void init_motor_info(MotorInfo* motorInfo);
void set_sensor_initial_values(SensorInfo* sensorInfo);


/////// helper functions

#endif //OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
