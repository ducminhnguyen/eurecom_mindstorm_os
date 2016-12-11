//
// Created by parallels on 12/6/16.
//


#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "../libs/header/config.h"
#include "../libs/header/motorControl.h"

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
        return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
    }
    return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

float getInitialGyroValue() {
    uint8_t sn_gyro;
    bool stop = false;
    float value = 0;
    while (!stop) {
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0)) {
            stop = true;
            if (!get_sensor_value0(sn_gyro, &value)) {
                value = 0;
            }
        }
    }
    return value;
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
    int i;
    uint8_t sn;
    FLAGS_T state;
    uint8_t sn_touch;
    uint8_t sn_color;
    uint8_t sn_compass;
    uint8_t sn_sonar;
    uint8_t sn_mag;
    uint8_t sn_gyro;
    char s[ 256 ];
    int val;
    float value;
    uint32_t n, ii;

    float initialGyro = getInitialGyroValue();

    for ( ; ; ){

        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0 )) {
            printf("GYRO value");
            if (!get_sensor_value0(sn_gyro, &value)) {
                value = 0;
            }
            printf( "\r(%f) \n", initialGyro - value);
            fflush( stdout );
        }
//        if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
//            printf( "COLOR sensor is found, reading COLOR...\n" );
//            if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
//                val = 0;
//            }
//            printf( "\r(%s) \n", color[ val ]);
//            fflush( stdout );
//        }
//        if (ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
//            printf("COMPASS found, reading compass...\n");
//            if ( !get_sensor_value0(sn_compass, &value )) {
//                value = 0;
//            }
//            printf( "\r(%f) \n", value);
//            fflush( stdout );
//        }
//        if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
//            printf("SONAR found, reading sonar...\n");
//            if ( !get_sensor_value0(sn_sonar, &value )) {
//                value = 0;
//            }
//            printf( "\r(%f) \n", value);
//            fflush( stdout );
//        }
//        if (ev3_search_sensor(NXT_ANALOG, &sn_mag,0)){
//            printf("Magnetic sensor found, reading magnet...\n");
//            if ( !get_sensor_value0(sn_mag, &value )) {
//                value = 0;
//            }
//            printf( "\r(%f) \n", value);
//            fflush( stdout );
//        }

        if ( _check_pressed( sn_touch )) break;
        Sleep( 200 );
        printf( "\r        " );
        fflush( stdout );
        if ( _check_pressed( sn_touch )) break;
        Sleep( 200 );
    }

    ev3_uninit();
    printf( "*** ( EV3 ) Bye! ***\n" );
    return 0;
}

