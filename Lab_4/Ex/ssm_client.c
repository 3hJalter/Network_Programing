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

#define MAX_LINE 4096
#define SERV_PORT 1255

char strInput[20];

int client_socket;
struct sockaddr_in server_addr;
char send_buffer[MAX_LINE];
char receive_buffer[MAX_LINE];

void onCreateSocket(char* address);

void onLogin();

void onInitScreen();

void onExitScreen();

void displayLoginScreen();

void displayMainMenuScreen();

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }
    onCreateSocket(argv[1]);
    displayLoginScreen();
    return 0;
}

void onCreateSocket(char* address) {
   // Create a socket
   if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Problem in creating the socket");
      exit(2);
   }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(address);  // Change to the server's IP address if needed

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }
    printf("Connect\n");
}

void onExitScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Exit screen\n");
}

void displayLoginScreen(){
    long int choice;
    do {
        onInitScreen();
        printf("----- Welcome to Student Schedule Management -----\n");
        printf("| 1. Login                                       |\n");
        printf("| 2. Exit Screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        if(fgets(strInput, sizeof(strInput), stdin) != NULL) {
            choice = strtol(strInput, NULL, 10);
        }
        switch (choice) {
            case 1:
                onLogin();
                break;
            case 2:
                onExitScreen();
                break;
            default:
                printf("--> Error: %s is invalid choice\n", strInput);
                break;
        }
    } while (choice != 2);
}

