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
    kill(0, SIGKILL);
}

void handle_kill(int sock, int protocol) { // handle input reads for server
    char buffer[100];
    for (;;) {
        char *line = fgets(buffer, 255, stdin);
        if (line == NULL) {
            if (protocol == IPPROTO_TCP) {
                write(sock, "0x03", 18);
                close(sock);
                printf("internal error");
                kill(0, SIGKILL);
            }
        }
        bzero(buffer, 100);
    }
}

void handle_read_tcp(int sock) { //handle socket reads for client
    char buffer[256];
    for (;;) {
        int n = (int) read(sock, buffer, 255);
        if (strcmp(buffer, "0x03") == 0) {
            close(sock);
            printf("internal error");
            kill(0, SIGKILL);
        }
        if (n < 0)
            error();
        if (n == 0) {
            close(sock);
            printf("internal error");
            kill(0, SIGKILL);
        }
        if (n != 0) {
            printf("%s\n", buffer);
        }
    }
}

void handle_session_tcp(int sock) { //handler multiple server connections
    int n;
    char buffer[256];
    for (;;) {
        bzero(buffer, 256);
        n = read(sock, buffer, 255);
        if (strcmp(buffer, "0x03") == 0) {
            close(sock);
            printf("internal error");
            kill(0, SIGKILL);
        }
        if (n < 0) error();
        if (n == 0) {
            close(sock);
            error();
        }
        if (n != 0) {
            printf("Here is the message: %s\n", buffer);
        }
        n = write(sock, "I got your message", 18);
        if (n < 0) error();
    }
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, pid, kill_pid, read_pid, addr_size;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    int n;
    if (argc < 3 || argc > 5) {
        printf("invalid or missing options\nusage: snc [-l] [-u] [hostname] port\n");
        exit(1);
    }
    int protocol = IPPROTO_TCP;
    int sock_type = SOCK_STREAM;
    int type_flag = 0;
    for (int i = 1; i <= 2; i++) { //read flags
        if (strcmp(argv[i], "-u") == 0) {
            protocol = IPPROTO_UDP;
            sock_type = SOCK_DGRAM;
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
        if (protocol == IPPROTO_TCP) {
            sockfd = socket(AF_INET, sock_type, protocol);
            if (sockfd < 0)
                error();
            bzero((char *) &serv_addr, sizeof(serv_addr));
            portno = atoi(argv[argc - 1]);
            if (portno == 0) {
                error();
            }
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(portno);
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            if (bind(sockfd, (struct sockaddr *) &serv_addr,
                     sizeof(serv_addr)) < 0)
                error();
            clilen = sizeof(cli_addr);
            listen(sockfd, 5);
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
                        handle_session_tcp(newsockfd);
                        exit(0);
                    } else close(newsockfd);
                } else {
                    close(newsockfd);
                }
            }
        } else {
            portno = atoi(argv[argc - 1]);
            if (portno == 0) {
                error();
            }
            char * response = "I got your message";
            char buffer[256];
            struct sockaddr_in serv_addr, cli_addr;

            if ((sockfd = socket(AF_INET, sock_type, 0)) < 0) {
                error();
            }
            memset(&serv_addr, 0, sizeof(serv_addr));
            memset(&cli_addr, 0, sizeof(cli_addr));

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(portno);

            if (bind(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                error();
            }
            kill_pid = fork();
            if (kill_pid == 0) {
                handle_kill(sockfd, protocol);
            }
            int len, n;
            for (;;) {
                n = recvfrom(sockfd, (char *) buffer, 256, MSG_WAITALL, (
                        struct sockaddr *) &cli_addr, (socklen_t *) &len);
                buffer[n] = '\0';
                printf("Here is the message: %s\n", buffer);
            }
        }
    } else { //act as client
        char buffer[256];
        portno = atoi(argv[argc - 1]);
        if (portno == 0) {
            error();
        }
        if (protocol == IPPROTO_TCP) {
            sockfd = socket(AF_INET, sock_type, protocol);
            if (sockfd < 0)
                error();

            server = gethostbyname(argv[argc - 2]);
            if (server == NULL) {
                error();

            }
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.
                    sin_family = AF_INET;
            bcopy(server->h_addr,
                  (char *) &serv_addr.sin_addr.s_addr,
                  server->h_length);
            serv_addr.sin_port = htons(portno);
            if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
                error();

            read_pid = fork();
            if (read_pid == 0) {
                handle_read_tcp(sockfd);
            }
            for (;;) {
                bzero(buffer, 256);
                char *line = fgets(buffer, 255, stdin);
                if (line == NULL) {
                    write(sockfd,
                          "0x03", 18);
                    close(sockfd);
                    printf("internal error");
                    kill(0, SIGKILL);
                }
                n = (int) write(sockfd, buffer, strlen(buffer));
                if (n < 0)
                    error();
                bzero(buffer, 256);
            }
        } else {
            if ((sockfd = socket(AF_INET, sock_type, 0)) < 0) {
                error();
            }

            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(portno);
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            for (;;) {
                memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));
                addr_size = sizeof(serv_addr);
                bzero(buffer, 256);
                char *line = fgets(buffer, 255, stdin);
                if (line == NULL) {
                    for (;;) {}
                }

                int nbytes = strlen(buffer) + 1;
                if (sendto(sockfd, buffer, nbytes, 0, (struct sockaddr *) &serv_addr, addr_size) < 0) {
                    error();
                }
            }
        }
    }
}
