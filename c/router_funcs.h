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

void diep(char* message);
void calc_checksum(unsigned char* message);
bool is_not_corrupt(unsigned char* message);

#endif
