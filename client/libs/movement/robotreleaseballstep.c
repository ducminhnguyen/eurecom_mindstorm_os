//
// Created by parallels on 1/21/17.
//

#include "../header/std_include.h"
#include "../header/robotreleaseballstep.h"

#define GRABBER_SPEED 200
#define GRABBER_TIME 700

static double begin_time;

void robotreleaseball_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    double current_time = get_current_time_ms() / 1000.0f;

    if (global_params.robot_state == ROBOT_OPEN_GRABBER) {
        if (((double)(current_time - begin_time)) > 1.0f) {
            global_params.robot_state = ROBOT_RUN_BACKWARD;
            begin_time = current_time;
        }
    }
    else if (global_params.robot_state == ROBOT_RUN_BACKWARD) {
        if (((double)(current_time - begin_time)) > 1.5f) {
            global_params.robot_state = ROBOT_CLOSE_GRABBER;
            begin_time = current_time;
        }
    }
    else if (global_params.robot_state == ROBOT_CLOSE_GRABBER) {
        if (((double)(current_time - begin_time)) > 0.7f) {
            global_params.robot_state = ROBOT_STOP_RUNNING;
            begin_time = current_time;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // next step
        global_params.robot_state = ROBOT_COMPLETE_STEP;
    }
}


// call this function in the run motor function of the loop
void robotreleaseball_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if (global_params.robot_state == ROBOT_OPEN_GRABBER) {
        set_tacho_speed_sp(motorInfo->graberMotor, -GRABBER_SPEED);
        set_tacho_time_sp(motorInfo->graberMotor, GRABBER_TIME);
        set_tacho_command_inx(motorInfo->graberMotor, TACHO_RUN_FOREVER);

    } else if (global_params.robot_state == ROBOT_RUN_BACKWARD) {
        int run_time = 200;
        int run_speed = motorInfo->speed < 0 ? motorInfo->speed : -motorInfo->speed;
        if (sensorInfo->diffGyro < 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, (run_speed + 10*abs(sensorInfo->diffGyro)));
            set_tacho_speed_sp(motorInfo->leftMotor, run_speed);
            //printf("Left tilt: %f \n", sensorInfo->diffGyro);
        } else if (sensorInfo->diffGyro > 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, run_speed);
            set_tacho_speed_sp(motorInfo->leftMotor, (run_speed + 10*abs(sensorInfo->diffGyro)));
            //printf("Right tilt: %f \n", sensorInfo->diffGyro);
        } else {
            set_tacho_speed_sp(motorInfo->leftMotor, run_speed);
            set_tacho_speed_sp(motorInfo->rightMotor, run_speed);
        }
        set_tacho_time_sp(motorInfo->leftMotor, run_time);
        set_tacho_time_sp(motorInfo->rightMotor, run_time);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    } else if (global_params.robot_state == ROBOT_CLOSE_GRABBER) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_STOP);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_STOP);

        set_tacho_speed_sp(motorInfo->graberMotor, GRABBER_SPEED);
        set_tacho_time_sp(motorInfo->graberMotor, GRABBER_TIME);
        set_tacho_command_inx(motorInfo->graberMotor, TACHO_RUN_TIMED);
        
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_speed_sp(motorInfo->graberMotor, 0);
        set_tacho_command_inx(motorInfo->graberMotor, TACHO_STOP);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_STOP);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_STOP);
    } else if (global_params.robot_state == ROBOT_COMPLETE_STEP) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_speed_sp(motorInfo->graberMotor, 0);
        set_tacho_command_inx(motorInfo->graberMotor, TACHO_RESET);
        set_tacho_command_inx(motorInfo->leftMotor, TACHO_RESET);
        set_tacho_command_inx(motorInfo->rightMotor, TACHO_RESET);
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotreleaseball_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    begin_time = get_current_time_ms() / 1000.0f;
    global_params.robot_state = ROBOT_OPEN_GRABBER;
    motorInfo->speed = 250;
    global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run = 1000.0f;
    global_params.robot_steps[global_params.current_step].robot_run_timed_speed = 200.0f;
}
