/*
* Name: Kristina Anderson
* Title: Final Project Functions 3 and 4
* Description: TODO
*/
#include "header.h"
#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <sys/socket.h>
#include <dirent.h>
#include <unistd.h>

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
	struct Packet req3;
	req3.request_type = 3;
/*
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
*/
	printf("Enter file to save: \n");
	char source[MAX_FILENAME_LENGTH];
	scanf(" %s", source);
	printf("Attemping to retrieve file: %s \n", source);
/*
	FILE *src;
	char fileNamePlusPath[MAX_FILENAME_LENGTH+8];
	strcpy(fileNamePlusPath, "./Files/");
	strcat(fileNamePlusPath, source);

	if((src = fopen(fileNamePlusPath, "rb")) == NULL){
		printf("Error: filename not found\n");
		return;
	}
*/
	strcpy(req3.fileName, source);
/*
	//send contents of file
	ssize_t bytes_read;
	while (0 < (bytes_read = fread(req3.data, sizeof(char), DATA_SIZE, src))){
		req3.len = bytes_read;
		req3.error = 0;
		write(sockfd, &req3, sizeof(struct Packet));
	}
*/
	write(sockfd, &req3, sizeof(req3));

	//Wait for response
	printf("Waiting for response...\n");
	struct Packet rcv;
	recvfrom (sockfd, &rcv, sizeof(struct Packet), 0, NULL, NULL);
	if(rcv.error == 1){
		printf("Error: File name does not exist.\n");
	}
	else {
		printf("Downloading file");
		FILE *fp;
		fp = fopen(filename, "w");
		while (recv.len != DATA_SIZE) {
			fwrite(rcv.data, recv.len, 1, fp);
        		printf("\n");
			recvfrom (sockfd, &rcv, sizeof(struct Packet), 0, NULL, NULL);
    		}
		fwrite(rcv.data, recv.len, 1, fp); // write the last piece of data to the file
		fclose(fp);	
    	}
	return;
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
	struct Packet req4;	// Packet that will be sent as acknowledgement
	struct Packet rcv;	// Packet that will store incoming data

	req4.request_type = 4;
	char fileNames[MAX_FILENAME_LENGTH];
/*
	struct dirent *de;  // Pointer for directory entry
  	DIR *dr = opendir("./Files");

	//check name against all names in directory
	req4.error = 0;
	while ((de = readdir(dr)) != NULL){
		memset(fileNames, '\0', sizeof(fileNames));
		for (int i = 0; i < MAX_FILENAME_LENGTH; i++){
			fileNames[i] = de->d_name[i];
		}

		// if name already exists receives rest of packets then sends error
		if((strcmp(firstPacket.fileName, fileNames)) == 0){
			req4.error = 1;
			ssize_t bytes_read;
			while((bytes_read = recv(connectSockfd, &rcv, sizeof(struct Packet), 0)) > 0){
				if(rcv.len < DATA_SIZE){
					write(connectSockfd, &req4, sizeof(struct Packet));
					return;
				}
			}
		}
	}
*/
	//no duplicate so save packets while we are still receiving data
	char dest[MAX_FILENAME_LENGTH+8];
	FILE *destFile;
/*
	// Create file name
	strcpy(dest, "./Files/");*/
	
	strcat(dest, firstPacket.fileName);

	// Open specified file
	destFile = fopen(dest, "rb");

	// Write first packet received to file
	if (destFile == NULL) {
		req4.data = 0;
		req4.len = 0;
		printf("FILE NOT FOUND");
		write(connectSockfd, &req4,sizeof(req4));// sends back the error - no file found
	}
	else {
		while (!feof(destFile)) {
			fread(req4.data, LINESIZE, 1, destFile);
			write(connectSockfd, recv_data, 1);
		}
	}
}

