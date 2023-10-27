//
// Created by Jalter on 10/26/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define PORT 8082
#define MAX_LINE 1024

// Define a global variable for the socket file descriptor
int client_fd;

// Define a signal handler function for SIGIO
void sigio_handler() {
    // Declare variables for recvfrom()
    char buffer[MAX_LINE];
    struct sockaddr_in server_addr;
    socklen_t len;
    ssize_t n;
    // Call recvfrom() to receive data from the server
    len = sizeof(server_addr);
    n = recvfrom(client_fd, buffer, MAX_LINE, 0, (struct sockaddr *) &server_addr, &len);
    if (n == -1) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    } else if (n == 0) {
        printf("Server disconnected\n");
        close(client_fd);
        exit(EXIT_SUCCESS);
    } else {
        buffer[n] = '\0';
        printf("Received from server: %s\n", buffer);

        // Perform any other actions you want in the signal handler
        // For example, you can send a reply to the server
        n = sendto(client_fd, buffer, n, 0, (struct sockaddr *) &server_addr, len);
        if (n == -1) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }
        printf("Sent to server: %s\n", buffer);
    }
}

int main() {
    struct sockaddr_in server_addr;
    struct sigaction sa;
    // Create a UDP socket
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

// Connect to the server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

// Register the signal handler for SIGIO
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigio_handler;
    sa.sa_flags = SA_RESTART; // Restart any interrupted system calls
    if (sigaction(SIGIO, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

// Set the O_ASYNC flag for the socket file descriptor
    if (fcntl(client_fd, F_SETFL, O_ASYNC) == -1) {
        perror("fcntl failed");
        exit(EXIT_FAILURE);
    }

// Optionally, set the F_SETOWN flag for the socket file descriptor
// This will make the current process receive the SIGIO signal
// Alternatively, you can use F_SETSIG to specify a different signal number
    if (fcntl(client_fd, F_SETOWN, getpid()) == -1) {
        perror("fcntl failed");
        exit(EXIT_FAILURE);
    }

// Loop until the user types "exit" or the server disconnects
    while (1) {
//         Do any other tasks you want in the main loop
//         For example, you can read user input and send it to the server
        char buffer[MAX_LINE];
        ssize_t n;
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