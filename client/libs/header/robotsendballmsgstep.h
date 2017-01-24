//
// Created by DuongLam on 1/23/2017.
//

#ifndef ROBOT_BUNBO_ROBOTSENDBALLMSGSTEP_H
#define ROBOT_BUNBO_ROBOTSENDBALLMSGSTEP_H


// call this function in the update all function in the loop
void robotsendballmsg_update(MotorInfo *motorInfo, SensorInfo *sensorInfo);
// call this function in the run motor function of the loop
void robotsendballmsg_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo);
// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotsendballmsg_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo);

#endif //ROBOT_BUNBO_ROBOTSENDBALLMSGSTEP_H
