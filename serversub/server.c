//Andrew Gnott (agnott)
//Brittany Harrington (bharrin4)
//Nicholas Swift (nswift)

#include<stdio.h>	 
#include<stdlib.h>	  
#include<string.h>
#include<unistd.h>	
#include<sys/stat.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<netinet/in.h>	
#include<netdb.h>	
// http://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
#include<openssl/md5.h>
#define	SERVER_PORT 41001	
#define MAX_PENDING 1	
#define	MAX_LINE 5000
  
struct recvInfo{
	short int filename_len;
	char filename[MAX_LINE];
};
  
int main(){	
  struct sockaddr_in sin;	
  struct recvInfo info;
  char buf[MAX_LINE];	
  int len;	
  int s, new_s;
  int opt = 1;
  
  /*build  address  data  structure*/
  bzero((char *)&sin, sizeof(sin));	
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr =	INADDR_ANY;
  
  /* TODO: Server port from command line. */
  sin.sin_port = htons(SERVER_PORT);	
  
  /*setup passive open*/	
  if((s	= socket(PF_INET, SOCK_STREAM, 0)) < 0){	
  	perror("simplex-­‐talk:	socket");	
  	exit(1);	
  }	
  
  /*set socket options*/
  if((setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)& opt, sizeof(int)))<0){	
  	perror("simplex-­‐talk:setscokt");	
  	exit(1);	
  }	
  if((bind(s, (struct sockaddr*)&sin, sizeof(sin))) < 0){	
  	perror("simplex-­‐talk:	bind");	
  	exit(1);	
  }	
  if((listen(s, MAX_PENDING))<0){	
	perror("simplex-­‐talk: listen");	
	exit(1);	
  }	
 	
  printf("Welcome to the first TCP Server!\n");	
  
	/*wait for connection, then receive and print text*/	
  while(1){
  	if((new_s = accept(s, (struct sockaddr*)&sin, (socklen_t *)&len)) < 0){	
 		perror("simplex-­‐talk:	accept");	
 		exit(1);
 	}
	else printf("New connection.\n");

	// break; } while(1) {  //TEST LINE
  	//Receiving size of file name
	if((len = recv(new_s, buf, sizeof(buf),0)) == -1){
		perror("Server Received Error!");
		exit(1);
	}
	else printf("New message: %s\n", buf);
		
	info.filename_len = (short) atoi(buf);
	printf("Size: %s \n", buf);
	memset(buf,0,strlen(buf));
		
	//Receiving file name
	if((len = recv(new_s, buf, sizeof(buf),0)) == -1){
		perror("Server Received Error!");
		exit(1);
	}
	else printf("New message: %s\n", buf);

	printf("length: %d\n", info.filename_len);
	if(info.filename_len != strlen(buf)){
		printf("Error receiving filename");
		exit(1);
	}
	strcpy(info.filename, buf);
	printf("Filename: %s \n", buf);
	memset(buf,0,strlen(buf));
	
		
	FILE *fp;
	struct stat st;
	//int filesize;
		
	/* Check if file exists */		
	if(stat(info.filename, &st) != 0){
		printf("File does not exist.\n");
		//filesize=-1;
		//exit(1);
	}
	else{
		printf("File exists.\n");
		//filesize = st.st_size;
	}
	
	/* TODO: send filesize back to client. */
		
	/* Open File */
	fp=fopen(info.filename, "rt");
	if(fp==NULL){
		printf("Error opening file.\n");
		exit(1);
	}
	else printf("File successfully opened.\n");
	
	/* Determines MD5 hash value of the file. */
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	unsigned char c[MD5_DIGEST_LENGTH];

	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, fp)) != 0)
		MD5_Update (&mdContext, data, bytes);
	MD5_Final (c,&mdContext);
	printf("MD5: ");
	int i;
	for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
	printf ("\n");
	
	/* TODO: Send MD5 hash value of the file back to the client. */
	
	
	/* TODO: Read file contents and send to client. */		
	
		
	/* Close file */
	printf("Closing file.\n");	
	fclose(fp);
	
	/* Code that might be useful for file i/o slash messaging */
	//fseek(fp, 0, SEEK_END);
	//len=ftell(fp);
	//fseek(fp, 0, SEEK_SET);
	//if (sendline)   {
	//	fread(sendline, 1, len, fp);
	//}

	//printf("%s\n", sendline);

	//if(send(s, sendline, len, 0)==-1){
	//	perror("client send error!"); 
	//	exit(1);
	//}
	//gettimeofday(&start, NULL);

	//recieve back message from server into recline

	//get time of day immediately after receiving message
	//gettimeofday(&end, NULL);
	//print out RTT time
	//printf("RTT: %ld microseconds \n", ((end.tv_sec*1000000+end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
		
	/* Cleanup */
	printf("Server finishes, close the connection!\n");
	close(new_s);
  }
  return 0;
};
