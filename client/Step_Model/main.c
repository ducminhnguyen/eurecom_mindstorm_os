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

Robot_State robotState = ROBOT_STOP;

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

    return;
}

void run_robot(SensorInfo* sensorInfo, MotorInfo* motorInfo) { // draw

    return;
}

void ResetSensors(struct SensorInfo sensorInfo) {

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
    printf("Start program\n");
    initRobot();

    MotorInfo motorInfo;
    init_motor_info(&motorInfo);

    SensorInfo sensorInfo;
    set_sensor_initial_values(&sensorInfo);

    robotState = ROBOT_TURN_LEFT;

    while (true) {
        update_all_sensor(&sensorInfo, &motorInfo);
        run_robot(&sensorInfo, &motorInfo);
    }
}
