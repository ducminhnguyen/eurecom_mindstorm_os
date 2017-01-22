#include <pthread>

#include "./libs/header/config.h"
#include "./libs/header/motorControl.h"


typedef struct SensorState {
    uint16_t color;
    uint16_t us;
    uint16_t gyro;
} SensorState;

typedef enum {BEGINNER, FINISHER} robot_role;
typedef enum {TEAM_LEFT, TEAM_RIGHT} team_side;
typedef enum {SMALL_STADIUM, BIG_STADIUM} stadium;

typedef enum {ROBOT_GO_STRAIGHT, ROBOT_SCAN, ROBOT_STOP, ROBOT_GRAB,
    ROBOT_TURN_RIGHT, ROBOT_TURN_LEFT, ROBOT_GO_TIMED, ROBOT_IDLE,
    ROBOT_RELEASE
} robot_state;


robot_state current_robot_state = ROBOT_IDLE;

void *send_position(void *arg)
{
    while(!done)
    {
        sleep(2);
        call_function(); // send x,y position
    }
    return 0;
}

void communicate_with_server() {
    if (current_robot_state == ROBOT_IDLE) {
        // get message from server
    } else {

    }
}


void update_everything() {
    


}

void steer_robot() {

}

// 
int main() {
    // Initialize
    initialize_bt("ip_server");
    initialize_sensor_state(&sensor);

    // get roles and other information from server

    // when receive start message

    // create a thread to send position message
    pthread_t tid;
    pthread_create(&tid, NULL, &send_position, NULL);

    // start
    while (TRUE) {


        // read info from bluetooth and sensor
        update_everything(&motor, &sensor);


        //steer robot
        steer_robot(&motor, &sensor);

        // set updates per seconds if possible


    }
}