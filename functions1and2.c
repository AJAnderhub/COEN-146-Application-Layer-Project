/*
* Name: Jack Dishman
* Title: Final Project Functions 1 and 2
* Description: Request and send the names of all files in directory "/Files"
*/
#include "header.h"
#include <stdio.h>
#include <sys/socket.h>
#include <dirent.h>
#include <string.h>


/*** Request all filenames ***

-Request names of all files in server, by sending packet type 1
-Wait for response from server - request type 2
-Display received names on stdout

*/
void SendRequest1(int sockfd){

	struct Packet req1;
	req1.request_type=1;
	size_t size = sizeof(req1);
	send(sockfd,&req1, size,0);

	printf("Files on server: \n---------------------\n");

	while(1) {
		recv(sockfd,&req1,sizeof(struct Packet),0);
		if(req1.len == 0){
			printf("---------------------\n");
			return;
		}
		printf("%s\n", req1.fileName);
	}
}

/*** Send all filenames ***

-Send file names - request type 2
    -Data has file names

*/
void SendRequest2(int connectSockfd){

	struct Packet req2;
	req2.request_type = 2;

	struct dirent *de;  // Pointer for directory entry
	DIR *dr = opendir("./Files");

	if (dr == NULL){
		req2.error = 1;
	}
	else{
		req2.error = 0;
	}

	req2.len = 1;
	//read file names in directory
	while ((de = readdir(dr)) != NULL){
		memset(req2.fileName, '\0', sizeof(req2.fileName));
		for (int i = 0; i < MAX_FILENAME_LENGTH; i++){
			req2.fileName[i] = de->d_name[i];
		}

		if((strcmp(".",req2.fileName) != 0) && (strcmp("..",req2.fileName) != 0) && (strcmp(".DS_Store",req2.fileName) != 0)){
			send(connectSockfd,&req2,sizeof(struct Packet),0);
		}
	}

	req2.len = 0;
	send(connectSockfd,&req2,sizeof(struct Packet),0);

	closedir(dr);

}
