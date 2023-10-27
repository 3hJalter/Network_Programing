//
// Created by Jalter on 10/26/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define MAX_LINE 1024

int main(int argc, char **argv) {
    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    int client_fd;
    ssize_t n;
    struct sockaddr_in server_addr;
    char buffer[MAX_LINE];
    fd_set read_fds;
    // Create a TCP socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);;
    server_addr.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    // Loop until the user types "exit" or the server disconnects
    while (1) {
        // Clear and set the file descriptor set
        FD_ZERO(&read_fds);
        FD_SET(client_fd, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        // Wait for either the server or the user input to be ready for reading
        n = select(client_fd + 1, &read_fds, NULL, NULL, NULL);
        if (n == -1) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        // Check if the server is ready and read the message from it
        if (FD_ISSET(client_fd, &read_fds)) {
            n = read(client_fd, buffer, MAX_LINE);
            if (n == -1) {
                perror("read failed");
                exit(EXIT_FAILURE);
            } else if (n == 0) {
                printf("Server disconnected\n");
                close(client_fd);
                return 0;
            } else {
                buffer[n] = '\0';
                printf("Received from server: %s\n", buffer);
            }
        }

        // Check if the user input is ready and send it to the server
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            n = read(STDIN_FILENO, buffer, MAX_LINE);
            if (n == -1) {
                perror("read failed");
                exit(EXIT_FAILURE);
            } else {
                buffer[n] = '\0';
                printf("Sent to server: %s\n", buffer);
                n = write(client_fd, buffer, n);
                if (n == -1) {
                    perror("write failed");
                    exit(EXIT_FAILURE);
                }
                // Check if the user wants to exit
                if (strcmp(buffer, "exit\n") == 0) {
                    printf("Exiting\n");
                    close(client_fd);
                    return 0;
                }
            }
        }
    }
}

