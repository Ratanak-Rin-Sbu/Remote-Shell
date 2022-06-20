#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "util.h"

#define SERV_PORT   8000
#define MAXLINE     512

int main(int argc, char **argv) {
    int sfd;
    struct sockaddr_in saddr;

    //TODO: prepare sadder with INADDR_ANY and SERV_PORT
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(SERV_PORT);


    //TODO: make a server socket sfd
    sfd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    int enable = 1;
    ON_FALSE_EXIT(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) >= 0, "cannot set socket option");

    //TODO: bind saddr to the server socket
    int returnV;
    returnV = bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));


    //TODO: listen to the server socket with the backlog size of 1024
    returnV = listen(sfd, 1024);


    while(1) {
        struct sockaddr_in caddr;
        int cfd, clen = sizeof(caddr);

        //TODO: accept a connection to the server socket
        cfd = accept(sfd, (struct sockaddr *)&caddr, &clen);


        printf("[request] from client: %s:%d\n", inet_ntoa(caddr.sin_addr), caddr.sin_port);

        if(fork() == 0/*child*/) {
            //TODO: dupe cfd to the stdin (0), the stdout (1) and the stderr (2)
            dup2(cfd, 0);
            dup2(cfd, 1);
            dup2(cfd, 2);


            //TODO: close cfd and sfd
            close(sfd);
            close(cfd);


            //TODO: execute shell (from the previous assignment)
            //hint: use execvp
            char *args[] = {"./shell", NULL};
            execvp(args[0], args);

        }
        else {
            //TODO: close cfd
            close(cfd);
        }
    }
    return 0;
}
