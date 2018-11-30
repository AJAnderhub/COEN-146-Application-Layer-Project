/*
* Name: AJ Anderhub
* Title: Final Project Client
* Description: Client for multimedia server that allows the user to:
*       * Request a list of files stored on the the server
*       * Request to download a file that is stored on the the server
*       * Send a file to be stored on the the server
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
#include <sys/stat.h>


#define SERVER_PORT 1719
int main(int argc,char *argv[]){
    /***
        SETUP AND CONNECTION
    ***/
    struct sockaddr_in server;
    struct hostent *host_info;
    int sock, count;
    char *server_name;

    // Create a directory named files if not already there
    mkdir("./Files", 0777);

    /*get server name from command line. if none, use'local host'*/
    server_name =(argc>1) ? argv[1] :"localhost";
    /*create the socket*/
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0) {
        perror("creating stream socket");
        exit(1);
    }
    host_info=gethostbyname(server_name);
    if(host_info==NULL){
        fprintf(stderr,"%s:unknown host: %s\n",argv[0],server_name);
        exit(2);
    }
    /*set up the server's socket address, then connect*/
    server.sin_family=host_info->h_addrtype;
    memcpy((char *)&server.sin_addr,host_info->h_addr,host_info->h_length);
    server.sin_port=htons(SERVER_PORT);
    if(connect(sock,(struct sockaddr *)&server,sizeof (server))<0){
        perror("connecting to server");
        exit(3);
    }

    printf("Connected to server %s\n",server_name);

    /***
        LOOP
    ***/
    while (1) // Infinite loop until 4 command is entered
	{
		int commandInput; // Holds value for command input
		char inputChar; // Temporary holder for input character

		printf("\n(1) List Files Saved on Server\n(2) Download a file from the server\n(3) Save a file to the server\n(4) Quit\nInput your command: ");

		scanf("%d", &commandInput);
		printf("\n");

		switch (commandInput)
		{
			case 1: // List all files from server
                SendRequest1(sock);
                break;

			case 2: // Download a file from the server
                SendRequest3(sock);
                break;

			case 3: // Save a file to the server
                SendRequest5(sock);
				break;

            case 4: // Ends Porgram
                close(sock);
                return 0;

			default: // Gives error if command not recognized
				printf("Command not recognized.\n");
				break;
		}
    }
}
