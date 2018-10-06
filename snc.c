#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <zconf.h>
#include <netdb.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    int n;
    char *type_flag = argv[1];
    int protocol;
    int flag = 0;
    for (int i = 1; i <= 2; i++) {
        if (strcmp(argv[i], "u") == 0) {
            protocol = IPPROTO_UDP;
            flag = 1;
        }
    }
    if (!flag) {
        protocol = IPPROTO_TCP;
    }
    if (argc < 3) {
        fprintf(stderr, "invalid or missing options\n"
                        "usage: snc [-l] [-u] [hostname] port");
        exit(1);
    }
    if ((strcmp("l", type_flag) == 0)) {
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0)
            error("internal error");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[argc - 1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
            error("internal error");
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("internal error");
        bzero(buffer, 256);
        while (1) {
            n = (int) read(newsockfd, buffer, 255);
            if (n < 0) error("internal error");
            printf("Here is the message: %s", buffer);
            n = (int) write(newsockfd, "I got your message", 18);
            if (n < 0) error("internal error");
        }

    } else {
        char buffer[256];
        portno = atoi(argv[argc - 1]);
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0)
            error("internal error");
        server = gethostbyname(argv[argc - 2]);
        if (server == NULL) {
            fprintf(stderr, "internal error\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy(server->h_addr,
              (char *) &serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            error("internal error");
        while (1) {
            printf("Please enter a message: ");
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            n = (int) write(sockfd, buffer, strlen(buffer));
            if (n < 0)
                error("internal error");
            bzero(buffer, 256);
            n = (int) read(sockfd, buffer, 255);
            if (n < 0)
                error("internal error");
            printf("%s\n", buffer);
        }
    }
}
