#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LINE 4096
#define SERV_PORT 3002
#define LISTEN_Q 8
#define FILE_RECEIVED "/mnt/d/Code/Workspace/C/Network_Program/Lab_2/Ex2/b.jpg"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
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

    FILE *fp;

    for (;;) {
        cli_len = sizeof(cli_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
        printf("%s\n", "Received request...");

        fp = fopen(FILE_RECEIVED, "w");
        if (fp == NULL) {
            perror("Error opening receive file");
            exit(1);
        }

        while ((n = recv(conn_fd, buf, MAX_LINE, 0)) > 0) {
            fwrite(buf, 1, n, fp);
        }

        if (n < 0) {
            perror("Read error");
            exit(1);
        }

        fclose(fp);
        close(conn_fd);
        break;
    }

    close(listen_fd);
    return 0;
}
