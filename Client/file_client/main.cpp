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
#include "iknlib.h"

using namespace std;

static int sockfd, n;
static int portno = PORT;
static struct sockaddr_in serv_addr;
static struct hostent *server;
static char readBuffer[BUFSIZE];
static char writeBuffer[BUFSIZE];
unsigned int second = 1000000;

void receiveFile(char* fileName, int socketfd);

int main(int argc, char *argv[])
{
	printf("Starting Client\n");
	// Checking if enough arguments were passed on run.
	if (argc < 3){
	    error( "Too few arguments.");
		return -1;
	}
	//Opening socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
	    error("ERROR opening socket");
		return -1;
	}
	//Getting host based on IP Passed in ARG 1
	server = gethostbyname(argv[1]);
	if (server == NULL){
	    error("Can't find host");
		return -1;
	}

	printf("Server at: %s, port: %i\n",argv[1], portno);

	printf("Connecting\n");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	    error("Error connecting");
		return -1;
	}


	//request file from the server
	receiveFile(argv[2], sockfd);
	printf("File received!");
}

/**
 * Modtager filstørrelsen og udskriver meddelelsen: "Filen findes ikke" hvis størrelsen = 0
 * ellers
 * Åbnes filen for at skrive de bytes som senere modtages fra serveren (HUSK kun selve filnavn)
 * Modtag og gem filen i blokke af 1000 bytes indtil alle bytes er modtaget.
 * Luk filen, samt input output streams
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn
 * @param sockfd Stream for at skrive til/læse fra serveren
 */
void receiveFile(char* fileName, int sockfd)
{
	writeTextTCP(sockfd,fileName);
	printf("Sent!\n");
	long filesize = getFileSizeTCP(sockfd);
	printf("Filesize: %li\n",filesize);
	const char* extractedFilename = extractFileName(fileName);
	if(filesize > 0){
		size_t j = ceil((double)filesize / 1000);
		FILE* fp;
		fp = fopen(extractedFilename, "wb");
		if(fp != NULL){
			for (size_t i = 0; i < j; i++)
			{
				const char* buf = readTextTCP(readBuffer, sizeof(readBuffer), sockfd);
				printf("Received: %s", (void *)buf);
				fwrite((void *)buf, 1,sizeof(buf),fp);
				printf("Wrote bytes: %li to %li, into %s \n", i * 1000, (i+1)*1000, extractedFilename);
			}
			
		}
	}
	else{
		printf("File is empty or does not exist on server.");
	}
}

