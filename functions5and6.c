/*
* Name: Hamilton Coke
* Title: Final Project Functions 1 and 2
* Description: TODO
*/
#include "header.h"
#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <sys/socket.h>
#include <dirent.h>


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
	struct Packet req5;
	req5.request_type = 5;
			

    printf("Enter file to save: \n");

	char source[32];
	fgets(source,32,stdin);
	if(FILE *src = fopen(source, "rb")){
		
	}
	FILE *src = fopen(source, "rb");
	req5.fileName = source;
	
	//send contents of file
	while (0< (n = fread(req5.data, sizeof(char), 128, src))){
		req5.len = n;
		req5.error = 0;
		write(sockfd, req5, n);
	}
	req5.len = 0;
	write(sockfd, req5, sizeof(req5));

	//Wait for response
	struct Packet rcv;
	recvfrom (sockfd, &rcv, sizeof(struct Packet), 0, NULL, NULL);
	if(rcv.error == 1){
		printf("Error: File name already exists.\n");
	}
	else{
		printf("Save Successful\n")
	}
}

/*** Store Received File ***

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
    struct Packet req6;
	struct Packet rcv;

	req6.request_type = 6;
	char fileNames[MAX_FILENAME_LENGTH];

	struct dirent *de;  // Pointer for directory entry
  	DIR *dr = opendir("./Files");

	//check name against all names in directory
	while ((de = readdir(dr)) != NULL){
		memset(fileNames, '\0', sizeof(fileNames));
		for (int i = 0; i < MAX_FILENAME_LENGTH; i++){
			fileNames[i] = de->d_name[i];
		
		// if name already exists receives rest of packets then sends error
		if((strcmp(firstPacket.fileName, fileNames)) == 0){
			req6.error == 1;
			while((ssize_t bytes_read = recv(connectionSocket, &rcv, sizeof(struct Packet), 0))>0){
				if(rcv.len == 0){
					write(connectSockfd, req6, sizeof(req6));
					return;
				}
			}		
		}
		req6.error = 0;
	}


	int isFirst = 1;
	char dest[32];
	FILE *destFile;
	int len;

	//no duplicate so save packets while we are still receiving data
	while((ssize_t bytes_read = recv(connectionSocket, &rcv, sizeof(struct Packet), 0))>0){

		//last packet sent will always have len of 0 as implemented in SendRequest5		
		if(rcv.len == 0){
			req6.data = '\0';
			write(connectSockfd, req6, sizeof(req6));
			return;
		}
		
		//checks if file has been created yet so not to overwrite the file
		if(isFirst == 1){
			strcpy(dest, rcv.fileName);		
			destFile = fopen(dest, "wb");
			fwrite(rcv.data, rcv.len, sizeof(char), destFile);
			isFirst = 0;			
		}	
		else{
			destFile = fopen(dest, "a");
			fwrite(rcv.data, rcv.len, sizeof(char), destFile);
			fclose(destFile);
		}


	}		
	
		
}
