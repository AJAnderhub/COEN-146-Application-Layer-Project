/*
* Name: TBD
* Title: Final Project Functions 3 and 4
* Description: TODO
*/
#include "header.h"



/*** Download file ***

-Prompt user to input file name they want to download
-Request file by name from server, by sending packet
-Wait for response from server - request type 4
    -If error packet received
        -Display error message “file not found”
    -If successful packet received
        -Write file to client directory with specified name
        -Keep writing incoming packets until len != DATA_SIZE

*/
void SendRequest3(int sockfd){
    // TODO
}

/*** Send Requested File ***

-Search for file with name specified by request
    -If found send file - request type 4
        -File in data field
        -Might need to break file into multiple pieces if too large, just send data in multiple packets with same extra information, length will determine end of file
    -If not found - send error - request type 4
        -Nothing in data field, error = 1

*/
void SendRequest4(int connectSockfd, struct Packet firstPacket){
    // TODO
}
