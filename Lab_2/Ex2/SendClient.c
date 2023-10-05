#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LINE 4096
#define SERV_PORT 3002
#define FILE_SENT "/mnt/d/Code/Workspace/C/Network_Program/Lab_2/Ex2/a.jpg"

int main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server>");
        exit(1);
    }

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(SERV_PORT);

    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    FILE *fp = fopen(FILE_SENT, "r");
    if (fp == NULL) {
        perror("Not found send file");
        exit(1);
    }

    char buf[MAX_LINE];
    size_t bytesRead;

    while ((bytesRead = fread(buf, 1, sizeof(buf), fp)) > 0) {
        send(sock_fd, buf, bytesRead, 0);
    }

    fclose(fp);
    close(sock_fd);

    exit(0);
}
