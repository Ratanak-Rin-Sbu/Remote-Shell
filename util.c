#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

//read n bytes
int readn(int fd, char *ptr, int nbytes) {
    int nleft, nread;
    for(nleft = nbytes; nleft > 0; ) {
        nread = read(fd, ptr, nleft);
        if(nread < 0)       //error
            return nread;
        else if(nread == 0) //EOF
            break;
        nleft -= nread;
        ptr   += nread;
    }
    return nbytes - nleft;
}

//read a line
int read_line(int fd, char *ptr, int maxlen) {
    int n, rc;
    char c;
    for(n = 1; n < maxlen; n++) {
        rc = read(fd, &c, 1);
        if(rc == 1) {
            *ptr++ = c;
            if(c == '\n')
                break;
        }
        else if(rc == 0) {
            if(n == 1)
                return 0;   //EOF, no data read
            else
                break;      //EOF, some data was read
        }
        else
            return -1;      //error
    }

    *ptr = 0;
    return n;
}

//write n bytes
int writen(int fd, char *ptr, int nbytes) {
    int nleft, nwritten;
    for(nleft = nbytes; nleft > 0; ) {
        nwritten = write(fd, ptr, nleft);
        if(nwritten < 0)       //error
            return nwritten;

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return nbytes - nleft;
}

//write a string
int write_str(int fd, char *str) {
    writen(fd, str, strlen(str));
}

//write a file
int write_file(int fd, char *fname) {
    int rfd = open(fname, O_RDONLY, 0);
    char buf[MAX_LINE];
    int n;
    while((n = read(rfd, buf, MAX_LINE)) > 0) {
        writen(fd, buf, n);
    }
    close(rfd);
    return 1;
}
