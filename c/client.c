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

const char * const CLIENT_ADDRS[] = { };

size_t current_value = 1;

void diep(char* s);
void calc_checksum(unsigned char* message);
bool is_not_corrupt(unsigned char* message);
void *handle_message(void* socket);
void *send_message(void);

/**
 * Handles any messages sent from a client.
 *
 * params:
 *   socket - the socket interface to the client connection 
 */
void *handle_message(void* socket)
{
  while( 1 ) 
  {
    unsigned char recv_buffer[MAX_BUFFER_SIZE] = {0};
    int status;
    int client_socket = *((int*) socket);
    if( (status = recv(client_socket, recv_buffer, sizeof recv_buffer, 0)) == -1 ) 
    {
      diep("client - recv() inside handle_message thread");
    }

    if( status == 0 ) 
    {
      // client has closed the connection
      pthread_exit(NULL);
    }
    
    // corruption check
    bool not_corrupt = is_not_corrupt(recv_buffer);

    // print the contents of the message
    if( not_corrupt )
	{
    	printf("data - %d%d, source - %d", recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET+1], 
        	recv_buffer[SOURCE_OFFSET]);
	}

    if( send(client_socket, recv_buffer, sizeof recv_buffer, 0) == -1 ) 
    {
      diep("client - send() inside handle_message thread");
    }
  }
}

/**
 * Calculates the checksum.
 */
void calc_checksum(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[DATA_OFFSET] + 
        message[DATA_OFFSET+1];
    unsigned char checksum = sum + ~sum;
    message[CHECK_OFFSET] = checksum;
}

/**
 * Checks the message to see if it is corrupted.
 *
 * return:
 *   true if not corrupted
 */
bool is_not_corrupt(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[CHECK_OFFSET] + 
        message[DATA_OFFSET] + message[DATA_OFFSET+1];
    return ~sum == 0;
}

/**
 * Sends a message to client selected at random every two seconds. 
 */
void *send_message(void) 
{
  // every two seconds - send message to client randomly selected
	// pick random client
	// connect to router
	// package message
	// send message to router
	// print data + destination to stdout
	// loop
}

/**
 * Print the error and quit.
 *
 * params:
 *  s - a string describing error / exception / problem
 */
void diep(char* s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    // start the send message loop
    pthread_t thread;
    int status = 0;
    if( (status = pthread_create(&thread, NULL, send_message, NULL)) != 0 )
    {
        fprintf(stderr, "client - error creating thread! error code = %d\n", status);
        exit(1);
    }

    int server_socket = 0;

    struct sockaddr_in server_info;
    struct sockaddr_in client_info; 

    // obtain a socket for the server
    if( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
      diep("client - socket() inside main");
    }

    memset(&server_info, 0, sizeof server_info);
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);
    server_info.sin_port = htons(PORT);

	// bind the server to port
    if( bind(server_socket, (struct sockaddr*)&server_info, sizeof server_info) == -1 ) 
	{
		diep("client - bind() inside main");
	} 

	// listen for incoming connections
    if( listen(server_socket, BACKLOG) == -1 ) 
	{
		diep("client - listen() inside main");
	}

    printf("C server is listening...\n");
    while(1)
    {
        // accept request and create new thread to service that request
        int client_len = sizeof client_info;
        int connection_socket = accept(server_socket, (struct sockaddr*)&client_info, 
            (socklen_t*)&client_len); 

        printf("accepted\n");
        pthread_t thread;
        int status = 0;
        if( (status = pthread_create(&thread, NULL, handle_request_t, (void *)&connection_socket)) != 0 ) 
        {
          fprintf(stderr, "client - error creating thread! error code = %d\n", status);
          exit(1);
        }
    }

	return EXIT_SUCCESS;
}

