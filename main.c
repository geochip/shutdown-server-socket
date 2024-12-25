#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int server_socket = -1;

void handler(int signum) {
    printf("got signal %d\n", signum);
    if (shutdown(server_socket, SHUT_RD) < 0) {
        perror("shutdown read");
        exit(EXIT_FAILURE);
    }
    // if (close(server_socket) < 0) {
    //     std::perror("close");
    //     std::exit(EXIT_FAILURE);
    // }
}

int main() {
    struct sigaction action = {};
    action.sa_handler = &handler;
    action.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &action, NULL) != 0)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }


    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {};
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(server_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    r = listen(server_socket, 32);
    if (r < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int client_socket = accept(server_socket, NULL, NULL);

    printf("client_socket=%d\n", client_socket);
    if (client_socket < 0) {
        printf("errno=%d, %s\n", errno, strerror(errno));
    }
}
