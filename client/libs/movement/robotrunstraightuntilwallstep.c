//
// Created by parallels on 1/20/17.
//

#include "../header/robotrunstraightuntilwallstep.h"
#include "../header/std_include.h"

// call this function in the update all function in the loop
void robotrunstraightuntilwall_update(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    update_sensor_value(sensorInfo);
    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if (sensorInfo->currentDistance < global_params.robot_steps[global_params.current_step].robot_run_straight_until_wall_distance_to_stop) {
            global_params.robot_state = ROBOT_STOP_RUNNING;
        }
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        // move to next step();
        // movetonextstep(&global_params, motorInfo, sensorInfo);
        // for individual step testing comment this
    }
}


// call this function in the run motor function of the loop
void robotrunstraightuntilwall_run_motor(MotorInfo *motorInfo, SensorInfo *sensorInfo) {
    int left_motor_speed, right_motor_speed;
    get_tacho_speed(motorInfo->leftMotor, &left_motor_speed);
    get_tacho_speed(motorInfo->rightMotor, &right_motor_speed);

    if (global_params.robot_state == ROBOT_RUN_STRAIGHT) {
        if (sensorInfo->diffGyro > 0) {// && left_motor_speed < right_motor_speed + 30) { // left tilt
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            printf("Left tilt: %f \n", sensorInfo->diffGyro);
        } else if (sensorInfo->diffGyro < 0) {// && left_motor_speed + 30 > right_motor_speed) { // right tilt
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed - 10*abs(sensorInfo->diffGyro));
            printf("Right tilt: %f \n", sensorInfo->diffGyro);
        } else {
            set_tacho_speed_sp(motorInfo->leftMotor, motorInfo->speed);
            set_tacho_speed_sp(motorInfo->rightMotor, motorInfo->speed);
        }
        set_tacho_time_sp(motorInfo->leftMotor, motorInfo->time);
        set_tacho_time_sp(motorInfo->rightMotor, motorInfo->time);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    } else if (global_params.robot_state == ROBOT_STOP_RUNNING) {
        set_tacho_speed_sp(motorInfo->leftMotor, 0);
        set_tacho_speed_sp(motorInfo->rightMotor, 0);
        set_tacho_command_inx(motorInfo->leftMotor, motorInfo->command);
        set_tacho_command_inx(motorInfo->rightMotor, motorInfo->command);
    }
}

// this function is called whenever the state is entered to ensure starting parameter of a state is always in the
// right form. Implement this function if you think at the start of this step sensor and motor need to be in a specific
// state
void robotrunstraightuntilwall_init_step(MotorInfo *motorInfo, SensorInfo *sensorInfo) {

}
