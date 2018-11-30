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

	printf("Enter the name of the file that you want to download from the server: ");
	char source[MAX_FILENAME_LENGTH];
	scanf(" %s", source);
	printf("Attemping to retrieve file: %s \n", source);

	// Send packet with file name as request
	strcpy(req3.fileName, source);
	write(sockfd, &req3, sizeof(req3));

	// Wait for response
	printf("Waiting for response...\n");

	// Packet that will hold data received from server
	struct Packet rcv;

	// Receive initial packet
	recv(sockfd, &rcv, sizeof(struct Packet), 0);

	// Server replies with an error, so end function
	if(rcv.error == 1){
		printf("Error: Server could not find a file with that name.\n");
		return;
	}

	// Server found file, so we need to download it
	printf("Downloading file...");
	char fileNamePlusPath[MAX_FILENAME_LENGTH+8];
	strcpy(fileNamePlusPath, "./Files/");
	strcat(fileNamePlusPath, source);

	FILE *fp;
	fp = fopen(fileNamePlusPath, "wb");

	while (rcv.len != 0) {
		fwrite(rcv.data, rcv.len, 1, fp);
		recv(sockfd, &rcv, sizeof(struct Packet), 0);
	}

	fwrite(rcv.data, rcv.len, 1, fp); // Write the last piece of data to the file
	fclose(fp);

	printf("complete.\n");

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
	struct Packet req4;	// Packet(s) that will be sent as data
	req4.request_type = 4;

	FILE *src;
	char fileNamePlusPath[MAX_FILENAME_LENGTH+8];
	strcpy(fileNamePlusPath, "./Files/");
	strcat(fileNamePlusPath, firstPacket.fileName);

	// Attempt to open file specified by name;
	// if file not found, send back error packet
	if((src = fopen(fileNamePlusPath, "rb")) == NULL){
		req4.error = 1;
		ssize_t bytes_read;

		write(connectSockfd, &req4, sizeof(struct Packet));
		return;
	}

	// File was found, so we need to send file
	ssize_t bytes_read;
	req4.error = 0;
	while (0 < (bytes_read = fread(req4.data, sizeof(char), DATA_SIZE, src))){
		req4.len = bytes_read;
		write(connectSockfd, &req4, sizeof(struct Packet));
	}

	req4.len = 0;
	write(connectSockfd, &req4, sizeof(req4));
	fclose(src);
}
