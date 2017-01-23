//
// Created by parallels on 1/20/17.
//

#include "../header/robotturnlefttoidealanglestep.h"
#include "../header/std_include.h"

// call this function in the update all function in the loop
void robotturnlefttoidealangle_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    if (global_params.robot_state == ROBOT_TURN_LEFT) {
        //printf("update: %f\n", (float)fabsf(fabsf(sensorInfo->diffGyro) - global_params.robot_steps[global_params.current_step].robot_turn_left_degree));
        if((float)fabsf(sensorInfo->diffGyro) >= (global_params.robot_steps[global_params.current_step].robot_turn_left_degree) - 5.0f){
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // move to next step();
        // movetonextstep(&global_params, motorInfo, sensorInfo);
        // for individual step testing comment this
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}


// call this function in the run motor function of the loop
void robotturnlefttoidealangle_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if(global_params.robot_state == ROBOT_TURN_LEFT) {
        //printf("running: %d\n", TURN_TIME);
        set_tacho_speed_sp(motorInfo->leftMotor, -60);
        set_tacho_speed_sp(motorInfo->rightMotor, 60);
        set_tacho_time_sp(motorInfo->leftMotor, TURN_TIME);
        set_tacho_time_sp(motorInfo->rightMotor, TURN_TIME);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_RUN_TIMED);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_RUN_TIMED);
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING || global_params.robot_state == ROBOT_COMPLETE_STEP) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_STOP);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_STOP);
    }
}

int get_min_position_array(float* arr, int size) {
    int position = 0;
    float min_val = arr[0];
    for (int i = 1; i < size ; ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
            position = i;
        }
    }
    return position;
}

float angle_diff_normalized(float pivot_angle, float target_angle) {
    float diff_with_ideal_straight = (pivot_angle - target_angle) / 360.0f;
    float normalize_angle = (diff_with_ideal_straight - ((int)diff_with_ideal_straight)) * 360.0f;
    if (normalize_angle < 0) {
        if (fabsf(normalize_angle) > fabsf(normalize_angle + 360)) {
            return normalize_angle + 360;
        } else {
            return normalize_angle;
        }
    } else if (normalize_angle > 0) {
        if (fabsf(normalize_angle) > fabsf(normalize_angle - 360)) {
            return normalize_angle - 360;
        } else {
            return normalize_angle;
        }
    } else {
        return normalize_angle;
    }
}

float get_ideal_angle(float target_angle) {
    float arr[4], absarr[4];
    arr[0] = angle_diff_normalized(global_params.ideal_straight_angle, target_angle);
    absarr[0] = fabsf(arr[0]);
    arr[1] = angle_diff_normalized(global_params.ideal_left_angle, target_angle);
    absarr[1] = fabsf(arr[1]);
    arr[2] = angle_diff_normalized(global_params.ideal_right_angle, target_angle);
    absarr[2] = fabsf(arr[2]);
    arr[3] = angle_diff_normalized(global_params.ideal_behind_angle, target_angle);
    absarr[3] = fabsf(arr[3]);

    int position = get_min_position_array(absarr, 4);

    if (position == 0) {
        return target_angle + arr[0];
    } else if (position == 1) {
        return target_angle + arr[1];
    } else if (position == 2) {
        return target_angle + arr[2];
    } else {
        return target_angle + arr[3];
    }
}


// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotturnlefttoidealangle_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    update_sensor_value(sensorInfo);
    // calculate ideal angle and fix
    float target_angle = sensorInfo->currentGyro - global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle;

    global_params.robot_steps[global_params.current_step].robot_turn_left_to_ideal_angle = fabsf(sensorInfo->currentGyro - get_ideal_angle(target_angle));

    global_params.robot_state = ROBOT_TURN_LEFT;
}



