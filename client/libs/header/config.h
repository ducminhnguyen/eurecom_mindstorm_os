//
// Created by DuongLam on 12/4/2016.
//

#ifndef ROBOT_BUNBO_CONFIG_H
#define ROBOT_BUNBO_CONFIG_H


#define PORT_A 65           // port A
#define PORT_B 66           // port B
#define PORT_C 67           // port C
#define PORT_D 68           // port D


#define PORT_GRABBER 66		// port B
#define PORT_LEFT_W 67      // port C
#define PORT_RIGHT_W 68     // port D

#define GRABER_CLOSE 0		// Graber state originally always 0 (close)
#define GRABER_OPEN 1		// Open state

/* Bluetooth object */
#define TEAM_ID     8       /* Team ID*/

#define MSG_ACK             0
#define MSG_NEXT            1
#define MSG_START           2
#define MSG_STOP            3
#define MSG_CUSTOM          4
#define MSG_KICK            5
#define MSG_POSITION        6
#define MSG_BALL            7

#define INET_PORT   8888

#define LOGLINESIZE    200

#define KNRM            0
#define KRED            1
#define KGRN            2
#define KYEL            3
#define KBLU            4
#define KMAG            5
#define KCYN            6
#define KWHT            7

#define TURN_SPEED 10
#define TURN_TIME 150
#define GRABBER_SPEED 200
#define GRABBER_TIME 500
//typedef enum {
//    ROBOT_GO_STRAIGHT, ROBOT_SCAN, ROBOT_STOP, ROBOT_GRAB,
//    ROBOT_TURN_RIGHT, ROBOT_TURN_LEFT, ROBOT_GO_TIMED, ROBOT_IDLE,
//    ROBOT_RELEASE
//} Robot_State;

typedef enum {
	UNKNOW, BLACK, BLUE, GREEN, YELLOW, RED, WHITE, BROWN
} Color;



#endif //ROBOT_BUNBO_CONFIG_H
