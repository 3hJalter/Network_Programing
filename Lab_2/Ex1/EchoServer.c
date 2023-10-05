#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LINE 4096 /*max text line length*/
#define SERV_PORT 3001 /*port*/
#define LISTEN_Q 8 /*maximum number of client connections */

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int listen_fd, conn_fd;
    ssize_t n;
//    pid_t child_pid;
    socklen_t cli_len;
    char buf[MAX_LINE];
    struct sockaddr_in cli_addr, serv_addr;

    //creation of the socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
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
        // print the client ip addr
        

        printf("%s\n", "Received request...");

        while ((n = recv(conn_fd, buf, MAX_LINE, 0)) > 0) {
            printf("%s", "String received from and resent to the client:");
            puts(buf);
            send(conn_fd, buf, n, 0);
        }

        if (n < 0) {
            perror("Read error");
            exit(1);
        }
        close(conn_fd);

    }
    //close listening socket
//    close(listen_fd);
}