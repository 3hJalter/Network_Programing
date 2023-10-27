#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define MAX_LINE 4096 /*max text line length*/
#define SERV_PORT 1255 /*port*/
#define LISTEN_Q 8 /*maximum number of client connections*/

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int listen_fd, conn_fd;
    size_t n;
    pid_t child_pid;
    socklen_t cli_len;
    char buf[MAX_LINE];
    struct sockaddr_in cli_addr, serv_addr;

    //Create a socket for the soc_let
    //If sock_fd<0 there was an error in the creation of the socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }


    //preparation of the socket address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    //bind the socket
    bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen(listen_fd, LISTEN_Q);
    // print address and port of the server
    printf("Server address: %s\n", inet_ntoa(serv_addr.sin_addr));
    printf("Server port: %d\n", htons(serv_addr.sin_port));
    printf("%s\n", "Server running...waiting for connections.");



    for (;;) {

        cli_len = sizeof(cli_addr);
        //accept a connection
        conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);

        printf("%s\n", "Received request...");

        if ((child_pid = fork()) == 0) {//if it’s 0, it’s child process

            printf("Child pid %d created for dealing with client requests\n", getpid());

            //close listening socket
            close(listen_fd);
            while ((n = recvfrom(conn_fd, buf, MAX_LINE, 0, (struct sockaddr *) &cli_addr, &cli_len)) > 0) {
                printf("%s", "String received from and resent to the client:");
                puts(buf);
                send(conn_fd, buf, n, 0);
            }

//            while ((n = recv(conn_fd, buf, MAX_LINE, 0)) > 0) {
//                printf("%s", "String received from and resent to the client:");
//                puts(buf);
//                send(conn_fd, buf, n, 0);
//            }

            if (n <= 0)
                printf("The client id %s close or error on reading\n", inet_ntoa(cli_addr.sin_addr));
            exit(0);
        }
        signal(SIGCHLD, SIG_IGN);
        //close socket of the server
        close(conn_fd);
    }
}