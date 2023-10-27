//
// Created by Jalter on 10/26/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8082
#define MAX_LINE 1024

int main() {
    int server_fd, client_fd[2], max_fd;
    ssize_t n;
    struct sockaddr_in server_addr, client_addr[2];
    socklen_t len[2];
    char buffer[MAX_LINE];
    fd_set read_fds;
    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

// Bind the socket to the port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

// Listen for incoming connections
    if (listen(server_fd, 2) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

// Accept two clients
    for (int i = 0; i < 2; i++) {
        len[i] = sizeof(client_addr[i]);
        client_fd[i] = accept(server_fd, (struct sockaddr *) &client_addr[i], &len[i]);
        if (client_fd[i] == -1) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Accepted client %d\n", i + 1);
    }

// Close the server socket
    close(server_fd);

// Set the max file descriptor for select
    max_fd = (client_fd[0] > client_fd[1]) ? client_fd[0] : client_fd[1];

// Loop until both clients disconnect
    while (1) {
        // Clear and set the file descriptor set
        FD_ZERO(&read_fds);
        FD_SET(client_fd[0], &read_fds);
        FD_SET(client_fd[1], &read_fds);

        // Wait for one of the clients to be ready for reading
        n = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (n == -1) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        // Check which client is ready and forward the message to the other one
        for (int i = 0; i < 2; i++) {
            if (FD_ISSET(client_fd[i], &read_fds)) {
                n = read(client_fd[i], buffer, MAX_LINE);
                if (n == -1) {
                    perror("read failed");
                    exit(EXIT_FAILURE);
                } else if (n == 0) {
                    printf("Client %d disconnected\n", i + 1);
                    close(client_fd[i]);
                    return 0;
                } else {
                    buffer[n] = '\0';
                    printf("Received from client %d: %s\n", i + 1, buffer);
                    n = write(client_fd[1 - i], buffer, n);
                    if (n == -1) {
                        perror("write failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("Forwarded to client %d: %s\n", 2 - i, buffer);
                }
            }
        }
    }
}
