#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LINE 4096
#define SERV_PORT 1255

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char send_buffer[MAX_LINE];
    char receive_buffer[MAX_LINE];

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    // Prepare the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  // Change to the server's IP address if needed

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }
    printf("Connect\n");
    // Send and receive data
    while (1) {
        printf("Enter a message to send to the server (or 'quit' to exit): ");
        fgets(send_buffer, MAX_LINE, stdin);

        // Send the message to the server
        send(client_socket, send_buffer, strlen(send_buffer), 0);

        if (strcmp(send_buffer, "quit\n") == 0) {
            printf("Client is exiting...\n");
            break;
        }

        // Receive and display the server's response
        recv(client_socket, receive_buffer, sizeof(receive_buffer), 0);
        printf("Server response: %s", receive_buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
