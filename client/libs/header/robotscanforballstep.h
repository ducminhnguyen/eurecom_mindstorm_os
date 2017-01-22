//
// Created by parallels on 1/20/17.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_ROBOTSCANFORBALLSTEP_H
#define OS_ROBOT_PROJECT_FALL2016_ROBOTSCANFORBALLSTEP_H

// array for saving value


// call this function in the update all function in the loop
void robotscanforball_update(MotorInfo *motorInfo, SensorInfo *sensorInfo);


// call this function in the run motor function of the loop
void robotscanforball_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo);

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotscanforball_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo);

#endif //OS_ROBOT_PROJECT_FALL2016_ROBOTSCANFORBALLSTEP_H
