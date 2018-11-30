#define DATA_SIZE 128
#define MAX_FILENAME_LENGTH 64
#define h_addr h_addr_list[0]

struct Packet{
    int request_type;
    /* 1 to 6
    1 = request file names
    2 = response with file names
    3 = request specific file
    4 = response with specific file in payload
    5 = request file in payload be stored
    6 = response with file saved confirmation
    */
    int len; // [0, DATA_SIZE]
    int error; // 1 = error, 0 = success
    char fileName[MAX_FILENAME_LENGTH];
    char data[DATA_SIZE]; //
};

/*** Client - Request list of files from server ***/
void SendRequest1(int sockfd);
/*** Server - Send list of files to server ***/
void SendRequest2(int connectSockfd);

/*** Client - Request and download a file from server ***/
void SendRequest3(int sockfd);
/*** Server - Send a file to the client ***/
void SendRequest4(int connectSockfd, struct Packet firstPacket);

/*** Client - Send a file to be saved on the server ***/
void SendRequest5(int sockfd);
/*** Server - Receive and save a file sent by the user ***/
void SendRequest6(int connectSockfd, struct Packet firstPacket);
