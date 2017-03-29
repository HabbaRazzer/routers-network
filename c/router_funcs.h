/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Helpful Functions (header) 
 */
#ifndef ROUTER_FUNCS.H
#define ROUTER_FUNCS.H

void diep(char* message);
void calc_checksum(unsigned char* message);
bool is_not_corrupt(unsigned char* message);

#endif

