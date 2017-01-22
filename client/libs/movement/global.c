//
// Created by parallels on 1/18/17.
//

#include "../header/global.h"
#include <time.h>
#include <math.h>

#define PI 3.14159265

int movetonextstep(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo *sensorInfo) {
    if (global_param->run_style == LOOP) {
        global_param->current_step = (++global_param->current_step) % global_param->total_step;
        global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
    } else if (global_param->run_style == ONE_WAY) {
        ++global_param->current_step;
        if (global_param->current_step == global_param->total_step) {
            global_param->current_step = 0;
            global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
            global_param->robot_state = ROBOT_FINISH_PROGRAM;
            return 0;
        } else {
            global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
        }
    }
    return 1;
}


int update_robot_position(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo* sensorInfo) 
{
    static time_t previous_updated_time = 0;
    time_t current_time = time(NULL);

    if (global_param->robot_state == ROBOT_RUN_STRAIGHT) 
    {
        // update position based-on gyro and ultra sonic sensor
        float d_x, x_y;
        d_x = cos(PI * ((sensorInfo->currentGyro - global_param->robot_direction_x) % 360) );

    }
    

    if (current_time - previous_updated_time >= 2)
    {
        /* code */
        // send bluetooth position to server
        SendRobotPosition(global_param);
        previous_updated_time = current_time;
    }
    

}