// header include
#include "../header/motorControl.h"
#include "../header/robotturnleftstep.h"
#include "../header/global.h"
#include "../header/std_include.h"
#include "../header/robotturnrightstep.h"
#include "../header/robotrunstraightuntilwallstep.h"
#include "../header/robotrunstraightstep.h"

// Update the sensor information
// Calculate condition to change ROBOT_STATE and ROBOT step
// Update the ROBOT_STATE and the step


static float robotscanforball_min_dis = 20000;
static float robotscanforball_min_angle = 2 << 15 - 1;
static uint8_t robotscanforball_current_step = 0;
static step * global_current_step_pt = 0;

void robotscanforball_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    static uint8_t ball_found = 0;
    update_sensor_info(sensorInfo);

    // check for the obstacle
    if (sensorInfo->currentDistance <= 5.0) {
        global_params.robot_state = ROBOT_STOP_RUNNING;
        robotscanforball_current_step = 4;
        return;
    }


    if (robotscanforball_current_step == 0) { // turn left
        robotturnleft_update(motorInfo, sensorInfo);
    }
    else if (robotscanforball_current_step == 1) { // turn right
        robotturnright_update(motorInfo, sensorInfo);
        printf("Distance %f, %f\n", sensorInfo->currentDistance, robotscanforball_min_dis);
        // check for distance
        if (sensorInfo->currentDistance < robotscanforball_min_dis) {
            robotscanforball_min_dis = sensorInfo->currentDistance;
            robotscanforball_min_angle = sensorInfo->currentGyro;
        }
    
    } 
    else if (robotscanforball_current_step == 2) { // turn back to ball direction
        robotturnleft_update(motorInfo, sensorInfo);
    } 
    else if (robotscanforball_current_step == 3) { // in case can not find ball in short distance
        robotrunstraight_update(motorInfo, sensorInfo);
    }
    else if (robotscanforball_current_step == 4) {
        robotrunstraightuntilwall_update(motorInfo, sensorInfo);
    }


    if (global_params.robot_state == ROBOT_COMPLETE_STEP) {
        printf("Complete step %d\n", robotscanforball_current_step);
        switch (robotscanforball_current_step) {
            case 0: // change from turn left to right
                global_params.robot_state = ROBOT_TURN_RIGHT;
                robotscanforball_current_step = 1;
                break;
            case 1: // change turn state
                global_params.robot_state = ROBOT_TURN_LEFT;
                robotscanforball_current_step = 2; 
                if (robotscanforball_min_dis < 20.0) {
                    ball_found = 1;
                    global_current_step_pt->robot_turn_left_degree = 
                        abs(robotscanforball_min_angle - sensorInfo->currentGyro);
                }
                else {
                    global_current_step_pt->robot_turn_left_degree = 
                        abs(sensorInfo->currentGyro - sensorInfo->initialGyro);
                }
                break;
            case 2: // after finding ball direction, move to it
                if (ball_found == 1) {
                    robotscanforball_current_step = 4;
                    global_current_step_pt->robot_run_straight_until_wall_distance_to_stop = 6.0; // stop before hit the ball 6 cm
                }
                else { // if not, move forward a little bit
                    robotscanforball_current_step = 3;
                    global_current_step_pt->robot_run_timed_time_to_run = 0.5;
                }
                global_params.robot_state = ROBOT_RUN_STRAIGHT;
                break;
            case 3:
                global_current_step_pt->robot_turn_left_degree = 30;
                break;
            case 4:
                ball_found = 1;
                return;
                break;
        }
    }

}


// call this function in the run motor function of the loop
void robotscanforball_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if (robotscanforball_current_step == 0) { // turn left
        robotturnleft_run_motor(motorInfo, sensorInfo);
    }
    else if (robotscanforball_current_step == 1) { // turn right
        robotturnright_run_motor(motorInfo, sensorInfo);
    } 
    else if (robotscanforball_current_step == 2) { // turn back to ball direction or original direction
        robotturnleft_run_motor(motorInfo, sensorInfo);
    } 
    else if (robotscanforball_current_step == 3) { // in case can not find ball in short distance
        robotrunstraight_update(motorInfo, sensorInfo);
    }
    else if (robotscanforball_current_step == 4) {
        robotrunstraightuntilwall_update(motorInfo, sensorInfo);
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotscanforball_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    robotscanforball_min_dis = 20000;
    robotscanforball_min_angle = 2 << 15 - 1;
    robotscanforball_current_step = 0;

    global_current_step_pt = &global_params.robot_steps[global_params.current_step];

    global_current_step_pt->robot_turn_left_degree = 30;
    global_current_step_pt->robot_turn_right_degree = 60;

    global_params.robot_state = ROBOT_TURN_LEFT;
}