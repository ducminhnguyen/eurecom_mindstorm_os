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

struct SensorInfo {
    float currentGyro, initialGyro, diffGyro;
    int currentColor, initialColor, diffColor;
};

struct MotorInfo {
    uint8_t leftMotor, rightMotor, graberMotor;
    int speed;
    int command;
    int time;
    double turnDegree;
    int graberState;
};

struct CommandInfo {
    Robot_State cmdState;
    struct SensorInfo sensorInfo;
    struct MotorInfo motorInfo;
};

//////////////////////////// motor function declaration /////////////////////
void runStraight();
void turn_robot(struct MotorInfo motor_info, struct SensorInfo sensor, double degree);
void turnLeft(struct MotorInfo motorInfo, double degree);
void turnRight(struct MotorInfo motorInfo, double degree);
void runStraightLine(struct MotorInfo motorInfo, struct SensorInfo sensorInfo);
void stopRobot(struct MotorInfo motorInfo);
void grabObject(struct MotorInfo motorInfo);
void releaseObject(struct MotorInfo motorInfo);



/////////////////////////// sensor function declaration ///////////////////
int getColorSensorValue(struct SensorInfo sensorInfo);
void update_sensor_info(struct SensorInfo* info);


#endif //OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
