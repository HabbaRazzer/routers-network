/*
 * Nick Martinez, Robert Windisch
 * CSC434 - Computer Networks
 * Client-Server (C Implementation) Client 
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

#define MAX_BUFFER_SIZE 10
#define PORT 4446
#define SERVER_ADDR "127.0.0.1"
#define NUM_MESSAGES 10

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
    int server_socket = 0;

    struct sockaddr_in server_info;

    if( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{	
        diep("client - socket()");
	}

    memset(&server_info, 0, sizeof(server_info));

    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_info.sin_port = htons(PORT); 

    // establish connection with server
    if( connect(server_socket, (struct sockaddr*)&server_info, sizeof(server_info)) == -1 )
	{
        diep("client - connect()"); 
	}

    // send 10 messages to be processed by server
    size_t i;
    char send_buffer[MAX_BUFFER_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    char receive_buffer[MAX_BUFFER_SIZE] = {0};
    printf("C client is running...\n");
    for( i = 0; i < NUM_MESSAGES; ++i ) 
    {
      size_t j;
      printf("Sent - ");
      for( j = 0; j < MAX_BUFFER_SIZE; ++j ) 
      {
        printf("%c", send_buffer[j]);
      }
      printf("\n");

      if( send(server_socket, send_buffer, sizeof send_buffer, 0) == -1 ) 
      {
        diep("client - send()");
      }

      if( recv(server_socket, receive_buffer, sizeof receive_buffer, 0) == -1 ) 
      {
        diep("client - recv()");
      }

      printf("Received - ");
      for( j = 0; j < MAX_BUFFER_SIZE; ++j ) 
      {
        printf("%c", receive_buffer[j]);
      }
      printf("\n");
    }
    printf("C client done\n");
    
    // close connection with server
    close(server_socket);

	return EXIT_SUCCESS;
}

