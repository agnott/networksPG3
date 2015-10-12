#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#define MAX_LINE 10000

int main(int argc, char * argv[])
{
	FILE *fp;
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
		port_number=atoi(argv[2]);

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
		exit(1);
	}

	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{perror("error: connection error\n");
		close(s); 
		exit(1);
	}

	int filename_len=sizeof(filename);
	fp=fopen(filename, "rt");
	fseek(fp, 0, SEEK_END);
	len=ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (sendline)   {
		sprintf(sendline, "%d", filename_len);
		fread(sendline, 1, len, fp);
	}

	printf("%s\n", sendline);

	if(send(s, sendline, len, 0)==-1){
		perror("client send error!"); 
		exit(1);
	}
	gettimeofday(&start, NULL);

	//recieve back message from server into recline
	
	//get time of day immediately after receiving message
	gettimeofday(&end, NULL);
	//print out RTT time
	printf("RTT: %ld microseconds \n", ((end.tv_sec*1000000+end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
	
	//clear the two buffers so they can continue to receive messages
	bzero((char*)&sendline, sizeof(sendline));
        bzero((char*)&recvline, sizeof(recvline));

	close(s);
}

