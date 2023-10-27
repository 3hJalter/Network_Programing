//
// Created by Jalter on 10/19/2023.
//

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 1255
#define MAX_LINE 255

int main() {
    int sock_fd;
    long n;
    socklen_t len;
    char message[MAX_LINE];
    struct sockaddr_in serv_addr, cli_addr;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0) {
        printf("Server is running at port %d\n", SERV_PORT);
    } else {
        perror("bind failed\n");
        return 0;
    };
    // store all the clients that connected with this server
    struct sockaddr_in clients[10];
    int clients_count = 0;

    for(;;) {
        // receive message from client
        len = sizeof(cli_addr);
        n = recvfrom(sock_fd, message, MAX_LINE, 0, (struct sockaddr *) &cli_addr, &len);
        // check if this client is already connected with this server, if not, add it to the list
        int is_connected = 0;
        for (int i = 0; i < clients_count; i++) {
            if (clients[i].sin_addr.s_addr == cli_addr.sin_addr.s_addr && clients[i].sin_port == cli_addr.sin_port) {
                is_connected = 1;
                break;
            }
        }
        if (!is_connected) {
            clients[clients_count] = cli_addr;
            clients_count++;
        }
        message[n] = 0; // null terminate
        printf("Receive data from client ip: %s, port: %d\n", inet_ntoa(cli_addr.sin_addr), htons(cli_addr.sin_port));
        printf("Message: %s", message);
        if (strcmp(message, "end") == 0) break;
        // send to all other clients that connected with this server
        for (int i = 0; i < clients_count; i++) {
            if (clients[i].sin_addr.s_addr != cli_addr.sin_addr.s_addr || clients[i].sin_port != cli_addr.sin_port) {
                sendto(sock_fd, message, n, 0, (struct sockaddr *) &clients[i], len);
            }
        }
    }
}