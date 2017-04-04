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

#define CLIENT_PORT 8000
#define ROUTER_PORT 8080
#define BACKLOG 10
#define NUM_ROUTERS 4
#define TABLE_LEN 'A' + NUM_ROUTERS

char *const ROUTING_TABLE[TABLE_LEN] = {[65] = "mct164l12", [66] = "127.0.0.1", [67] = "other2",
    [68] = "other3"};

void *handle_request_t(void *socket);
void route_message(unsigned char *message);
void *handle_client_t(void *socket);

/**
 * Will listen for other routers on port <ROUTER_PORT>.
 */
void *handle_router_t(void *arg)
{
    // listens for other routers on port 8080
    // if gets a connection
        // accepts it
        // passes socket to handle_client_t
        //
    int router_socket = 0;
    struct sockaddr_in router_info;

    // obtain a socket for the client connection
    if ((router_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("router - socket() in handle_router_t");
    }

    // set up for the port we want to listen on and other parameters
    memset(&router_info, 0, sizeof router_info);
    router_info.sin_family = AF_INET;
    router_info.sin_addr.s_addr = htonl(INADDR_ANY);
    router_info.sin_port = htons(ROUTER_PORT);

    // bind the server to client port
    if (bind(router_socket, (struct sockaddr *)&router_info, sizeof router_info) == -1)
    {
        diep("router - bind() in handle_router_t");
    }

    // listen for incoming connections from client
    if (listen(router_socket, BACKLOG) == -1)
    {
        diep("router - listen() in handle_router_t");
    }

    printf("router is listening on port %d...\n", ROUTER_PORT);

    while (1)
    {
        // block until we get a connection request from a client, then accept it
        int router_len = sizeof router_info;
        int new_socket = accept(router_socket, (struct sockaddr *)&router_info,
                                (socklen_t *)&router_len);

        // create a new thread to handle the client
        pthread_t thread;
        int status = 0;
        if ((status = pthread_create(&thread, NULL, handle_client_t, (void *)&new_socket)) != 0)
        {
            fprintf(stderr, "router - error creating thread in handle_router_t! error code = %d\n",
                status);
            exit(1);
        }
        pthread_join(thread, NULL);
    }
}

/**
 * Client request handler.
 *
 * params:
 *   socket - the socket interface to the client connection
 */
void *handle_client_t(void *socket)
{
    // maintain connection with client until client disconnects
    while (1)
    {
        unsigned char recv_buffer[MAX_BUFFER_SIZE] = {0};
        int status;
        int client_socket = *((int *)socket);

        if ((status = recv(client_socket, recv_buffer, sizeof recv_buffer, 0)) == -1)
        {
            diep("router - recv() in handle_request_t");
        }

        if (status == 0)
        {
            // client has closed the connection
            pthread_exit(NULL);
        }

        // check that the message has not been corrupted
        if (is_not_corrupt(recv_buffer))
        {
            // ok - not corrupted. route and print message header + data
            route_message(recv_buffer);
            printf("Source - %c, Destination - %c, Message - %d%d", recv_buffer[SOURCE_OFFSET],
                   recv_buffer[DEST_OFFSET], recv_buffer[DATA_OFFSET], recv_buffer[DATA_OFFSET + 1]);
        }
        else
        {
            printf("Message corrupted.\n");
        }
    }
}

/**
 * Determines where to send a client message and sends it.
 *
 * params:
 *   message - the message to be routed
 */
void route_message(unsigned char *message)
{
    int router_socket = 0;

    struct sockaddr_in router_info;

    if ((router_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("router - socket() in route_message");
    }

    char *destination = ROUTING_TABLE[message[DEST_OFFSET]];
    struct hostent *h;
    h = gethostbyname(destination);
    memset(&router_info, 0, sizeof(router_info));
    router_info.sin_family = AF_INET;
    printf("%s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[0])));
    router_info.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr_list[0])));

    if(strcmp(destination, "127.0.0.1") == 0)
    {
         router_info.sin_port = htons(CLIENT_PORT);
    }else
    {
         router_info.sin_port = htons(ROUTER_PORT);
    }

    // establish connection with neighboring router
    if (connect(router_socket, (struct sockaddr *)&router_info, sizeof(router_info)) == -1)
    {
        diep("router - connect() in route_message");
    }

    // send message
    if (send(router_socket, message, sizeof message, 0) == -1)
    {
        diep("router - send() in route_message");
    }
}

int main(int argc, char *argv[])
{
    pthread_t router_t;
    int status = 0;
    if ((status = pthread_create(&router_t, NULL, handle_client_t, NULL)) != 0)
    {
        fprintf(stderr, "router - error creating thread in main! error code = %d\n", status);
        exit(1);
    }

    int client_socket = 0;
    struct sockaddr_in client_info;

    // obtain a socket for the client connection
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        diep("router - socket() in main");
    }

    // set up for the port we want to listen on and other parameters
    memset(&client_info, 0, sizeof client_info);
    client_info.sin_family = AF_INET;
    client_info.sin_addr.s_addr = htonl(INADDR_ANY);
    client_info.sin_port = htons(CLIENT_PORT);

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

    printf("router is listening on port %d...\n", CLIENT_PORT);

    while (1)
    {
        // block until we get a connection request from a client, then accept it
        int client_len = sizeof client_info;
        int new_socket = accept(client_socket, (struct sockaddr *)&client_info,
                                (socklen_t *)&client_len);

        // create a new thread to handle the client
        pthread_t thread;
        int status = 0;
        if ((status = pthread_create(&thread, NULL, handle_client_t, (void *)&new_socket)) != 0)
        {
            fprintf(stderr, "router - error creating thread in main! error code = %d\n", status);
            exit(1);
        }
        pthread_join(thread, NULL);
    }
    pthread_join(router_t, NULL);

    return EXIT_SUCCESS;
}
