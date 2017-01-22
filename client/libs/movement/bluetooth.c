//
// Created by DuongLam on 1/22/2017.
//

#include "../header/bluetooth.h"

/**
 * Initialize the value for bluetooth object
 * @param server: MAC address of the server
 */
void InitBtObject(char* server){
    global_params.btObj.pos.x = 0;
    global_params.btObj.pos.y = 0;
    global_params.btObj.serverAddr = server;
    global_params.btObj.msgId = 0;
}

/**
 * Procedure to connect to server by bluetooth
 * @return status of the connection 0 -> OK; 1 -> error
 */
int ConnectBtServer(){
    struct sockaddr_rc addr = { 0 };
    int status;

    /* allocate a socket */
    global_params.btObj.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (global_params.btObj.serverAddr, &addr.rc_bdaddr);

    /* connect to server */
    status = connect(global_params.btObj.socket, (struct sockaddr *)&addr, sizeof(addr));
}

/***
 * Function to read a message from server
 * @param buffer : memory to store the return message
 * @param maxSize : max size of the memory
 * @return message in bytes
 */
int ReadServerMsg(char *buffer, size_t maxSize){
    int bytes_read = read (global_params.btObj.socket, buffer, maxSize);
    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (global_params.btObj.socket);
        //TODO: try for reconnection
        exit (EXIT_FAILURE);
    }
    printf ("[DEBUG] received %d bytes\n", bytes_read);
    return bytes_read;
}

/**
 * Called when the robot reached its destination
 * send a next message to the server
 */
void ToDestination(){
    *((uint16_t *) string) = global_params.btObj.msgId++;
    string[2] = TEAM_ID;
    string[3] = global_params.btObj.ally;
    string[4] = MSG_NEXT;
    write(global_params.btObj.socket, string, 5);
}
/**
 * Send the current position of the robot to the server
 */
void SendRobotPosition(){
    *((uint16_t *) string) = global_params.btObj.msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    string[5] = global_params.btObj.pos.x;
    string[6] = 0x00;
    string[7] = global_params.btObj.pos.y;
    string[8]= 0x00;
    write(global_params.btObj.socket, string, 9);
}

/**
 *
 */
void SendBallMessage(){
    *((uint16_t *) string) = global_params.btObj.msgId++;
    string[2] = TEAM_ID;
    string[3] = global_params.btObj.ally;
    string[4] = MSG_BALL;
    //string[5] = //TODO: Action of grab or release ball
    string[6] = global_params.btObj.pos.x;
    string[7] = 0x00;
    string[8] = global_params.btObj.pos.y;
    string[9] = 0x00;
    write(global_params.btObj.socket, string, 10);
}