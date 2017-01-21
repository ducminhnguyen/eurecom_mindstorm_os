//
// Created by parallels on 1/20/17.
//

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "all_headers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

/// define all global variable before use, declare global in global.h
global_parameters global_params;
//////////////////////////////////////////

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

void initRobot() {
    while (ev3_tacho_init() < 1 ) Sleep( 1000 );
    ev3_sensor_init();
}

static bool _check_pressed( uint8_t sn )
{
    int val;

    if ( sn == SENSOR__NONE_ ) {
        return ( ev3_read_keys((uint8_t*) &val ) && ( val & EV3_KEY_UP ));
    }
    return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

void update_all_sensor(SensorInfo* sensorInfo, MotorInfo* motorInfo) { // update
    if (global_params.robot_state == ROBOT_COMPLETE_STEP) {
        movetonextstep(&global_params, motorInfo, sensorInfo);
    }
    global_params.robot_steps[global_params.current_step].update_all(motorInfo, sensorInfo);
    return;
}

void run_robot(SensorInfo* sensorInfo, MotorInfo* motorInfo) { // draw
    global_params.robot_steps[global_params.current_step].run_motor(motorInfo, sensorInfo);
    return;
}

void ResetSensors(struct SensorInfo sensorInfo) {

}

void init_robot_steps(MotorInfo* motorInfo, SensorInfo* sensorInfo) {
    global_params.robot_steps = (step*)malloc(sizeof(step) * 30);
    global_params.total_step = 1;
    global_params.run_style = ONE_WAY;
    global_params.current_step = 0;

    global_params.robot_steps[0].robot_run_straight_until_wall_distance_to_stop = 30;
    global_params.robot_steps[0].robot_run_timed_time_to_run = 2;
    global_params.robot_steps[0].robot_run_timed_speed = -400;
    global_params.robot_steps[0].robot_turn_left_degree = 90;
    global_params.robot_steps[0].robot_turn_right_degree = 10;
    global_params.robot_steps[0].init_step = &robotturnright_init_step;
    global_params.robot_steps[0].run_motor = &robotturnright_run_motor;
    global_params.robot_steps[0].update_all = &robotturnright_update;

    global_params.robot_steps[global_params.current_step].init_step(motorInfo, sensorInfo);
}

int main( void ) {

#ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "192.168.0.204";

#endif
    if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
    printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
    printf( "Waiting tacho is plugged...\n" );

#endif

    initRobot();
    printf("Start program\n");


    MotorInfo motorInfo;
    init_motor_info(&motorInfo);

    stopRobot(motorInfo);

    SensorInfo sensorInfo;
    set_sensor_initial_values(&sensorInfo);
    printf("finish init\n");


    init_robot_steps(&motorInfo, &sensorInfo);

    global_params.calibrated_straight_angle = get_gyro_sensor_value();

    while (true) {
        update_all_sensor(&sensorInfo, &motorInfo);
        run_robot(&sensorInfo, &motorInfo);
    }
}

