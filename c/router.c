/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Router
*/
#define _XOPEN_SOURCE 600

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "router_funcs.h"

#define CLIENT_PORT "8000"
#define ROUTER_PORT "8080"
#define BACKLOG 10
#define NUM_ROUTERS 4
#define TABLE_LEN 'A' + NUM_ROUTERS

char *const ROUTING_TABLE[TABLE_LEN] = {[65] = "mct162l07", [66] = "mct162l14", [67] = "other2",
    [68] = "other3"};

void route_message(unsigned char *message);
void *handle_client_t(void *socket);

/**
 * Client request handler.
 *
 * params:
 *   socket - the socket interface to the client connection
 */
void *handle_client_t(void *socket)
{
	
	while(1)
	{
    // maintain connection with client until client disconnects
	unsigned char recv_buffer[MAX_BUFFER_SIZE] = {0};
	int status;
	int client_socket = *((int *)socket);

	if ( (status = recv(client_socket, recv_buffer, sizeof recv_buffer, 0)) == -1 )
	{
		diep("router - recv() in handle_request_t");
	}

	// check that the message has not been corrupted
	if ( is_not_corrupt(recv_buffer) )
	{
		//ok - not corrupted. route and print message header + data
		if(recv_buffer[SOURCE_OFFSET] == 'B')
		{
			close(client_socket);
		}
		route_message(recv_buffer);
		printf("Source - %c, Destination - %c, Message - %d%d\n", recv_buffer[SOURCE_OFFSET],
			   recv_buffer[DEST_OFFSET], recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET + 1]);
	} else
	{
		printf("Message corrupted.\n");
	}
	
	}
	pthread_exit(NULL);
	
}

/**
 * Determines where to send a client message and sends it.
 * * params:
 *   message - the message to be routed
 */
void route_message(unsigned char *message)
{
    int router_socket = 0;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	char *destination = ROUTING_TABLE[message[DEST_OFFSET]];
	char *dest_port;
	if(message[DEST_OFFSET] == 'A')
    {
		dest_port = CLIENT_PORT;
    }
    else if(message[DEST_OFFSET] == 'B')
    {
		dest_port = ROUTER_PORT;
    }
	
	getaddrinfo(destination, dest_port, &hints, &res);

	// get socket
    if ( (router_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1 )
    {
        diep("router - socket() in route_message");
    }   

    // establish connection with router or client
    if ( connect(router_socket, res->ai_addr, res->ai_addrlen) == -1 )
    {
        diep("router - connect() in route_message");
    }

    // send message
    if ( send(router_socket, message, sizeof message, 0) == -1 )
    {
        diep("router - send() in route_message");
    }

    close(router_socket);
}

int main(int argc, char *argv[])
{
    int router_socket = 0;
	struct addrinfo hints, *res;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, ROUTER_PORT, &hints, &res);

    // obtain a socket
    if ( (router_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1 )
    {
        diep("router - socket() in main");
    }

    // bind to port <ROUTER_PORT>
    if ( bind(router_socket, res->ai_addr, res->ai_addrlen) == -1 )
    {
        diep("router - bind() in main");
    }
	
	// configure to reuse addresses
	int optval = 1;
	if ( setsockopt(router_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1 ) 
	{
		diep("router - setsockopt() in main");
	} 

    // listening...
    if ( listen(router_socket, BACKLOG) == -1 )
    {
        diep("router - listen() in main");
    }

	pthread_t thread;
    while (1)
    {
        // block and accept when request comes in 
		struct sockaddr_storage client_info;
        socklen_t ci_size = sizeof client_info;
	int client = accept(router_socket, (struct sockaddr *)&client_info, &ci_size);
		

		// create a new thread to service that request
        int status = 0;
        if ( (status = pthread_create(&thread, NULL, handle_client_t, (void *)&client)) != 0 )
        {
            fprintf(stderr, "router - error creating thread in main! error code = %d\n",
                status);
            exit(1);
        }
    }
	pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}
