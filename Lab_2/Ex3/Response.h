//
// Created by Jalter on 10/4/2023.
//

#ifndef NETWORK_PROGRAM_RESPONSE_H
#define NETWORK_PROGRAM_RESPONSE_H

#include <string.h>
#include <malloc.h>
#include <sys/socket.h>

#define MAX_RESPONSE_STRING_SIZE 50
#define MAX_RESPONSE_DATA_SIZE 1000

typedef enum ResponseType {
    R_INVALID = -1,
    R_SUCCESS = 0,
} ResponseType;

typedef struct Response{
    ResponseType type;
    char responseString[MAX_RESPONSE_STRING_SIZE];
    char data[MAX_RESPONSE_DATA_SIZE];
} Response;

Response responseInvalidRequest = {R_INVALID, "Invalid request"};
Response responseSuccess = {R_SUCCESS, "Success"};
Response responseUserAuthorizedFailed = {R_INVALID, "User authorized failed"};
Response responseReadScheduleFailed = {R_INVALID, "Read schedule failed"};


size_t GetResponseSize(){
    return sizeof(Response);
}

void AddResponseData(Response *response, char *data) {
    strcpy(response->data, data);
}

Response CreateResponse(ResponseType type, char *responseString, char *data) {
    Response response;
    response.type = type;
    strcpy(response.responseString, responseString);
    if (data != NULL) {
        AddResponseData(&response, data);
    }
    return response;
}

char* ConvertResponseToString(Response response) {
    char *responseString = malloc(sizeof(char) * 50);
    switch (response.type) {
        case R_SUCCESS:
            strcpy(responseString, "R_SUCCESS");
            break;
        default:
            strcpy(responseString, "R_INVALID");
            break;
    }
    strcat(responseString, "\n");
    strcat(responseString, response.responseString);
    if (strlen(response.data) > 0) {
        strcat(responseString, "\n");
        strcat(responseString, response.data);
    }
    return responseString;
}

void SendResponse(int sock_fd, Response response, char* data) {
    if (strlen(data) < MAX_RESPONSE_DATA_SIZE) {
        AddResponseData(&response, data);
        char *responseString = ConvertResponseToString(response);
        send(sock_fd, responseString, strlen(responseString), 0);
        free(responseString);
    } else {
        // Calculate number of cut data
        unsigned int numberDataCut = strlen(data) / MAX_RESPONSE_DATA_SIZE;
        // loop to send data
        for (int i = 0; i < numberDataCut; ++i) {
            char *dataCut = malloc(sizeof(char) * MAX_RESPONSE_DATA_SIZE);
            memcpy(dataCut, data + i * MAX_RESPONSE_DATA_SIZE, MAX_RESPONSE_DATA_SIZE);
            AddResponseData(&response, dataCut);
            char *responseString = ConvertResponseToString(response);
            send(sock_fd, responseString, strlen(responseString), 0);
            free(responseString);
            free(dataCut);
        }
    }
}

Response ConvertStringToResponse(char* string) {
    Response response;
    char *token = strtok(string, "\n");
    if (strcmp(token, "R_SUCCESS") == 0) {
        response.type = R_SUCCESS;
    } else {
        response.type = R_INVALID;
    }
    token = strtok(NULL, "\n");
    strcpy(response.responseString, token);
    token = strtok(NULL, "\n");
    if (token != NULL) {
        AddResponseData(&response, token);
    }
    return response;
}

#endif //NETWORK_PROGRAM_RESPONSE_H
