/*
 * Nick Martinez, Robert Windisch, Stephen Clabaugh, Darnell Martin
 * CSC434 - Computer Networks
 * Routing-Network (C Implementation) Helpful Functions (implementation) 
 */

/**
 * Calculates the checksum.
 */
void calc_checksum(unsigned char* message)
{
    unsigned char sum = message[SOURCE_OFFSET] + message[DEST_OFFSET] + message[DATA_OFFSET] + 
        message[DATA_OFFSET+1];
    unsigned char checksum = ~sum;
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

