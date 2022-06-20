#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command_line.h"
#include "scanner.h"

static void prompt() {
    char buf[100];
    char *path = getcwd(buf, sizeof(buf));
    printf("%s> ", path == NULL? "": path);
    fflush(stdout);
}

int main(int argc, char **argv) {
    while(1) {
        //print the prompt
        prompt();
        fflush(stdout);

        //read a command line
        char cmdline[MAXLINE];
        fgets(cmdline, sizeof(cmdline), stdin);
        if(feof(stdin))
            exit(0);

        //execute the command line
        execute_cmd_line(cmdline);
    }
}
