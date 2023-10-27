#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 1255
#define MAX_LINE 250

int main() {
    int sock_fd;
    long n;
    struct sockaddr_in serv_addr, from_socket;
    socklen_t addr_len = sizeof(from_socket);
    char send_line[MAX_LINE], recv_line[MAX_LINE + 1];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("creat socket\n");

    while (fgets(send_line, MAX_LINE, stdin) != NULL) {
        sendto(sock_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        n = recvfrom(sock_fd, recv_line, MAX_LINE, 0, (struct sockaddr *) &from_socket, &addr_len);
        recv_line[n] = 0; //null terminate
        printf("%s\n", recv_line);
    }
}