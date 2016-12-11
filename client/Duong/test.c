//
// Created by DuongLam on 12/4/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "../libs/header/config.h"
#include "../libs/header/motorControl.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

#endif
int main( void ){

    int i;
    char s[256];

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
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

    printf( "*** ( EV3 ) Hello! ***\n" );

    printf( "Found tacho motors:\n" );
    for ( i = 0; i < DESC_LIMIT; i++ ) {
        if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ ) {
            printf( "  type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
            printf( "  port = %s\n", ev3_tacho_port_name( i, s ));
            printf("  port = %d %d\n", ev3_tacho_desc_port(i), ev3_tacho_desc_extport(i));
        }
    }

    // Test go straight for 5 seconds
    runStraight();

    // End connection
    ev3_uninit();
    printf( "*** ( EV3 ) Bye! ***\n" );
    return ( 0 );
}

//void initTachoData(uint8_t* sn,int max_speed){
//    set_tacho_stop_action_inx(*sn, TACHO_COAST );
//    set_tacho_speed_sp(*sn, max_speed * 2 / 3 );
//    set_tacho_time_sp(*sn, 5000 );
//    set_tacho_ramp_up_sp(*sn, 2000 );
//    set_tacho_ramp_down_sp(*sn, 2000 );
//}
//
//void runStraight(){
//    uint8_t snl, snr;
//    uint8_t *lp = &snl, *rp = &snr;
//    FLAGS_T state;
//    if ( ev3_search_tacho_plugged_in(PORT_LEFT_W, 0, &snl, 0 ) &&
//            ev3_search_tacho_plugged_in(PORT_RIGHT_W,0, &snr, 0 )) {
//        int max_speed;
//
//        printf( "TESTING GOING STRAIGHT , run for 5 sec...\n" );
//        get_tacho_max_speed( snr, &max_speed );
//        printf("  max speed = %d\n", max_speed );
//        initTachoData(lp, max_speed);
//        initTachoData(rp, max_speed);
//        set_tacho_command_inx(snl, TACHO_RUN_TIMED );
//        set_tacho_command_inx(snr, TACHO_RUN_TIMED );
//        /* Wait tacho stop */
//        Sleep( 100 );
//        do {
//            get_tacho_state_flags( snr, &state );
//        } while ( state );
////        printf( "run to relative position...\n" );
////        set_tacho_speed_sp( sn, max_speed / 2 );
////        set_tacho_ramp_up_sp( sn, 0 );
////        set_tacho_ramp_down_sp( sn, 0 );
////        set_tacho_position_sp( sn, 90 );
////        for ( i = 0; i < 8; i++ ) {
////            set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
////            Sleep( 500 );
////        }
//
//    } else {
//        printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
//    }
//}


