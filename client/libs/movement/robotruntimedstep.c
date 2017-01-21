//
// Created by parallels on 1/21/17.
//

#include "../header/std_include.h"
#include "../header/robotruntimedstep.h"
static clock_t begin_time;

void robotruntimed_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    clock_t current_time = clock();
    update_sensor_value(sensorInfo);
    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if ((current_time - begin_time) / CLOCKS_PER_SEC > (global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run - 50) / 1000) {
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // next step
        global_params.robot_state == ROBOT_COMPLETE_STEP;
    }
}


// call this function in the run motor function of the loop
void robotruntimed_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    int left_motor_speed, right_motor_speed;
    get_tacho_speed(motorInfo->leftMotor, &left_motor_speed);
    get_tacho_speed(motorInfo->rightMotor, &right_motor_speed);

    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if (sensorInfo->diffGyro > 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed - 10*((motorInfo->speed > 0)*2 - 1)*abs(sensorInfo->diffGyro));
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            printf("Left tilt: %f \n", sensorInfo->diffGyro);
        } else if (sensorInfo->diffGyro < 0) {
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed - 10*((motorInfo->speed > 0)*2 - 1) *abs(sensorInfo->diffGyro));
            printf("Right tilt: %f \n", sensorInfo->diffGyro);
        } else {
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
        }
        set_tacho_time_sp(motorInfo->leftMotor, global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run);
        set_tacho_time_sp(motorInfo->rightMotor, global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run);
        set_tacho_command_inx(motorInfo->leftMotor, global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
//        usleep(global_params.robot_steps[global_params.current_step].robot_run_timed_time_to_run * 10000);
//        set_tacho_speed_sp(motorInfo->leftMotor, 0);
//        set_tacho_speed_sp(motorInfo->rightMotor, 0);
//        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
//        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
//        global_params.robot_state = ROBOT_STOP_RUNNING;
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING || global_params.robot_state == ROBOT_COMPLETE_STEP) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotruntimed_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    set_sensor_initial_values(sensorInfo);
    global_params.robot_state = ROBOT_RUN_STRAIGHT;
    motorInfo->speed = global_params.robot_steps[global_params.current_step].robot_run_timed_speed;
    begin_time = clock();
}
