//
// Created by parallels on 1/21/17.
//

#include "../header/scenario.h"
#include "../header/robotstep_headers.h"


void scenario_small_stadium_beginner_init() {
    global_params.robot_steps = (step*)malloc(sizeof(step) * 20);
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
}

void scenario_small_stadium_finisher_init() {
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

void scenario_large_stadium_left_beginner() {
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
     * release ball, go backward
     *
    */
}


void scenario_large_stadium_left_finisher() {

}

void scenario_large_stadium_right_beginner() {

}

void scenario_large_stadium_right_finisher() {

}