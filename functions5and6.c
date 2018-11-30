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
#include <unistd.h>

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


	// Display all locally stored files
	printf("Files stored locally: \n---------------------\n");

	struct dirent *de;  // Pointer for directory entry
	DIR *dr = opendir("./Files");

	if (dr == NULL){
		printf("***No files found***\n---------------------\n");
		return;
	}

	char tempFileName[MAX_FILENAME_LENGTH];
	while ((de = readdir(dr)) != NULL){
		for (int i = 0; i < MAX_FILENAME_LENGTH; i++){
			tempFileName[i] = de->d_name[i];
		}


		if((strcmp(".",tempFileName) != 0) && (strcmp("..",tempFileName) != 0) && (strcmp(".DS_Store",tempFileName) != 0)){
			printf("%s\n", tempFileName);
		}
	}
	printf("---------------------\n");



    printf("Enter file to upload to the server: ");

	char source[MAX_FILENAME_LENGTH];
	//fgets(source, MAX_FILENAME_LENGTH, stdin);
	scanf(" %s", source);
	printf("Attemping to send file: %s \n", source);


	FILE *src;
	char fileNamePlusPath[MAX_FILENAME_LENGTH+8];
	strcpy(fileNamePlusPath, "./Files/");
	strcat(fileNamePlusPath, source);

	if((src = fopen(fileNamePlusPath, "rb")) == NULL){
		printf("Error: filename not found\n");
		return;
	}

	strcpy(req5.fileName, source);

	//send contents of file
	ssize_t bytes_read;
	req5.error = 0;
	while (0 < (bytes_read = fread(req5.data, sizeof(char), DATA_SIZE, src))){
		req5.len = bytes_read;
		write(sockfd, &req5, sizeof(struct Packet));
	}


	req5.len = 0;
	write(sockfd, &req5, sizeof(req5));

	//Wait for response
	printf("Waiting for response...\n");
	struct Packet rcv;
	recv(sockfd, &rcv, sizeof(struct Packet), 0);
	if(rcv.error == 1){
		printf("Error: File name already exists.\n");
	}
	else{
		printf("Save Successful\n");
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

	struct Packet req6;	// Packet that will be sent as acknowledgement
	struct Packet rcv;	// Packet that will store incoming data

	req6.request_type = 6;
	char fileNames[MAX_FILENAME_LENGTH];

	struct dirent *de;  // Pointer for directory entry
  	DIR *dr = opendir("./Files");

	//check name against all names in directory
	req6.error = 0;
	while ((de = readdir(dr)) != NULL){
		memset(fileNames, '\0', sizeof(fileNames));
		for (int i = 0; i < MAX_FILENAME_LENGTH; i++){
			fileNames[i] = de->d_name[i];
		}

		// if name already exists receives rest of packets then sends error
		if((strcmp(firstPacket.fileName, fileNames)) == 0){
			req6.error = 1;
			ssize_t bytes_read;
			while((bytes_read = recv(connectSockfd, &rcv, sizeof(struct Packet), 0)) > 0){
				if(rcv.len < DATA_SIZE){
					write(connectSockfd, &req6, sizeof(struct Packet));
					return;
				}
			}
		}
	}

	//no duplicate so save packets while we are still receiving data
	char dest[MAX_FILENAME_LENGTH+8];
	FILE *destFile;

	// Create file name
	strcpy(dest, "./Files/");
	strcat(dest, firstPacket.fileName);

	// Open specified file
	destFile = fopen(dest, "wb");

	// Write first packet received to file
	fwrite(firstPacket.data, firstPacket.len, sizeof(char), destFile);

	// Add to the file until the terminating packet is received
	ssize_t bytes_read;
	while((bytes_read = recv(connectSockfd, &rcv, sizeof(struct Packet), 0))>0){

		//last packet sent will always have len of 0 as implemented in SendRequest5
		if(rcv.len == 0){
			req6.data[0] = '\0';
			write(connectSockfd, &req6, sizeof(req6));
			fclose(destFile);
			return;
		}

		fwrite(rcv.data, rcv.len, sizeof(char), destFile);
	}
}
