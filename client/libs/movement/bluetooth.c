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
    *((uint16_t *)  global_params.btObj.msg) = global_params.btObj.msgId++;
    global_params.btObj.msg[2] = TEAM_ID;
    global_params.btObj.msg[3] = global_params.btObj.ally;
    global_params.btObj.msg[4] = MSG_NEXT;
    write(global_params.btObj.socket,  global_params.btObj.msg, 5);
}
/**
 * Send the current position of the robot to the server
 */
void SendRobotPosition(){
    *((uint16_t *)  global_params.btObj.msg) = global_params.btObj.msgId++;
    global_params.btObj.msg[2] = TEAM_ID;
    global_params.btObj.msg[3] = 0xFF;
    global_params.btObj.msg[4] = MSG_POSITION;
    global_params.btObj.msg[5] = global_params.btObj.pos.x;
    global_params.btObj.msg[6] = 0x00;
    global_params.btObj.msg[7] = global_params.btObj.pos.y;
    global_params.btObj.msg[8]= 0x00;
    write(global_params.btObj.socket,  global_params.btObj.msg, 9);
}

/**
 * Send ball position and the act of grab/release ball
 * @param act : the act of grab or release ball. 0->release; 1->grab
 */
void SendBallMessage(int act){
    *((uint16_t *) global_params.btObj.msg) = global_params.btObj.msgId++;
    global_params.btObj.msg[2] = TEAM_ID;
    global_params.btObj.msg[3] = global_params.btObj.info.ally;
    global_params.btObj.msg[4] = MSG_BALL;
    global_params.btObj.msg[5] = act;
    global_params.btObj.msg[6] = global_params.btObj.pos.x;
    global_params.btObj.msg[7] = 0x00;
    global_params.btObj.msg[8] = global_params.btObj.pos.y;
    global_params.btObj.msg[9] = 0x00;
    write(global_params.btObj.socket,  global_params.btObj.msg, 10);
}