//
// Created by parallels on 12/4/16.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
#define OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H

#include <stdint.h>

struct SensorInfo {
    float currentGyro, initialGyro, diffGyro;

};

struct MotorInfo {
    uint8_t leftMotor, rightMotor, graberMotor;
};

void runStraight();
void turnLeft(double degree);
void turnRight(double degree);


#endif //OS_ROBOT_PROJECT_FALL2016_MOTORCONTROL_H
