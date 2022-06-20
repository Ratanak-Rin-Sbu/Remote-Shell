#ifndef __COMMAND__
#define __COMMAND__

#include <unistd.h>

#define MAXARGS 16

typedef struct command {
    //the command and its arguments
    char *argv[MAXARGS];    //command and arguments
    int argc;               //# of valid argv

    //file descriptors for stdin and stdout
    int fdin;               //file descriptor for input
    int fdout;              //file descriptor for output

    //pid of the process that runs this command
    pid_t pid;              

    void (*execute)(struct command *self);    
    void (*execute_internal)(struct command *self);
    int  (*is_internal)(struct command *self);
    void (*destroy)(struct command *self);

    void (*set_pipe)(struct command *self, struct command *next);
    void (*set_redir_out)(struct command *self, char *fname);
    void (*set_redir_in)(struct command *self, char *fname);
} command_t;

extern command_t *make_command();

#endif
