//
// Created by DuongLam on 1/22/2017.
//

#include "../header/bluetooth.h"

int readfromserver(bluetooth_object bt_obj, char *buffer, size_t maxSize){
    int bytes_read = read (bt_obj.socket, buffer, maxSize);
    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (bt_obj.socket);
        exit (EXIT_FAILURE);
    }
    printf ("[DEBUG] received %d bytes\n", bytes_read);
    return bytes_read;
}