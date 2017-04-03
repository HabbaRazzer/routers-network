/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Router
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
#include "router_funcs.h"

#define CLIENT_PORT 8000
#define BACKLOG 10
#define NUM_ROUTERS 4
#define TABLE_LEN ('A' + NUM_ROUTERS)

const * char const ROUTING_TABLE[TABLE_LEN] = { ['A'] = "127.0.0.1", ['B'] = "other1",
    ['C'] = "other2", ['D'] = "other3" };

void *handle_request_t(void *socket);
void route_message(const char* message);

/**
 * Client request handler.
 *
 * params:
 *   socket - the socket interface to the client connection
 */
void *handle_client_t(void *socket)
{
    // maintain connection with client until client disconnects
    while( 1 )
    {
        char recv_buffer[MAX_BUFFER_SIZE] = {0};
        int status;
        int client_socket = *((int*) socket);
        if( (status = recv(client_socket, recv_buffer, sizeof recv_buffer, 0)) == -1 )
        {
            diep("router - recv() in handle_request_t");
        }

        if( status == 0 )
        {
            // client has closed the connection
            pthread_exit(NULL);
        }

        // check that the message has not been corrupted
        if( is_not_corrupt(recv_buffer[CHECK_OFFSET]) )
        {
            // ok - not corrupted. route and print message header + data
            route_message(recv_buffer);
            printf("Source - %c, Destination - %c, Message - %d%d\n", recv_buffer[SOURCE_OFFSET],
                recv_buffer[DEST_OFFSET], recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET+1]);
        }
        printf("Message corrupted.\n");
    }
}

/**
 * Determines where to send a client message and sends it.
 *
 * params:
 *   message - the message to be routed
 */
void route_message(const char* message)
{
    int router_socket = 0;

    struct sockaddr_in router_info;

    if( (router_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
        diep("router - socket() in route_message");
	}

    char* destination = ROUTING_TABLE[message[DEST_OFFSET]];
    memset(&server_info, 0, sizeof(router_info));
    router_info.sin_family = AF_INET;
    router_info.sin_addr.s_addr = inet_addr(destination);
    router_info.sin_port = htons(ROUTER_PORT);

    // establish connection with neighboring router
    if( connect(router_socket, (struct sockaddr*)&router_info, sizeof(router_info)) == -1 )
	{
        diep("router - connect() in route_message");
	}

    // send message
    if( send(router_socket, message, sizeof message, 0) == -1 )
    {
        diep("router - send() in route_message");
    }
}

int main(int argc, char *argv[])
{
    int client_socket = 0;
    struct sockaddr_in client_info;

	// obtain a socket for the client connection
    if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
        diep("router - socket() in main");
	}

    memset(&server_info, 0, sizeof server_info);
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);
    server_info.sin_port = htons(CLIENT_PORT);

	// bind the server to client port
    if( bind(client_socket, (struct sockaddr*)&server_info, sizeof server_info) == -1 )
	{
		diep("router - bind() in main");
	}

	// listen for incoming connections from client
    if( listen(client_socket, BACKLOG) == -1 )
	{
		diep("router - listen() in main");
	}

    printf("router is listening on port %d...\n", CLIENT_PORT);
	
	int client_len = sizeof client_info; 
	int new_socket = accept(client_socket, (struct sockaddr*)&client_info, 
		(socklen_t*)&client_len); 
 
	// create a new thread to handle the client
	pthread_t thread; 
	int status = 0; 
	if( (status = pthread_create(&thread, NULL, handle_client_t, (void *)&new_socket)) != 0 ) 
	{ 
		fprintf(stderr, "router - error creating thread in main! error code = %d\n", status); 
		exit(1); 
	} 

	return EXIT_SUCCESS;
}
