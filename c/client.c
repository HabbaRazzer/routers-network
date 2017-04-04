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
#include <stdbool.h>
#include "router_funcs.h"

#define PORT 8080
#define INCOMING_PORT 8000
#define BACKLOG 10
#define ROUTER_ADDR "127.0.1.1"
#define CLIENT "B"

const char *const CLIENT_ADDRS[] = {"B", "B", "B"};

size_t current_value = 1;

void *handle_message(void *socket);
void *send_message(void *socket);

/**
 * Handles any messages sent from a client.
 *
 * params:
 *   socket - the socket interface to the client connection 
 */
void *handle_message(void *socket)
{

    while (1)
    {

        unsigned char recv_buffer[MAX_BUFFER_SIZE] = {0};
        int status;
        int client_socket = *((int *)socket);

        if ((status = recv(client_socket, recv_buffer, sizeof recv_buffer, 0)) == -1)
        {
            diep("client - recv() inside handle_message thread");
        }
        if (recv_buffer[DATA_OFFSET] != 0)
        {
            if (status == 0)
            {
                // roter has closed the connection
                pthread_exit(NULL);
            }

            // corruption check
            bool not_corrupt = is_not_corrupt(recv_buffer);

            // print the contents of the message
            if (not_corrupt)
            {
                printf("Message recieved: %d,%d, source - %c \n", recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET + 1],
                       recv_buffer[SOURCE_OFFSET]);
            }
            else
            {
                printf("Message corrupted.\n");
            }
        }
    }
}

/**
 * Sends a message to client selected at random every two seconds. 
 */
void *send_message(void *socket)
{
    srand((unsigned)time(NULL));
    int counter = 1;
    int value;
    int router_socket = *((int *)socket);

    while (1) // loop
    {

        value = (rand() % 3); // pick random client
        unsigned char message[MAX_BUFFER_SIZE];
        message[SOURCE_OFFSET] = *CLIENT;
        message[DEST_OFFSET] = *CLIENT_ADDRS[value];
        message[DATA_OFFSET] = counter;
        message[DATA_OFFSET + 1] = counter;
        calc_checksum(message); // package message

        printf("Message Sent: %c, %c, %d, %d, %d \n", message[SOURCE_OFFSET], message[DEST_OFFSET],
               message[DATA_OFFSET], message[DATA_OFFSET + 1], message[CHECK_OFFSET]); // print data + destination to stdout

        if (send(router_socket, message, sizeof message, 0) == -1)
        {
            diep("client - send()");
        }

        counter++;
        fflush(stdout);
        sleep(2); // every two seconds - send message to client randomly selected
    }
}

int main(int argc, char *argv[])
{
    //create socket with router
    int router_socket = 0;

    struct sockaddr_in server_info;

    if ((router_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("client - socket()");
    }

    memset(&server_info, 0, sizeof(server_info));

    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(ROUTER_ADDR);
    server_info.sin_port = htons(PORT);

    // establish connection with router
    if (connect(router_socket, (struct sockaddr *)&server_info, sizeof(server_info)) == -1)
    {
        diep("client - connect()");
    }

    // start the send message loop
    pthread_t thread;
    int status = 0;
    if ((status = pthread_create(&thread, NULL, send_message, (void *)&router_socket)) != 0)
    {
        fprintf(stderr, "client - error creating thread! error code = %d\n", status);
        exit(1);
    }
     pthread_join(thread, NULL);

    // start the recieve message
    int client_socket = 0;
    struct sockaddr_in client_info;

    // obtain a socket for the router connection
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("router - socket() in main");
    }

    // set up for the port we want to listen on and other parameters
    memset(&client_info, 0, sizeof client_info);
    client_info.sin_family = AF_INET;
    client_info.sin_addr.s_addr = inet_addr(ROUTER_ADDR);
    client_info.sin_port = htons(INCOMING_PORT);

    // bind the server to client port
    if (bind(client_socket, (struct sockaddr *)&client_info, sizeof client_info) == -1)
    {
        diep("router - bind() in main");
    }

    // listen for incoming connections from client
    if (listen(client_socket, BACKLOG) == -1)
    {
        diep("router - listen() in main");
    }

    printf("client is listening on port %d...\n", INCOMING_PORT);

    while (1)
    {
        // block until we get a connection request from a client, then accept it
        int client_len = sizeof client_info;
        int new_socket = accept(client_socket, (struct sockaddr *)&client_info,
                                (socklen_t *)&client_len);

        // create a new thread to handle the client
        pthread_t thread2;
        int status = 0;
        if ((status = pthread_create(&thread2, NULL, handle_message, (void *)&new_socket)) != 0)
        {
            fprintf(stderr, "router - error creating thread in main! error code = %d\n", status);
            exit(1);
        }
         pthread_join(thread2, NULL);
    }

    return EXIT_SUCCESS;
}
