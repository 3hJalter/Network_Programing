//
// Created by Jalter on 10/2/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "Response.h"
#include "../../lib/StudentScheduleManagement.h"

#define MAX_LINE 4096
#define SERV_PORT 3001

DynamicList *uniStudentRsList;
User *user;

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

char* CreateLoginRequest(char* username, char* password){
    char *loginString = malloc(sizeof(char) * 50);
    strcpy(loginString, "LOGIN ");
    strcat(loginString, username);
    strcat(loginString, "-");
    strcat(loginString, password);
    return loginString;
}

char * CreateGetStudentRegistrationListRequest(char * username) {
    char *requestString = malloc(sizeof(char) * 50);
    strcpy(requestString, "GET_STUDENT_REGISTRATION_LIST ");
    strcat(requestString, username);
    return requestString;
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
    char *loginString = CreateLoginRequest(username, password);
    send(sock_fd, loginString, strlen(loginString), 0);
    // receive response from server
    char *responseString = malloc(GetResponseSize());
    recv(sock_fd, responseString, GetResponseSize(), 0);
    // convert response to response object
    Response response = ConvertStringToResponse(responseString);
    // check response
    if (response.type == R_SUCCESS) {
        // convert the data to user
        user = ConvertStringToUser(response.data);
        char *requestString = CreateGetStudentRegistrationListRequest(user->username);
        send(sock_fd, requestString, strlen(requestString), 0);
        // clear response string
        memset(responseString, 0, GetResponseSize());
        // receive response from server
        recv(sock_fd, responseString, GetResponseSize(), 0);
        // convert response to response object
        response = ConvertStringToResponse(responseString);
        if (response.type != R_SUCCESS) {
            printf("Get student registration list failed\n");
            return;
        }
        // convert the data to student registration list
        uniStudentRsList = ConvertJsonToStudentRegistrationList(response.data);
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
                char *requestString = malloc(sizeof(char) * 50);
                strcpy(requestString, "CHECK_SCHEDULE ");
                strcat(requestString, dayOfWeekStr);
                // send to server
                send(sock_fd, requestString, strlen(requestString), 0);
                // receive response from server
                char *responseString = malloc(GetResponseSize());
                recv(sock_fd, responseString, GetResponseSize(), 0);
                // print response to test
                Response response = ConvertStringToResponse(responseString);
                DynamicList *courseSs = ConvertJsonToCourseList(response.data);
//                if (strcmp(dayOfWeekStr, "All") != 0 && strcmp(dayOfWeekStr, "all") != 0) {
//                    DayOfWeek dayOfWeek;
//                    dayOfWeek = ConvertDayStringToEnum(dayOfWeekStr);
//                    PrintCourseListByDayStudyAndStudentRegistrationList(courseSs, dayOfWeek, uniStudentRsList);
//                    break;
//                } else PrintCourseListByStudentRegistrationList(courseSs, uniStudentRsList);
                free(requestString);
                freeCourseList(courseSs);
                requestString = NULL;
                break;
            case 2:
                freeUser(user);
//                freeDynamicList(uniStudentRsList);
//                uniStudentRsList = NULL;
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

