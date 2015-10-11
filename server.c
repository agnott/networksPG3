#include<stdio.h>	 
#include<stdlib.h>	  
#include<string.h>	
#include<sys/types.h>	
#include<sys/socket.h>	
#include<nePnet/in.h>	
#include<
netdb.h
>	
  
#define	
  SERVER_PORT	
  41700	
  
#define	
  MAX_PENDING	
  5	
  
#define	
  MAX_LINE	
  256	
  
int
main()	
  
{	
  
struct
sockaddr_in
	
  sin;	
  
char	
  
buf
[MAX_LINE];	
  
int
len
;	
  
int
	
  s,	
  
new_s
;
