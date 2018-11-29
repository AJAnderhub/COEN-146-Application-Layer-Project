/*
* Name: Kristina Anderson
* Title: Final Project Functions 3 and 4
* Description: TODO
*/
#include "header.h"
#define SORT_PORT 1719
#define LINESIZE 80
#define NUMWORDS 1



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
    char filename[LINESIZE];
	printf("What file would you like to download?");
	scanf("%s", &filename);
	
	struct sockaddr_in server;
    struct hostent *host_info;
    int count, port_number;
    char inlin[LINESIZE]; /*buffer to copy from user to server*/
    char outline[LINESIZE];/*buffer to copy from server to user*/
	count = 1;
        outline[count]='\0';
        if(write(sockfd, filename,count)<0){
			printf("writing to server failed\n");
		}
		printf("Client: sending filename to server: %s", filename);
    count=read(sockfd,inlin,LINESIZE);
    if (inlin[0] == '0') {
		printf("FILE NOT FOUND");
		return;
	}
	else {
		FILE *fp;
		fp = fopen(filename, "w");
		fwrite(inlin, count, 1, fp);
    }
    while ((count=read(sockfd,inlin,LINESIZE)) >0) {
		fwrite(inlin, count, 1, fp);
        printf("\n");
    }
	fclose(fp);
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
	char recv_data[LINESIZE];	
	ssize_t bytes_read;
	
		bytes_read = recv(sd,&recv_data[0],1024,0); // read data from the socket
        recv_data[bytes_read] ='\0'; // checks for end of data
			if (recv_data[0] == '0') {
				printf("error on the socket");
			}
			else {
				printf("\n(%s , %d) said: ", inet_ntoa(client.sin_addr), ntohs(client.sin_port)); // prints the data
        		printf("%s\n", recv_data); // prints the data
				FILE *fp;
				fp = fopen(recv_data, "r"); // filename
				if (fp == NULL) {
        			filename[0] = '0';
					count = 1;
					printf("FILE NOT FOUND");
					if(write(sockfd, recv_data,count)<0) { // sends back the error - no file
						printf("sending to client failed\n");
					}
				}
				else {
					while (!feof(fp)) {
						fread(recv_data, LINESIZE, 1, fp);
						write(sockfd, recv_data, 1);
					}
				}
			}
        	fflush(stdout); // clear out junk characters on the stdio
}

