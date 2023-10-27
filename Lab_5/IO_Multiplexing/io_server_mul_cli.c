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
#define MAX_CLIENTS 10

int main() {
    int server_fd, client_fd, max_fd;
    ssize_t n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
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
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

// Initialize an array of client sockets
    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

// Set the max file descriptor for select
    max_fd = server_fd;

// Loop until the server is terminated
    while (1) {
        // Clear and set the file descriptor set
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        // Add the active client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0) {
                FD_SET(client_sockets[i], &read_fds);
            }
        }

        // Wait for one of the sockets to be ready for reading
        n = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (n == -1) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        // Check if the server socket is ready and accept a new connection
        if (FD_ISSET(server_fd, &read_fds)) {
            len = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &len);
            if (client_fd == -1) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            printf("Accepted a new connection\n");

            // Add the new client socket to the array
            int i;
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = client_fd;
                    break;
                }
            }

            // Check if the array is full
            if (i == MAX_CLIENTS) {
                printf("Reached the maximum number of clients\n");
                close(client_fd);
            } else {
                // Update the max file descriptor if needed
                if (client_fd > max_fd) {
                    max_fd = client_fd;
                }
            }
        }

        // Check which client sockets are ready and forward the messages between them
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0 && FD_ISSET(client_sockets[i], &read_fds)) {
                n = read(client_sockets[i], buffer, MAX_LINE);
                if (n == -1) {
                    perror("read failed");
                    exit(EXIT_FAILURE);
                } else if (n == 0) {
                    printf("Client disconnected\n");
                    close(client_sockets[i]);
                    client_sockets[i] = 0;
                } else {
                    buffer[n] = '\0';
                    printf("Received from client: %s\n", buffer);

                    // Forward the message to all other clients
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_sockets[j] > 0 && j != i) {
                            n = write(client_sockets[j], buffer, n);
                            if (n == -1) {
                                perror("write failed");
                                exit(EXIT_FAILURE);
                            }
                            printf("Forwarded to client: %s\n", buffer);
                        }
                    }
                }
            }
        }
    }
}
