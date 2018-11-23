/*
* Name: TBD
* Title: Final Project Functions 1 and 2
* Description: TODO
*/
#include "header.h"



/*** Upload file ***

-Prompt user to input file name they want to upload
-Check if file does not exist
    -Error
-While read byte size < sizeof file
    -Send packets
    -Might need to break file into multiple pieces if too large, just send data in multiple packets with same extra information, length will determine end of file
-Wait for response from server - request type 6
-Display success upon acknowledgement or error if filename is already in server

*/
void SendRequest5(int sockfd){
    // TODO
}

/*** Store Received Fiel ***

-Check to see if file name sent firstPacket is already stored in server
-If name is duplicate
    -Receive remaining packets
    -Send error - request type 6
        -Nothing in data field, error = 1
-If name is not a duplicate
    -Receive remaining packets
    -Write file chunks from packet payloads to destination file name
        -Keep writing incoming packets until len != DATA_SIZE
    -Send saved file confirmation - request type 6
        -Nothing in data field, error = 0

*/
void SendRequest6(int connectSockfd, struct Packet firstPacket){
    // TODO
}
