

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define SERV_ADDR   "dc:53:60:AD:61:90"     /* Whatever the address of the server is */
#define TEAM_ID     8                       /* Your team ID */

#define MSG_ACK     0
#define MSG_NEXT    1
#define MSG_START   2
#define MSG_STOP    3
#define MSG_CUSTOM  4
#define MSG_KICK    5
#define MSG_POSITION 6


void debug (const char *fmt, ...) {
    va_list argp;

    va_start (argp, fmt);

    vprintf (fmt, argp);

    va_end (argp);
}

unsigned char rank = 0;
unsigned char length = 0;
unsigned char previous = 0xFF;
unsigned char next = 0xFF;

int s;

uint16_t msgId = 0;

int read_from_server (int sock, char *buffer, size_t maxSize) {
    int bytes_read = read (sock, buffer, maxSize);

    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (s);
        exit (EXIT_FAILURE);
    }

    printf ("[DEBUG] received %d bytes\n", bytes_read);

    return bytes_read;
}

void beginner () {
    char string[58];

    printf ("I'm the beginner...\n");

    /* Send 3 POSITION messages, then a NEXT message */
    int i;
    for (i=0; i<3; i++){
		*((uint16_t *) string) = msgId++;
 		string[2] = TEAM_ID;
    	string[3] = 0xFF;
    	string[4] = MSG_POSITION;
    	string[5] = i;          /* x */
		string[6] = 0x00;
    	string[7] = i;		/* y */
		string[8]= 0x00;
		write(s, string, 9);
    }
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = next;
    string[4] = MSG_NEXT;
    write(s, string, 5);
}

void finisher () {
    char string[58];
    char type;

    printf ("I'm the finisher...\n");

    /* Get message */
    while (1){
        read_from_server (s, string, 58);
        type = string[4];

    	switch (type) {
        	case MSG_STOP:
        	    return;
        	case MSG_NEXT:
	    		//send a position
	    		*((uint16_t *) string) = msgId++;
 	    		string[2] = TEAM_ID;
    		    string[3] = 0xFF;
    		    string[4] = MSG_POSITION;
    		    string[5] = 0x00;          /* x */
				string[6]= 0x00;
    		    string[7] = 0x00;	    /* y */
				string[8] = 0x00;
 	   		 	write(s, string, 9);
        	    break;
        	default:
        	    printf ("Ignoring message %d\n", type);
		}
    }
}


int main(int argc, char **argv) {
    struct sockaddr_rc addr = { 0 };
    int status;

    /* allocate a socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (SERV_ADDR, &addr.rc_bdaddr);

    /* connect to server */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    /* if connected */
    if( status == 0 ) {
        char string[58];

        /* Wait for START message */
        read_from_server (s, string, 9);
        if (string[4] == MSG_START) {
            printf ("Received start message!\n");
            rank = (unsigned char) string[5];
            next = (unsigned char) string[7];
        }

        if (rank == 0)
            beginner ();
        else
            finisher ();

        close (s);

        sleep (5);

    } else {
        fprintf (stderr, "Failed to connect to server...\n");
        sleep (2);
        exit (EXIT_FAILURE);
    }

    close(s);
    return 0;
}


