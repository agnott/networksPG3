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
#define MAX_PENDING 1	
#define	MAX_LINE 10000
#define MAX_FILE_SIZE 1025
  
struct recvInfo{
	short int filename_len;
	char filename[MAX_LINE];
};
  
int main(int argc, char*argv[]){	
  struct sockaddr_in sin;	
  struct recvInfo info;
  char buf[MAX_LINE];	
  unsigned char fileData[MAX_FILE_SIZE];
  int len;	
  int s, new_s, portNum;
  int opt = 1;

	//check for correct number of arguments
	if (argc!=2)	{
		fprintf(stderr, "error: incorrect number of arguments\n");
	}	else	{
		//set first argument to port number
		portNum=atoi(argv[1]);
	}
  
  /*build  address  data  structure*/
  bzero((char *)&sin, sizeof(sin));	
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr =	INADDR_ANY;
  
  /* Set Server port from command line. */
  sin.sin_port = htons(portNum);	
  
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
		
	info.filename_len = htons((short) atoi(buf));
	printf("Size: %s \n", buf);
	memset(buf,0,strlen(buf));
		
	//Receiving file name
	if((len = recv(new_s, buf, sizeof(buf),0)) == -1){
		perror("Server Received Error!");
		exit(1);
	}
	else printf("New message: %s\n", buf);

	printf("length: %d\n", info.filename_len);
/*	if(info.filename_len != strlen(buf)){
		printf("Error receiving filename");
		exit(1);
	}*/
	strcpy(info.filename, buf);
	printf("Filename: %s \n", buf);
	memset(buf,0,strlen(buf));
	
		
	FILE *fp;
	struct stat st;
	//int filesize;
	
	//set up file path to afs directory
	char path[200];
	bzero(path, 200);
	strcat(path, "/afs/nd.edu/coursefa.15/cse/cse30264.01/files/program3/");
	strcat(path, info.filename);
		
	/* Check if file exists */		
	if(stat(path, &st) != 0){
		printf("File does not exist.\n");
		//filesize=-1;
		//exit(1);
	}
	else{
		printf("File exists.\n");
		//filesize = st.st_size;
	}
	
		
	/* Open File */
	fp=fopen(path, "rt");
	if(fp==NULL){
		printf("Error opening file.\n");
		exit(1);
	}
	else{
		printf("File successfully opened.\n");
	}
	
	/*send filesize back to client. */
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	printf("File size: %d\n", file_size);
	if(send(new_s, &file_size, sizeof(file_size), 0) == -1)	{
		perror("server send error!");
		exit(1);
	}

	/* Determines MD5 hash value of the file. */
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	unsigned char c[MD5_DIGEST_LENGTH];

	rewind(fp);
	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, fp)) != 0)
		MD5_Update (&mdContext, data, bytes);
	MD5_Final (c,&mdContext);
	printf("MD5: ");
	int i;
	for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
	printf ("\n");
	
	/* TODO: Send MD5 hash value of the file back to the client. */
	if (send(new_s, c, sizeof(c), 0) == -1)	{
		perror("Hash not sent successfully...\n");
		exit(1);
	}	
	
	/* Send file contents to client. */	
	rewind(fp);
	while(!feof(fp)){ 
		bzero(fileData, MAX_FILE_SIZE);
		fread(fileData, MAX_FILE_SIZE, 1, fp);

		//Send new portion of file data
		if (send(new_s, fileData, strlen((char *) fileData), 0) == -1)	{
			perror("File data not sent successfully...\n");
			exit(1);
		}	
	}
	
	bzero(fileData, MAX_FILE_SIZE);
/*	strcpy((char *)fileData, "STOP\0");
	printf("File: %s\n", fileData);
	//Send new portion of file data
	if (send(new_s, fileData, strlen((char *) fileData), 0) == -1)	{
		perror("File data not sent successfully...\n");
		exit(1);
	}	
*/
	/* Close file */
	printf("Closing file.\n");	
	fclose(fp);
	
	/* Cleanup */
	printf("Server finishes, close the connection!\n");
	close(new_s);
  }
  return 0;
};
