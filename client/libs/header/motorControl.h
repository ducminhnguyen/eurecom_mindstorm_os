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


struct SensorInfo {
    float currentGyro, initialGyro, diffGyro;

};

struct MotorInfo {
    uint8_t leftMotor, rightMotor, graberMotor;
    int speed;
    int command;
    int time;
};

void runStraight();
void turnLeft(double degree);
void turnRight(double degree);
void runStraightLine(struct MotorInfo motorInfo, struct SensorInfo sensorInfo);
void stopRobot(struct MotorInfo motorInfo);
#endif //OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
