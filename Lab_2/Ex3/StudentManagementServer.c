//
// Created by Jalter on 10/2/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "../../lib/StudentScheduleManagement.h"
#include "../../lib/Path.h"
#include "Request.h"
#include "Response.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LINE 4096
#define SERV_PORT 3001
#define LISTEN_Q 8

DynamicList *studentRs;
DynamicList *courseSs;
DynamicList *classIdList;
User *user;

int main(){
    studentRs = GetStudentRegistrationList(studentRegisterPath);
    courseSs = GetCourseList(courseSchedulePath);
    user = malloc(sizeof(User));
    int listen_fd, conn_fd;
    ssize_t n;
    socklen_t cli_len;
    char buf[MAX_LINE];
    struct sockaddr_in cli_addr, serv_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    int bind_status = bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    printf("bind_status: %d\n", bind_status);
    if (bind_status < 0) {
        perror("Problem in binding the socket");
        exit(2);
    }

    listen(listen_fd, LISTEN_Q);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;) {
        cli_len = sizeof(cli_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
        printf("%s\n", "Received request...");
        Request request;
        Response response;
        while ((n = recv(conn_fd, buf, MAX_LINE, 0)) > 0) {
            if (n > 0) {
                request = ConvertStringToRequest(buf);
                printf("Request: %s\n", buf);
            }
            switch (request.type) {
                case LOGIN:
                    user = UserAuthorized(request.requestString, userAccountPath);
                    if (user != NULL) {
                        response = responseSuccess;
                    } else {
                        response = responseUserAuthorizedFailed;
                    }
                    break;
                case CHECK_SCHEDULE:
                    response = responseSuccess;
                    break;
                default:
                    response = responseInvalidRequest;
                    break;

            }
            char *responseString = ConvertResponseToString(response);

            send(conn_fd, responseString, sizeof(char) * strlen(responseString), 0);
            free(responseString);
        }

        if (n < 0) {
            perror("Read error");
            exit(1);
        }

        close(conn_fd);
        break;
    }
}