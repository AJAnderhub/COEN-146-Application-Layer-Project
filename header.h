#define DATA_SIZE 128
#define MAX_FILENAME_LENGTH 32

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

void SendRequest1(int sockfd);
void SendRequest2(int connectSockfd);

void SendRequest3(int sockfd);
void SendRequest4(int connectSockfd, struct Packet firstPacket);

void SendRequest5(int sockfd);
void SendRequest6(int connectSockfd, struct Packet firstPacket);
