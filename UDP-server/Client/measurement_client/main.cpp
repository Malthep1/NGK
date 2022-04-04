//============================================================================
// Description : file_client in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

#define BUFSIZE 1000
#define PORT 9000

static int sockfd, n;
static int portno = PORT;
static socklen_t len;	
static struct sockaddr_in serv_addr;
static struct hostent *server;
static char readBuffer[BUFSIZE];
unsigned int second = 1000000;

void receiveFile(char* fileName, int socketfd);

int main(int argc, char *argv[])
{
	printf("Starting Client\n");
	// Checking if enough arguments were passed on run.
	if (argc < 3){
	    printf( "Too few arguments.\n");
		return -1;
	}
	//Opening socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
	    printf("ERROR opening socket\n");
		return -1;
	}
	//Getting host based on IP Passed in ARG 1
	server = gethostbyname(argv[1]);
	if (server == NULL){
	    printf("Can't find host\n");
		return -1;
	}

	printf("Server at: %s, port: %i\n",argv[1], portno);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	sendto(sockfd, (const char *)argv[2], sizeof(argv[2]), 
        			MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
	printf("Request sent!\n");
	n = recvfrom(sockfd,(char *)readBuffer,sizeof(readBuffer),MSG_WAITALL,
		(struct sockaddr *) &serv_addr, &len);
	printf("Data received: %s", readBuffer);
}

