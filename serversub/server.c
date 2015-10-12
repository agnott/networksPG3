//Andrew Gnott (agnott)
//Brittany Harrington (bharrin4)
//Nicholas Swift (nswift)

#include<stdio.h>	 
#include<stdlib.h>	  
#include<string.h>	
#include<sys/types.h>	
#include<sys/socket.h>	
#include<netinet/in.h>	
#include<netdb.h>	
#define	SERVER_PORT	41001	
#define MAX_PENDING	1	
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
 	bzero((char	*)&sin, sizeof(sin));	
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr =	INADDR_ANY;
  sin.sin_port = htons(SERVER_PORT);	
  
  /*setup	passive	open*/	
  if((s	= socket(PF_INET,	SOCK_STREAM, 0)) < 0){	
  	perror("simplex-­‐talk:	socket");	
  	exit(1);	
  }	
  
  /*set socket options*/
  if((setsockopt(s,	SOL_SOCKET,	SO_REUSEADDR,	(char	*)&	opt,	sizeof(int)))<0){	
  	perror("simplex-­‐talk:setscokt");	
  	exit(1);	
  }	
  if((bind(s,	(struct sockaddr*)&sin,	sizeof(sin)))	<	0)	{	
  	perror("simplex-­‐talk:	bind");	
  	exit(1);	
  }	
 	if((listen(s,	MAX_PENDING))<0){	
  	perror("simplex-­‐talk:	listen");	
		exit(1);	
 	}	
 	
 	printf("Welcome	to the	first	TCP	Server!\n");	
  
	/*wait for connection, then receive and print text*/	
	while(1){	
  	if((new_s = accept(s,	(struct sockaddr*)&sin,	&len)) < 0){	
 			perror("simplex-­‐talk:	accept");	
  		exit(1);
  	}	
  	
  	//Receiving size of file name
		if((len = recv(new_s, buf, sizeof(buf),0)) == -1){
			perror("Server Received Error!");
			exit(1);
		}
		info.filename_len = (short) atoi(buf);
		printf("Size: %s \n", buf);
		memset(buf,0,strlen(buf));
		
		//Receiving file name
		if((len = recv(new_s, buf, sizeof(buf),0)) == -1){
			perror("Server Received Error!");
			exit(1);
		}
		printf("length: %d\n", info.filename_len);
		if(info.filename_len != strlen(buf)){
			printf("Error receiving filename");
			exit(1);
		}
		strcpy(info.filename, buf);
		printf("Filename: %s \n", buf);
		memset(buf,0,strlen(buf));
		
		
  		//printf("Client finishes, close the connection!\n");
  		//close(new_s);
  }
  return;
};
