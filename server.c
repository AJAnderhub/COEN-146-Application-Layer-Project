/*
* Name: AJ Anderhub
* Title: Final Project Server
* Description: A multimedia server that can, upon user request:
*       * Send a list of files stored on the the server
*       * Send a file that is stored on the the server to the user
*       * Receive and store a file sent by the user
*/

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>


#define LISTEN_PORT 1719
#define NUMCLIENTS 20

void * connectionTask(void *);
void sort (char *[], int n);

int main(int argc,char *argv[]){
    /***
        SETUP AND CONNECTION
    ***/
    struct sockaddr_in server,client;
    int listeningSocket;
    socklen_t client_len;

    // Create a directory named files if not already there
    mkdir("./Files", 0777);

    int connectionSocket[NUMCLIENTS];
    pthread_t thrID[NUMCLIENTS];


    if((listeningSocket=socket(AF_INET,SOCK_STREAM,0))<0)   {
          perror("creating stream socket error!\n");
          exit(1);
    }
    server.sin_family=AF_INET;
    server.sin_port=htons(LISTEN_PORT);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(listeningSocket,(struct sockaddr *)&server,sizeof(server))<0){
          perror("error in binding!");
          exit(1);
    }
    if(listen(listeningSocket,3)<0){
          perror("error in listening!");
          exit(1);
    }

    int connectionCounter = 0;
    while(1){
        client_len = sizeof(client);
        if((connectionSocket[(connectionCounter%NUMCLIENTS)] = accept(
            listeningSocket,(struct sockaddr*)&client,&client_len)) < 0)
        {
             perror("error in binding!");
             exit(1);
        }
        else{
            pthread_create(&thrID[(connectionCounter%NUMCLIENTS)], NULL, connectionTask,
                (void *) &(connectionSocket[(connectionCounter%NUMCLIENTS)]));
            connectionCounter++;
        }
    }
    return (0);
}

// For each individual connection
void *connectionTask(void * assignedSocket){

    int connectionSocket = *((int *) assignedSocket);

    int count = 0;
    struct Packet firstPacket;

    while(1){
        ssize_t bytes_read = recv(connectionSocket, &firstPacket, sizeof(struct Packet), 0);
        // Socket closed
        if(bytes_read == 0){
            return 0;
        }

        if(firstPacket.request_type == 1){
            SendRequest2(connectionSocket);
        }
        else if(firstPacket.request_type == 3){
            SendRequest4(connectionSocket, firstPacket);
        }
        else if(firstPacket.request_type == 5){
            SendRequest6(connectionSocket, firstPacket);
        }
        else{
            printf("\n***Server error***\n");
        }

    }
}
