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
#include "iknlib.h"

using namespace std;
static int sockfd, newsockfd, n;
static int portnum = PORT;
static socklen_t clilen;
static char readBuf[BUFSIZE];
static char writeBuf[BUFSIZE];
struct sockaddr_in serv_addr, cli_addr;


void sendFile(const char* fileName, long fileSize, int outToClient);

/**
 * main starter serveren og venter på en forbindelse fra en klient
 * Læser filnavn som kommer fra klienten.
 * Undersøger om filens findes på serveren.
 * Sender filstørrelsen tilbage til klienten (0 = Filens findes ikke)
 * Hvis filen findes sendes den nu til klienten
 *
 * HUSK at lukke forbindelsen til klienten og filen når denne er sendt til klienten
 *
 * @throws IOException
 *
 */
int main(int argc, char *argv[])
{
    printf("Starting Server\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
		error("ERROR opening socket");
        return -1;
    }
    printf("Binding\n");

	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		error("ERROR on binding");
        return -1;
    }

	printf("Listening\n");
	listen(sockfd,5);

    for (;;)
	{
		printf("Waiting for accept\n");
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) error("ERROR on accept");
		else printf("Accepted\n");

		// Read incomming string, extract filename and size, then output these.
		bzero(readBuf,sizeof(readBuf));
		const char* buf = readTextTCP(readBuf, sizeof(readBuf), newsockfd);
		long size = check_File_Exists((char*)buf);
		printf("File Requested: %s, file length: %li \n",buf, size);

		//return file:
		sendFile(buf, size, newsockfd);
		printf("File sent! \n");	
		close(newsockfd);
	}
	close(sockfd);
	return 0; 
}

/**
 * Sender filen som har navnet fileName til klienten
 *
 * @param fileName Filnavn som skal sendes til klienten
 * @param fileSize Størrelsen på filen, 0 hvis den ikke findes
 * @param outToClient Stream som der skrives til socket
     */
void sendFile(const char* fileName, long fileSize, int outToClient)
{
	sprintf(writeBuf, "%li", fileSize);
	writeTextTCP(outToClient,writeBuf);
	bzero(writeBuf,sizeof(writeBuf));
	if(fileSize > 0){
		size_t j = ceil((double)fileSize / 1000);
		FILE* fp;
		fp = fopen(fileName, "rb");
		if(fp != NULL){
			for (size_t i = 0; i < j; i++)
			{
				fread(writeBuf,1,1000,fp);
				writeTextTCP(outToClient,writeBuf);
				printf("Sent bytes: %li to %li \n", i * 1000, (i+1)*1000);
			}
			
		}
	}
	else{
		printf("File is empty or does not exist on server.\n");
	}
	printf("All bytes Transmitted.\n");
}

