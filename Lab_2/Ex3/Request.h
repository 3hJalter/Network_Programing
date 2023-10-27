//
// Created by Jalter on 10/4/2023.
//

#ifndef NETWORK_PROGRAM_REQUEST_H
#define NETWORK_PROGRAM_REQUEST_H

#include <string.h>
#include <malloc.h>

typedef enum RequestType {
    LOGIN = 0,
    CHECK_SCHEDULE = 1,
    GET_STUDENT_REGISTRATION_LIST = 2,
    LOGOUT = 3,
} RequestType;

typedef struct Request{
    RequestType type;
    char requestString[50];
} Request;

char* ConvertRequestToString(Request request) {
    char *requestString = malloc(sizeof(char) * 50);
    switch (request.type) {
        case LOGIN:
            strcpy(requestString, "LOGIN");
            break;
        case CHECK_SCHEDULE:
            strcpy(requestString, "CHECK_SCHEDULE");
            break;
        default:
            strcpy(requestString, "INVALID");
            break;
    }
    strcat(requestString, " ");
    strcat(requestString, request.requestString);
    return requestString;
}

Request ConvertStringToRequest(char* string) {
    Request request;
    char *token = strtok(string, " ");
    // check if token is null
    if (token == NULL) {
        request.type = -1;
        request.requestString[0] = '\0';
        return request;
    }
    if (strcmp(token, "LOGIN") == 0) {
        request.type = LOGIN;
    } else if (strcmp(token, "CHECK_SCHEDULE") == 0) {
        request.type = CHECK_SCHEDULE;
    } else if (strcmp(token, "GET_STUDENT_REGISTRATION_LIST") == 0) {
        request.type = GET_STUDENT_REGISTRATION_LIST;
    } else if (strcmp(token, "LOGOUT") == 0) {
        request.type = LOGOUT;
    } else {
        request.type = -1;
    }
    token = strtok(NULL, " ");
    strcpy(request.requestString, token);
    return request;
}

#endif //NETWORK_PROGRAM_REQUEST_H
