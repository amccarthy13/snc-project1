#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <zconf.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    printf("\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    int n;
    if (argc < 2) {
        fprintf(stderr, "invalid or missing options\n"
                        "usage: snc [-l] [-u] [hostname] port");
        exit(1);
    }
    if (argv[1] == 'l') {
        int protocol;
        if (argv[2] == (char *) 'u') protocol = IPPROTO_UDP; else protocol = IPPROTO_TCP;
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0 )
            error("internal error");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[argc]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
            error("internal error");
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0)
            error("internal error");
        bzero(buffer, 256);
        n = (int) read(newsockfd, buffer, 255);
        if (n < 0) error("internal error");
        printf("Here is the message: %s\n", buffer);
        n = (int) write(newsockfd, "I got your message", 18);
        if (n < 0) error("internal error");
        return 0;
    }

    else {
        char local_buffer[256];
        if (argc < 3) {
            fprintf(stderr, "invalid or missing options\n"
                            "usage: snc [-l] [-u] [hostname] port");
            exit(1);
        }
        portno = atoi(argv[argc]);
        int protocol;
        if (argv[2] == (char *) 'u') protocol = IPPROTO_UDP; else protocol = IPPROTO_TCP;
        sockfd = socket(AF_INET, SOCK_STREAM, protocol);
        if (sockfd < 0)
            error("internal error");
        server = gethostbyname(argv[argc - 1]);
        if (server == NULL) {
            fprintf(stderr,"internal error help me");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy(server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
            error("internal error");
        printf("Please enter the message: ");
        bzero(local_buffer,256);
        fgets(local_buffer,255,stdin);
        n = (int) write(sockfd, local_buffer, strlen(local_buffer));
        if (n < 0)
            error("internal error");
        bzero(local_buffer,256);
        n = (int) read(sockfd, local_buffer, 255);
        if (n < 0)
            error("internal error");
        printf("%s\n",local_buffer);
        return 0;
    }
}