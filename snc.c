#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <zconf.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

void error() {
    perror("internal error");
    exit(1);
}

void handle_kill(int sock, int protocol) { // handle input reads for server
    char buffer[100];
    for (;;) {
        char *line = fgets(buffer, 255, stdin);
        if (line == NULL) {
            close(sock);
            if (protocol == IPPROTO_TCP) {
                write(sock, "0x03", 18);
                kill(0, SIGKILL);
            }
            else {
                kill(0, SIGKILL);
            }
        }
        bzero(buffer, 100);
    }
}

void handle_read(int sock) { //handle socket reads for client
    char buffer[256];
    for (;;) {
        int n = (int) read(sock, buffer, 255);
        if (strcmp(buffer, "0x03") == 0) {
            close(sock);
            kill(0, SIGKILL);
        }
        if (strcmp(buffer, ""))
        if (n < 0)
            error();
        if (n == 0) {
            close(sock);
            kill(0, SIGKILL);
        }
        if (n != 0) {
            printf("%s\n", buffer);
        }
    }
}

void handle_session(int sock) { //handler multiple server connections
    int n;
    char buffer[256];
    for (;;) {
        bzero(buffer, 256);
        n = read(sock, buffer, 255);
        if (strcmp(buffer, "0x03") == 0) {
            close(sock);
            kill(0, SIGKILL);
        }
        if (n < 0) error();
        if (n == 0) {
            close(sock);
            exit(1);
        }
        if (n != 0) {
            printf("Here is the message: %s\n", buffer);
        }
        n = write(sock, "I got your message", 18);
        if (n < 0) error();
    }
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, pid, kill_pid, read_pid;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    int n;
    if (argc < 3 || argc > 5) {
        printf("invalid or missing options\nusage: snc [-l] [-u] [hostname] port\n");
        exit(1);
    }
    int protocol = IPPROTO_TCP;
    int type_flag = 0;
    for (int i = 1; i <= 2; i++) { //read flags
        if (strcmp(argv[i], "-u") == 0) {
            protocol = IPPROTO_UDP;
        }
        if (strcmp(argv[i], "-l") == 0) {
            type_flag = 1;
        }
    }

    int client_flag = 0;
    char *client = "";
    if (type_flag) { //determine if server instance includes hostname
        if ((argc >= 5 && protocol == IPPROTO_UDP) || (argc >= 4 && protocol == IPPROTO_TCP)) {
            client = argv[argc - 2];
            client_flag = 1;
        }
    }

    if (type_flag) { // act as server
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0)
            error();
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[argc - 1]);
        if (portno == 0) {
            error();
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
            error();
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        for (;;) {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
            struct sockaddr_in *pV4Addr = &cli_addr;
            struct in_addr ipAddr = pV4Addr->sin_addr;
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
            if ((client_flag && strcmp(str, client) == 0) || !client_flag) {
                if (newsockfd < 0)
                    error();
                kill_pid = fork();
                if (kill_pid == 0) {
                    handle_kill(newsockfd, protocol);
                }
                pid = fork();
                if (pid < 0)
                    error();
                if (pid == 0) {
                    close(sockfd);
                    handle_session(newsockfd);
                    exit(0);
                } else close(newsockfd);
            } else {
                close(newsockfd);
            }
        }

    } else { //act as client
        char buffer[256];
        portno = atoi(argv[argc - 1]);
        if (portno == 0) {
            error();
        }
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0)
            error();
        server = gethostbyname(argv[argc - 2]);
        if (server == NULL) {
            error();
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy(server->h_addr,
              (char *) &serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            error();
        read_pid = fork();
        if (read_pid == 0) {
            handle_read(sockfd);
        }
        for (;;) {
            bzero(buffer, 256);
            char *line = fgets(buffer, 255, stdin);
            if (line == NULL) {
                close(sockfd);
                if (protocol == IPPROTO_TCP) {
                    write(sockfd, "0x03", 18);
                    kill(0, SIGKILL);
                }
                else {
                    kill(0, SIGKILL);
                }
            }
            n = (int) write(sockfd, buffer, strlen(buffer));
            if (n < 0)
                error();
            bzero(buffer, 256);
        }
    }
}
