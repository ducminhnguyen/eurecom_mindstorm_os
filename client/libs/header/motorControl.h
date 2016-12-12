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
    float currentColor, initialColor, diffColor;
};

struct MotorInfo {
    uint8_t leftMotor, rightMotor, graberMotor;
    int speed;
    int command;
    int time;
    double turnDegree = 90;
};

//////////////////////////// motor function declaration /////////////////////
void runStraight();
void turnLeft(struct MotorInfo motorInfo, double degree);
void turnRight(struct MotorInfo motorInfo, double degree);
void runStraightLine(struct MotorInfo motorInfo, struct SensorInfo sensorInfo);
void stopRobot(struct MotorInfo motorInfo);
void grabObject(struct MotorInfo motorInfo);
void releaseObject(struct MotorInfo motorInfo);

/////////////////////////// sensor function declaration ///////////////////
int getColorSensorValue(struct SensorInfo sensorInfo);


#endif //OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
