//
// Created by Jalter on 10/2/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Response.h"

#define MAX_LINE 4096
#define SERV_PORT 3001



void onLogin();

void onInitScreen();

void onExitScreen();

void displayLoginScreen();

void displayMainMenuScreen();

char strInput[20];
int sock_fd;

int main(int argc, char **argv){

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

    displayLoginScreen();
    return 0;
}

void onInitScreen() {
#ifdef _WIN32
    fflush(stdin);
#else
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Read and discard characters until a newline or EOF is encountered
    }
#endif
    printf("--------------------\n");
}

void onExitScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Exit screen\n");
}

void displayLoginScreen() {
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

void onLogin() {
    char username[20];
    char password[20];
    fflush(stdin);
    printf("- Enter username: ");
    scanf("%s", username);
    printf("- Enter password: ");
    scanf("%s", password);
    // concat LOGIN username and password
    char *loginString = malloc(sizeof(char) * 50);
    strcpy(loginString, "LOGIN ");
    strcat(loginString, username);
    strcat(loginString, "-");
    strcat(loginString, password);
    // send to server
    send(sock_fd, loginString, strlen(loginString), 0);
    // receive response from server
    char *responseString = malloc(sizeof(char) * 50);
    recv(sock_fd, responseString, sizeof(char) * 50, 0);
    // convert response to response object
    Response response = ConvertStringToResponse(responseString);
    // check response
    if (response.type == R_SUCCESS) {
        displayMainMenuScreen();
    } else {
        printf("Login failed\n");
    }
}

void displayMainMenuScreen() {
    long int choice;
    do {
        onInitScreen();
        printf("------------------- Main screen ------------------\n");
        printf("| 1. Read schedule                               |\n");
        printf("| 2. Exit screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        if(fgets(strInput, sizeof(strInput), stdin) != NULL) {
            choice = strtol(strInput, NULL, 10);
        }
        switch (choice) {
            case 1:
                printf("- Enter day of week: ");
                char dayOfWeekStr[20];
                scanf("%s", dayOfWeekStr);
//                if (strcmp(dayOfWeekStr, "All") != 0 && strcmp(dayOfWeekStr, "all") != 0) {
//                    DayOfWeek dayOfWeek;
//                    dayOfWeek = ConvertDayStringToEnum(dayOfWeekStr);
//                    PrintCourseListByDayStudyAndClassIdList(courseSs, dayOfWeek, classIdList);
//                    break;
//                } else PrintCourseListByClassIdList(courseSs, classIdList);
                break;
            case 2:
//                freeDynamicList(classIdList);
//                classIdList = NULL;
                onExitScreen();
#if defined(__linux__) || defined(__unix__)
                printf("Enter anything to return continue\n");
#endif
                break;
            default:
                printf("--> Error: Invalid choice\n");
                break;
        }
    } while (choice != 2);
}

