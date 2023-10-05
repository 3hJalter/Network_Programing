#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LINE 4096 /*max text lineHH length*/
#define SERV_PORT 3001 /*port*/

int
main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in serv_addr;
    char send_line[MAX_LINE], recv_line[MAX_LINE];

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    //Create a socket for the client
    //If sock_fd<0 there was an error in the creation of the socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket
    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    while (fgets(send_line, MAX_LINE, stdin) != NULL) {

        send(sock_fd, send_line, strlen(send_line), 0);

        if (recv(sock_fd, recv_line, MAX_LINE, 0) == 0) {
            //error: server terminated prematurely
            perror("The server terminated prematurely");
            exit(4);
        }
        printf("%s", "String received from the server: ");
        fputs(recv_line, stdout);
    }

    exit(0);
}