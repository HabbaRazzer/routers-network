/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Helpful Functions (header)
 */
#ifndef ROUTER_FUNCS
#define ROUTER_FUNCS

#define MAX_BUFFER_SIZE 5
#define SOURCE_OFFSET 0
#define DEST_OFFSET 1
#define CHECK_OFFSET 2
#define DATA_OFFSET 3

/**
 * Print the error and quit.
 *
 * params:
 *  message - a string describing error / exception / problem
 */
void diep(char* message);

/**
 * Calculates the checksum.
 */
void calc_checksum(unsigned char* message);

/**
 * Checks the message to see if it is corrupted.
 *
 * return:
 *   true if not corrupted
 */
bool is_not_corrupt(unsigned char* message);

#endif
