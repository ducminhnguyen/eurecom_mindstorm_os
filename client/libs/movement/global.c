//
// Created by parallels on 1/18/17.
//

#include "../header/global.h"
#include <time.h>
#include <math.h>
#include "../header/bluetooth.h"

#define PI 3.14159265

int movetonextstep(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo *sensorInfo) {
    if (global_param->run_style == LOOP) {
        global_param->current_step = (++global_param->current_step) % global_param->total_step;
        global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
    } else if (global_param->run_style == ONE_WAY) {
        ++global_param->current_step;
        if (global_param->current_step == global_param->total_step) {
            global_param->current_step = 0;
            global_param->robot_state = ROBOT_FINISH_PROGRAM;
            return 0;
        } else {
            global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
        }
    }
    return 1;
}


int update_robot_position(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo* sensorInfo) {
    static time_t previous_updated_time = 0;
    static float previous_us_value = -1.0;
    time_t current_time = time(NULL);

    if (global_param->robot_state == ROBOT_RUN_STRAIGHT) {
        // update position based-on gyro and ultra sonic sensor
        if (previous_us_value > 0) { // not the first time to check when run straight
            float d_x, d_y, d_d;
            d_d = sensorInfo->currentDistance - previous_us_value; // difference in us distance
            if (d_d > 1.0) { // in case sensor get right value
                d_x = cos(PI * ((sensorInfo->currentGyro - global_param->robot_direction_x) / 180.0)) * d_d;
                d_y = cos(PI * ((sensorInfo->currentGyro - global_param->robot_direction_y) / 180.0)) * d_d;

                global_param->robot_position_x += d_x;
                global_param->robot_position_y += d_y;
                previous_us_value = sensorInfo->currentDistance;
            }
        }
        else 
            previous_us_value = sensorInfo->currentDistance;
        
    }
    else { // reset value when not run straight 
        previous_us_value = -1.0;
    }
    

    if (current_time - previous_updated_time >= 2) {
        /* code */
        // send bluetooth position to server
        global_param->btObj.pos.x = global_param->robot_position_x;
        global_param->btObj.pos.y = global_param->robot_position_y;
        SendRobotPosition(global_param);
        previous_updated_time = current_time;
    }
    

}