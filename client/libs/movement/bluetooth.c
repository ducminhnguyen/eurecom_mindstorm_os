//
// Created by DuongLam on 1/22/2017.
//

#include "../header/bluetooth.h"

/**
 * Initialize the value for bluetooth object
 * @param server: MAC address of the server
 */
void InitBtObject(global_parameters *glo_param, char *server){
    glo_param->btObj.pos.x = 0;
    glo_param->btObj.pos.y = 0;
    glo_param->btObj.serverAddr = server;
    glo_param->btObj.msgId = 0;
}

/**
 * Init information for the game
 * @param stadium : 0-> small stadium; 1->big stadium
 */
void InitGameInfo(global_parameters *glo_param, unsigned char stadium){
    unsigned char role = (unsigned char) glo_param->btObj.msg[5];
    unsigned char side = (unsigned char) glo_param->btObj.msg[6];
    glo_param->btObj.info.ally = (uint16_t) glo_param->btObj.msg[7];
    if(role == 0){ glo_param->btObj.info.role = BEGINNER; }
    else { glo_param->btObj.info.role = FINISHER; }
    if(stadium == 0){ glo_param->btObj.info.stadium = SMALL; }
    else { glo_param->btObj.info.stadium = BIG; }
    if(side == 1){ glo_param->btObj.info.side = LEFT; }
    else { glo_param->btObj.info.side = RIGHT; }
}

/**
 * Procedure to connect to server by bluetooth
 * @return status of the connection 0 -> OK; 1 -> error
 */
int ConnectBtServer(global_parameters *glo_param){
    struct sockaddr_rc addr = { 0 };
    int status;

    /* allocate a socket */
    glo_param->btObj.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (glo_param->btObj.serverAddr, &addr.rc_bdaddr);

    /* connect to server */
    status = connect(glo_param->btObj.socket, (struct sockaddr *)&addr, sizeof(addr));
}

/***
 * Function to read a message from server
 * @param maxSize : max size of the memory
 * @return message in bytes
 */
int ReadServerMsg(global_parameters *glo_param, size_t maxSize){
    int bytes_read = read (glo_param->btObj.socket, glo_param->btObj.msg, maxSize);
    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (glo_param->btObj.socket);
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
void ToDestination(global_parameters *glo_param){
    *((uint16_t *)  glo_param->btObj.msg) = glo_param->btObj.msgId++;
    glo_param->btObj.msg[2] = TEAM_ID;
    glo_param->btObj.msg[3] = glo_param->btObj.info.ally;
    glo_param->btObj.msg[4] = MSG_NEXT;
    write(glo_param->btObj.socket,  glo_param->btObj.msg, 5);
}
/**
 * Send the current position of the robot to the server
 */
void SendRobotPosition(global_parameters *glo_param){
    unsigned char special = 0xFF;
    *((uint16_t *)  glo_param->btObj.msg) = glo_param->btObj.msgId++;
    glo_param->btObj.msg[2] = TEAM_ID;
    glo_param->btObj.msg[3] = special;
    glo_param->btObj.msg[4] = MSG_POSITION;
    glo_param->btObj.msg[5] = glo_param->btObj.pos.x;
    glo_param->btObj.msg[6] = 0x00;
    glo_param->btObj.msg[7] = glo_param->btObj.pos.y;
    glo_param->btObj.msg[8] = 0x00;
    write(glo_param->btObj.socket, glo_param->btObj.msg, 9);
}

/**
 * Send ball position and the act of grab/release ball
 * @param act : the act of grab or release ball. 0->release; 1->grab
 */
void SendBallMessage(global_parameters *glo_param,int act){
    *((uint16_t *) glo_param->btObj.msg) = glo_param->btObj.msgId++;
    glo_param->btObj.msg[2] = TEAM_ID;
    glo_param->btObj.msg[3] = glo_param->btObj.info.ally;
    glo_param->btObj.msg[4] = MSG_BALL;
    glo_param->btObj.msg[5] = act;
    glo_param->btObj.msg[6] = glo_param->btObj.pos.x;
    glo_param->btObj.msg[7] = 0x00;
    glo_param->btObj.msg[8] = glo_param->btObj.pos.y;
    glo_param->btObj.msg[9] = 0x00;
    write(glo_param->btObj.socket,  glo_param->btObj.msg, 10);
}