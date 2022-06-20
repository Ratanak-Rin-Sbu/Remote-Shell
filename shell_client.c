#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "util.h"

#define SERV_ADDR   "127.0.0.1"
#define SERV_PORT   8000

void repeat(int sfd) {
    char buf[MAX_LINE];
    int n;
    fd_set fds;

    while(1) {
        //copy input from stdin to sfd and from sfd to stdout

        //TODO: make fds the empty set
        //hint: use FD_ZERO
        FD_ZERO(&fds);


        //TODO: add 0 and sfd to fds
        //hint: use FD_SET
        FD_SET(0, &fds);
        FD_SET(sfd, &fds);


        //TODO: wait for input in fds
        //hint: use select
        select(sfd+1, &fds, NULL, NULL, NULL);


        //TODO: if there are input in stdin, send it to sfd
        //hint: use FD_ISSET, read, and writen
        if (FD_ISSET(0, &fds)) {
            n = read(0, buf, MAX_LINE);
            writen(sfd, buf, n);
        }

        //TODO: if there are input in sfd, send it to stdout
        //hint: use FD_ISSET, read, and writen
        if (FD_ISSET(sfd, &fds)) {
            n = read(sfd, buf, MAX_LINE);
            writen(1, buf, n);
        }
    }
}

int main(int argc, char **argv) {
    int sfd;
    struct sockaddr_in saddr;

    //TODO: prepare sadder with SERV_ADDR and SERV_PORT
    struct in_addr addr;
    saddr.sin_family = AF_INET;

    if (argc == 2)
    {
        inet_aton(argv[1], &addr);
    }
    else {
        inet_aton(SERV_ADDR, &addr);
    }

    saddr.sin_addr = addr;
    saddr.sin_port = htons(SERV_PORT);


    //TODO: make a socket sfd
    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    //TODO: connect to the server
    connect(sfd, (struct sockaddr *)&saddr, sizeof(saddr));

    printf("server: %s:%d\n", inet_ntoa(saddr.sin_addr), saddr.sin_port);

    repeat(sfd);

    //TODO: close the socket
    close(sfd);


    return 0;
}
