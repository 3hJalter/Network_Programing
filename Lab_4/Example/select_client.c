#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h> 


int main()
{

int n; 
fd_set readfds; 
struct timeval tv; 
char buf1[256], buf2[256];

 struct sockaddr_in server_socket;
  server_socket.sin_family=AF_INET;
  server_socket.sin_port = htons(3000);
  server_socket.sin_addr.s_addr=inet_addr("127.0.0.1");
  printf("server IP = %s", inet_ntoa(server_socket.sin_addr));
 
  int s1 = socket(AF_INET, SOCK_STREAM,0); // client socket 1

  if (connect(s1, (struct sockaddr *) &server_socket, sizeof(server_socket)) <0)
    printf("Error in connecting to server\n");
  else
    printf("connected to server\n");
  //socklen_t client_socket_len = sizeof(client_socket);

 int s2 = socket(AF_INET, SOCK_STREAM,0); // client socket 1

  if (connect(s2, (struct sockaddr *) &server_socket, sizeof(server_socket)) <0)
    printf("Error in connecting to server\n");
  else
    printf("connected to server\n");
  // wait until either socket has data ready to be recv()d (timeout 10.5 secs) 

  tv.tv_sec = 10; 
  tv.tv_usec = 500000; 

  char str1[50], str2[50];
  while (1)
   {
     // clear the set ahead of time 
     FD_ZERO(&readfds); 
     // add our descriptors to the set 
     FD_SET(s1, &readfds);
     FD_SET(s2, &readfds);
 
     // since we got s2 second, it's the "greater", so we use that for the n param in select() 
     n = s2+1; 
     printf("string to s1:"); gets(str1);	
     printf("string to s2:"); gets(str2);	
     send(s1, str1, sizeof(str1),0);
     send(s2, str2, sizeof(str2),0);

     int rv = select(n, &readfds, NULL, NULL, &tv); 

     if (rv == -1) {    
	perror("select"); // error occurred in select() 
     }
     else if (rv == 0) 
	{ 
		printf("Timeout occurred! No data after 10.5 seconds.\n"); 
	} 
	else { 
	  	// one or both of the descriptors have data 
		if (FD_ISSET(s1, &readfds)) { 
			recv(s1, buf1, sizeof buf1, 0); 
			printf("Receive on s1 :%s\n",buf1);
		} 
		if (FD_ISSET(s2, &readfds)) { 
			recv(s2, buf2, sizeof buf2, 0); 
			printf("Receive on s2 :%s\n",buf2);
		} 
	} 
   }
    return 0;
}