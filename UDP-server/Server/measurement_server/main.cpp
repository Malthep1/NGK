//============================================================================
// Description : file_server in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define BUFSIZE 1000
#define PORT 9000

static int sockfd, n;
static int portnum = PORT;
static socklen_t clilen;
static char readBuf[BUFSIZE];
static char writeBuf[BUFSIZE];
struct sockaddr_in serv_addr, cli_addr;
static char * uptimePath = "/proc/uptime";
static char * loadavgPath = "/proc/loadavg";


void sendFile(const char* fileName, long fileSize, int outToClient);


int main(int argc, char *argv[])
{
    printf("Starting UDP Server\n");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
		printf("ERROR opening socket");
        return -1;
    }
    printf("Binding\n");

	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		printf("ERROR on binding");
        return -1;
    }

    for (;;){
		printf("Listening for DGRAMS\n");
		// Read incomming string, extract filename and size, then output these.
		n = recvfrom(sockfd,(char *)readBuf,sizeof(readBuf),MSG_WAITALL, (struct sockaddr *) &cli_addr, &clilen);
		printf("Received: %s\n", readBuf);

		//Read from selected path and send.
		if(readBuf[0] == 'U' || readBuf[0] == 'u'){
			FILE* fp;
			fp = fopen(uptimePath, "rb");
			if(fp != NULL){
				fread(writeBuf,BUFSIZE,1,fp);
				sendto(sockfd, (const char *)writeBuf, sizeof(writeBuf), 
        			MSG_CONFIRM, (const struct sockaddr *) &cli_addr, clilen);
				printf("Uptime Transmitted.\n");
			}
		}
		if(readBuf[0] == 'L' || readBuf[0] == 'l'){
			FILE* fp;
			fp = fopen(loadavgPath, "rb");
			if(fp != NULL){
				fread(writeBuf,BUFSIZE,1,fp);
				sendto(sockfd, (const char *)writeBuf, sizeof(writeBuf), 
        			MSG_CONFIRM, (const struct sockaddr *) &cli_addr, clilen);
				printf("LoadAvg Transmitted.\n");
			}
		}
		bzero(readBuf,sizeof(readBuf));
		bzero(writeBuf,sizeof(writeBuf));
	}

	close(sockfd);
	return 0; 
}