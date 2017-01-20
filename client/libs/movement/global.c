//
// Created by parallels on 1/18/17.
//

#include "../header/global.h"

int movetonextstep(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo *sensorInfo) {
    if (global_param->run_style == LOOP) {
        global_param->current_step = (++global_param->current_step) % global_param->total_step;
        global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
    } else if (global_param->run_style == ONE_WAY) {
        ++global_param->current_step;
        if (global_param->current_step == global_param->total_step) {
            global_param->current_step = 0;
            global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
            return 0;
        } else {
            global_param->robot_steps[global_param->current_step].init_step(motorInfo, sensorInfo);
        }
    }
    return 1;
}