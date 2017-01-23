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

void doBeginner(){
    printf("Beginner sequence start");
    printf("Send a position message");
    SendRobotPosition(&global_params);
    printf("Send a ball message");
    SendBallMessage(&global_params, 0);
    printf("Send a next message");
    ToDestination(&global_params);
}

void doFinisher(){
    printf("Finisher sequence start");
    while(1){
        ReadServerMsg(&global_params, 58);
        unsigned char msg_type = GetMessageType(&global_params);
        switch (msg_type){
            case MSG_STOP: return;
            case MSG_NEXT:
                printf("Send a position message");
                SendRobotPosition(&global_params);
                SendBallMessage(&global_params, 1);
                return;
            case MSG_BALL:
                printf("Receive message ball position at %02X%02X,%02X%02X \n",
                       global_params.btObj.msg[7], global_params.btObj.msg[6], global_params.btObj.msg[9], global_params.btObj.msg[8]);
                break;
            default:
                printf("Ignore message %d\b", msg_type);
        }
    }
}

int main(int argc, char **argv ) {

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

    char* server_address = argv[1];
    InitBtObject(&global_params, server_address);
    //Connect to the server
    int connect_status = ConnectBtServer(&global_params);
    //Connected
    if(connect_status == 0){
        ReadServerMsg(&global_params, 9);
        if(GetMessageType(&global_params) == MSG_START){
            printf("Receive start message!\n");
            InitGameInfo(&global_params,(unsigned char) argv[2]);
        }

        if(global_params.btObj.info.role == BEGINNER){
            doBeginner();
        }
        else{
            doFinisher();
        }
        close(global_params.btObj.socket);
        sleep(5);
    } else {
        fprintf (stderr, "Failed to connect to server...\n");
        sleep (2);
        exit (EXIT_FAILURE);
    }
    close(global_params.btObj.socket);
    return 0;
}


