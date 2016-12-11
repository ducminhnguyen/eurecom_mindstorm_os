//
// Created by parallels on 12/6/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ncurses.h>
#include <unistd>


#include "../libs/header/config.h"
#include "../libs/header/motorControl.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif
const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

static bool _check_pressed( uint8_t sn )
{
    int val;

    if ( sn == SENSOR__NONE_ ) {
        return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
    }
    return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

void initRobot() {
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    ev3_sensor_init();

}

int main() {
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


    printf( "Found sensors:\n" );
    for ( i = 0; i < DESC_LIMIT; i++ ) {
        if (ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
            printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
            printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
            if ( get_sensor_mode( i, s, sizeof( s ))) {
                printf( "  mode = %s\n", s );
            }
            if ( get_sensor_num_values( i, &n )) {
                for ( ii = 0; ii < n; ii++ ) {
                    if ( get_sensor_value( ii, i, &val )) {
                        printf( "  value%d = %d\n", ii, val );
                    }
                }
            }
        }
    }


    for ( ; ; ){
        if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
            printf( "COLOR sensor is found, reading COLOR...\n" );
            if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
                val = 0;
            }
            printf( "\r(%s) \n", color[ val ]);
            fflush( stdout );
        }
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro)) {
            printf("GYRO value");
            if (!get_sensor_value0(sn_compass, &value)) {
                value = 0;
            }
            printf( "\r(%f) \n", value);
            fflush( stdout );
        }

        if (ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
            printf("COMPASS found, reading compass...\n");
            if ( !get_sensor_value0(sn_compass, &value )) {
                value = 0;
            }
            printf( "\r(%f) \n", value);
            fflush( stdout );
        }
        if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
            printf("SONAR found, reading sonar...\n");
            if ( !get_sensor_value0(sn_sonar, &value )) {
                value = 0;
            }
            printf( "\r(%f) \n", value);
            fflush( stdout );
        }
        if (ev3_search_sensor(NXT_ANALOG, &sn_mag,0)){
            printf("Magnetic sensor found, reading magnet...\n");
            if ( !get_sensor_value0(sn_mag, &value )) {
                value = 0;
            }
            printf( "\r(%f) \n", value);
            fflush( stdout );
        }

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

