/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Helpful Functions (implementation) 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "router_funcs.h"

void diep(char* message)
{
    perror(message);
    exit(1);
}

void calc_checksum(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[DATA_OFFSET] + 
        message[DATA_OFFSET+1];
    unsigned char checksum = ~sum;

    message[CHECK_OFFSET] = checksum;
}

bool is_not_corrupt(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[CHECK_OFFSET] + 
        message[DATA_OFFSET] + message[DATA_OFFSET+1];
	
    return sum == 255;
}
