//
// Created by parallels on 1/22/17.
//

#include "../header/std_include.h"
#include "../header/robotgrabballstep.h"

static clock_t begin_time;


void robotgrabball_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    clock_t current_time = clock();

    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        printf("%f, %f\n", ((double)(current_time - begin_time)) / CLOCKS_PER_SEC, (global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run) / 1000.0f);
        if (((double)(current_time - begin_time)) / CLOCKS_PER_SEC > (global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run) / 1000.0f) {
            printf("%d\n", global_params.robot_state);
            global_params.robot_state = ROBOT_CLOSE_GRABBER;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // next step
        global_params.robot_state == ROBOT_COMPLETE_STEP;
    }

}


// call this function in the run motor function of the loop
void robotgrabball_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    if (global_params.robot_state == ROBOT_OPEN_GRABBER) {
        set_tacho_speed_sp(motorInfo->graberMotor, -GRABBER_SPEED);
        set_tacho_time_sp(motorInfo->graberMotor, 2000);
        set_tacho_command_inx(motorInfo->graberMotor, TACHO_RUN_FOREVER);
        usleep(GRABBER_TIME * 1000);
        global_params.robot_state = ROBOT_RUN_STRAIGHT;
        begin_time = clock();
    } else if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        int run_time = 200;
        if (sensorInfo->diffGyro > 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            //printf("Left tilt: %f \n", sensorInfo->diffGyro);
        } else if (sensorInfo->diffGyro < 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            //printf("Right tilt: %f \n", sensorInfo->diffGyro);
        } else {
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
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
        usleep(GRABBER_TIME * 1000);
        global_params.robot_state = ROBOT_STOP_RUNNING;
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotgrabball_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    global_params.robot_state = ROBOT_OPEN_GRABBER;
    global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run = 1000.0f;
}
