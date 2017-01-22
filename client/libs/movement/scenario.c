//
// Created by parallels on 1/21/17.
//

#include "../header/scenario.h"
#include "../header/robotstep_headers.h"


void scenario_small_stadium_beginner_init(MotorInfo* motorInfo, SensorInfo* sensorInfo) {
    global_params.robot_steps = (step*)malloc(sizeof(step) * 20);
    global_params.total_step = 4;
    global_params.current_step = 0;
    step* stepArr = global_params.robot_steps;
    /*
     * run straight until black line
     * turn left 90 degree
     * run straight until black line
     * release ball and step back and close grabber
     * turn around 180
     * run straight until wall 20 cm
     * turn left
     * run straight until wall 30 cm
     * turn around 180
     * send signal end
     * wait server
     * run straight until black line
     * turn right 90
     * run straight for 3-5 sec
     * scan for ball
     * run to ball
     * grab ball
     * turn around 180
     * run until wall 20 cm
     * turn right 90 degree
     * run until wall 30 cm
     * turn 180
    */
    stepArr[0].init_step = &robotrunstraight_init_step;
    stepArr[0].run_motor = &robotrunstraight_run_motor;
    stepArr[0].update_all = &robotrunstraight_update;

    stepArr[1].init_step = &robotruntimed_init_step;
    stepArr[1].run_motor = &robotruntimed_run_motor;
    stepArr[1].update_all = &robotruntimed_update;
    stepArr[1].robot_run_timed_time_to_run = 300;
    stepArr[1].robot_run_timed_speed = 300;

    stepArr[2].init_step = &robotturnleft_init_step;
    stepArr[2].run_motor = &robotturnleft_run_motor;
    stepArr[2].update_all = &robotturnleft_update;
    stepArr[2].robot_turn_left_degree = 90.0f;

    stepArr[3].init_step = &robotrunstraight_init_step;
    stepArr[3].run_motor = &robotrunstraight_run_motor;
    stepArr[3].update_all = &robotrunstraight_update;

//    stepArr[3].init_step = &robotrunstraight_init_step;
//    stepArr[3].run_motor = &robotrunstraight_run_motor;
//    stepArr[3].update_all = &robotrunstraight_update;



//    stepArr[5].init_step = &robotturnleft_init_step;
//    stepArr[5].run_motor = &robotturnleft_run_motor;
//    stepArr[5].update_all = &robotturnleft_update;
    stepArr[0].init_step(motorInfo, sensorInfo);
}

void scenario_small_stadium_finisher_init(MotorInfo* motorInfo, SensorInfo *sensorInfo) {
    /*
     * wait server
     * run straight until black line
     * turn left 90
     * run straight for 3-5 sec (calibrate)
     * scan for ball
     * run to ball
     * grab ball
     * turn around 180
     * run until wall 20 cm
     * turn left 90 degree
     * run until wall 30 cm
     * turn 180
     *
     * run straight until black line
     * turn right 90 degree
     * run straight until black line
     * release ball and step back and close grabber
     * turn around 180
     * run straight until wall 20 cm (calibrate)
     * turn right 90
     * run straight until wall 30 cm (calibrate)
     * turn around 180
    */
}

void scenario_large_stadium_left_beginner(MotorInfo* motorInfo, SensorInfo *sensorInfo) {
    /*
     * run straight until wall 20 cm
     * turn left 90
     * run straight 5 sec (calibrate)
     * turn right 90
     * run straight until wall 20 cm (calibrate)
     * turn right 90
     * run straight until black line
     * run straight 1 sec (calibrate)
     * turn around 180
     * release ball, go backward, close grabber
     * turn around 180
     * run straight 2 sec (calibrate)
     * turn left 90
     * run straight until wall or black line
     * turn around 180
     * send signal to server
     * wait server
    */
}


void scenario_large_stadium_left_finisher(MotorInfo* motorInfo, SensorInfo *sensorInfo) {

}

void scenario_large_stadium_right_beginner(MotorInfo* motorInfo, SensorInfo *sensorInfo) {

}

void scenario_large_stadium_right_finisher(MotorInfo* motorInfo, SensorInfo *sensorInfo) {

}