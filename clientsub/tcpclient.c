// Brittany Harrington (bharrin4)
// Andrew Gnott (agnott)
// Nick Swift (nswift)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <mhash.h>
#include <openssl/md5.h>
#define MAX_LINE 10000

// Example command line:
// make
// ./tcpclient student01.cse.nd.edu 41001 "testfile.txt"

int main(int argc, char * argv[])
{
//	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char filename[20];
	char sendline[MAX_LINE];
	char recline[MAX_LINE];
	int s;
	int len, port_number;
	struct timeval start, end;

	//check for correct number of arguments
	if (argc==4) {	//name and three command line arguments
		host = argv[1];
		port_number = atoi(argv[2]);

		strcpy(filename, argv[3]);
	}
	else {
		fprintf(stderr, "error: too few arguments\n");
		exit(1);
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "error: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(port_number);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("error: socket failure\n");
	//	printf("Welcome to TCP client!\n"); 
		exit(1);
	}

	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{perror("error: connection error\n");
		close(s); 
		exit(1);
	}

	/* get length of file name and add it to filename string-- send 
 	 * the combination to server*/
	short int filename_len = strlen(filename);

	//Send length of file name			
	sprintf(sendline, "%d", filename_len);
	if(send(s, sendline, strlen(sendline), 0)==-1){
		perror("client send error!"); 
		exit(1);
	}
	else printf("Sent length of filename: %s \n", sendline);

	//Clear sendline var
	//bzero((char*)&sendline, sizeof(sendline));
	memset(sendline,0,strlen(sendline));
	printf("Cleared sendline: %s\n", sendline);

	//Send the sendline
	sprintf(sendline, "%s", filename);
	if(send(s, sendline, strlen(sendline), 0)==-1){
		perror("client send error!"); 
		exit(1);
	}
	else printf("Sent filename: %s \n", sendline);

	//Clear sendline var
	//bzero((char*)&sendline, sizeof(sendline));	
	memset(sendline,0,strlen(sendline));
	printf("Cleared sendline: %s\n", sendline);

	//clear the two buffers so they can continue to receive messages
	bzero((char*)&sendline, sizeof(sendline));
 	bzero((char*)&recline, sizeof(recline));
	
	/* receive size of requested file from server. If negative value, error */
		int file_size;
		printf("created file_size variable\n");
		if (len=(recv(s, &file_size, sizeof(file_size), 0)) == -1)	{
			perror("client recieved error");
			exit(1);
		}
		
		file_size=ntohs(file_size);	
		printf("File size: %d\n", file_size);

		if (file_size == 0)	{
			perror("File does not exist on the server\n");
			exit(1);
		}
	/* TODO: keep on receiving MD5 Hash value and stores for later use. */
	unsigned char serverHash[MD5_DIGEST_LENGTH];
	if (recv(s, serverHash, sizeof(serverHash), 0)==-1)	{
		perror("error receiving hash");
		exit(1);
	}
	int i;
	for (i=0; i<MD5_DIGEST_LENGTH; i++)	{	
		printf("%02x", serverHash[i]); 
	}
	printf("\n");

	/* TODO: starts to receive file from server, recording/computing time information. */
	
	/* TODO: after file is received, close connection. */
	close(s);
	
	/* TODO: computes MD5 Hash value based on content received, compare to original MD5 */
	
	/* TODO: if MD5 values don't match, signal error and exit */
	
	/* TODO: if MD5 values match, print matches and additional info (see handout) */

	/* TODO: client exits. */
}

