//
// Created by parallels on 1/18/17.
//

#ifndef OS_ROBOT_PROJECT_FALL2016_GLOBAL_H
#define OS_ROBOT_PROJECT_FALL2016_GLOBAL_H

#include "config.h"
#include "step.h"
#include "motorControl.h"
#define MAX_STEP 30

typedef enum {
    LOOP, ONE_WAY
} RUN_STYLE;

typedef enum {
    BEGINNER, FINISHER
} ROBOT_ROLE;

typedef enum {
   SMALL, BIG
} STADIUM;

typedef  enum {
    LEFT, RIGHT
} SIDE;

typedef enum {
    ROBOT_RUN_STRAIGHT, ROBOT_STOP_RUNNING, ROBOT_WAIT_FINISH_COMMAND,
    ROBOT_TURN_LEFT, ROBOT_TURN_RIGHT, ROBOT_COMPLETE_STEP, ROBOT_FINISH_PROGRAM, 
    ROBOT_CLOSE_GRABBER, ROBOT_OPEN_GRABBER, ROBOT_RUN_BACKWARD,
    ROBOT_TO_DESTINATION, ROBOT_WAIT_FOR_SERVER
} ROBOT_STATE; // state pool use for all steps

// Position to send to the server
typedef struct Position{
    int x;   /* Horizontal position*/
    int y;   /* Vertical position */
} Position;

// Information of the contest get from server
typedef struct GameInfo{
    int ally;
    STADIUM stadium;
    SIDE side;
    ROBOT_ROLE role;
}GameInfo;

// Bluetooth object to communicate with server
typedef struct BT_Object{
    bool havingBall;
    uint16_t msgId;
    Position pos;
    GameInfo info;
    int socket;
    char *serverAddr;
    char msg[58];
}BT_Object;

typedef struct global_parameters {
    step* robot_steps;
    int current_step, total_step;
    float calibrated_straight_angle, ideal_straight_angle, ideal_right_angle, ideal_left_angle, ideal_behind_angle;
    float ideal_target_angle;
    RUN_STYLE run_style;
    ROBOT_STATE robot_state;

   // for calculate current position
    float robot_position_x, robot_position_y;
    float robot_direction_x, robot_direction_y;

    BT_Object btObj;
} global_parameters;

extern global_parameters global_params; // to be define in main.c
// in main.c this variable must be daclare in the following fashion: global_parameters global_params;
int movetonextstep(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo* sensorInfo);

int update_robot_position(global_parameters *global_param, MotorInfo* motorInfo, SensorInfo* sensorInfo);

void init_ideal_angles();

float get_ideal_angle(float target_angle);

double get_current_time_ms();

#endif //OS_ROBOT_PROJECT_FALL2016_GLOBAL_H
