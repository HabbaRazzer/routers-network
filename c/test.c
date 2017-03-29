/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Client 
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

#define MAX_BUFFER_SIZE 5
#define SOURCE_OFFSET 0
#define DEST_OFFSET 1
#define CHECK_OFFSET 2
#define DATA_OFFSET 3
#define ROUTER_ADDR "127.0.0.1"

//temp ips for testing
const char * const CLIENT_ADDRS[] = {"A","B","C"};

size_t current_value = 1;

void calc_checksum(unsigned char* message);
void *send_message(void);


 


/**
 * Calculates the checksum.
 */
void calc_checksum(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[DATA_OFFSET] + 
        message[DATA_OFFSET+1];
    unsigned char checksum = ~sum;
    message[CHECK_OFFSET] = checksum;
}


/**
 * Sends a message to client selected at random every two seconds. 
 */
void *send_message(void) 
{
    srand((unsigned)time(NULL));
    int counter = 1;
    int value;
    
    while(1)// loop
    {
	// connect to router
        value = (rand()%3);// pick random client
        unsigned char message[MAX_BUFFER_SIZE];
        message[SOURCE_OFFSET] = *ROUTER_ADDR;
        message[DEST_OFFSET] = *CLIENT_ADDRS[value];
        message[DATA_OFFSET] = counter;
        message[DATA_OFFSET + 1] = counter;
        calc_checksum(message);// package message

        printf("Message form c client #1: %c, %c, %d, %d, %d \n", message[SOURCE_OFFSET], message[DEST_OFFSET], 
                        message[DATA_OFFSET], message[DATA_OFFSET+1], message[CHECK_OFFSET]);// print data + destination to stdout
        counter++;
	// send message to router
        fflush(stdout);
        sleep(2); // every two seconds - send message to client randomly selected
    }
}



int main(int argc, char *argv[])
{

   
    // start the send message loop
   send_message();

	return EXIT_SUCCESS;
}

