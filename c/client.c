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

#define PORT 4446
#define BACKLOG 10
#define ROUTER_ADDR "127.0.0.1"

const char *const CLIENT_ADDRS[] = {"B", "C", "D"};

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

        if (status == 0)
        {
            // client has closed the connection
            pthread_exit(NULL);
        }

        // corruption check
        bool not_corrupt = is_not_corrupt(recv_buffer);

        // print the contents of the message
        if (not_corrupt)
        {
            printf("data - %d%d, source - %c", recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET + 1],
                   recv_buffer[SOURCE_OFFSET]);
        }

        if (send(client_socket, recv_buffer, sizeof recv_buffer, 0) == -1)
        {
            diep("client - send() inside handle_message thread");
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
    //int j;

    while (1) // loop
    {

        value = (rand() % 3); // pick random client
        //unsigned char receive_buffer[MAX_BUFFER_SIZE] = {0};
        unsigned char message[MAX_BUFFER_SIZE];
        message[SOURCE_OFFSET] = *ROUTER_ADDR;
        message[DEST_OFFSET] = *CLIENT_ADDRS[value];
        message[DATA_OFFSET] = counter;
        message[DATA_OFFSET + 1] = counter;
        calc_checksum(message); // package message

        printf("Message form c client #1: %c, %c, %d, %d, %d \n", message[SOURCE_OFFSET], message[DEST_OFFSET],
               message[DATA_OFFSET], message[DATA_OFFSET + 1], message[CHECK_OFFSET]); // print data + destination to stdout

        if (send(router_socket, message, sizeof message, 0) == -1)
        {
            diep("router - send()");
        }

        /**if (recv(router_socket, receive_buffer, sizeof receive_buffer, 0) == -1)
        {
            diep("router - recv()");
        }
        printf("Received - ");
        for (j = 0; j < MAX_BUFFER_SIZE; ++j)
        {
            printf("%c", receive_buffer[j]);
        }
        printf("\n");**/

        counter++;
        fflush(stdout);
        sleep(2); // every two seconds - send message to client randomly selected
    }
}

int main(int argc, char *argv[])
{
    int router_socket_out = 0;

    struct sockaddr_in server_info;

    if ((router_socket_out = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("client - socket()");
    }

    memset(&server_info, 0, sizeof(server_info));

    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(ROUTER_ADDR);
    server_info.sin_port = htons(PORT);

    // establish connection with server
    if (connect(router_socket_out, (struct sockaddr *)&server_info, sizeof(server_info)) == -1)
    {
        diep("client - connect()");
    }

    // start the send message loop
    pthread_t thread;
    int status = 0;
    if ((status = pthread_create(&thread, NULL, send_message, (void *)&router_socket_out)) != 0)
    {
        fprintf(stderr, "client - error creating thread! error code = %d\n", status);
        exit(1);
    }
    pthread_join(thread, NULL);

    int router_socket_in = 0;

    struct sockaddr_in client_info;

    // obtain a socket for the server
    if ((router_socket_in = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("client - socket() inside main");
    }

    // bind the server to port
    if (bind(router_socket_in, (struct sockaddr *)&server_info, sizeof server_info) == -1)
    {
        diep("client - bind() inside main");
    }

    // listen for incoming connections
    if (listen(router_socket_in, BACKLOG) == -1)
    {
        diep("client - listen() inside main");
    }

    printf("C client is listening...\n");
    while (1)
    {
        // accept request and create new thread to service that request
        int client_len = sizeof client_info;
        int connection_socket = accept(router_socket_in, (struct sockaddr *)&client_info,
                                       (socklen_t *)&client_len);

        printf("accepted\n");
        pthread_t thread;
        int status = 0;
        if ((status = pthread_create(&thread, NULL, handle_message, (void *)&connection_socket)) != 0)
        {
            fprintf(stderr, "client - error creating thread! error code = %d\n", status);
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}
