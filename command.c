#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "command.h"

//forward definitions
static void execute_cmd(command_t *self);
static void execute_internal_cmd(command_t *self);
static int  is_internal_cmd(command_t *self);
static void destroy_cmd(command_t *self);

static void set_pipe(command_t *self, command_t *next);
static void set_redir_out(command_t *self, char *fname);
static void set_redir_in(command_t *self, char *fname);

//make a command
command_t *make_command() {
    command_t *cmd = malloc(sizeof(command_t));
    memset(cmd, 0, sizeof(command_t));

    cmd->fdin  = STDIN_FILENO /*0*/;
    cmd->fdout = STDOUT_FILENO/*1*/;
    cmd->pid   = -1;

    cmd->execute          = execute_cmd;    
    cmd->execute_internal = execute_internal_cmd;
    cmd->is_internal      = is_internal_cmd;
    cmd->destroy          = destroy_cmd;

    cmd->set_pipe         = set_pipe;
    cmd->set_redir_out    = set_redir_out;
    cmd->set_redir_in     = set_redir_in;

    return cmd;
}

//execute a command
static void execute_cmd(command_t *self) {
    ON_FALSE_EXIT(self->argc > 0, "empty command");
    //TODO: fork and execute self in the child process
    pid_t pid = fork();

    //child process
    //   TODO: if fdin/fdout are set for the redirection or for the pipe,
    //         dupe them to STDIN_FILENO/STDOUT_FILENO and close them
    //   TODO: execute self
    //   hint: use execvp
    if (pid == 0) {
       if(self->fdin != STDIN_FILENO) {
           dup2(self->fdin, STDIN_FILENO);
           close(self->fdin);
       }
       else if(self->fdout != STDOUT_FILENO) {
           dup2(self->fdout, STDOUT_FILENO);
           close(self->fdout);
       }
       execvp(self->argv[0], self->argv);
    }

    //parent process
    //   TODO: if fdin/fdout are set for the redirection or for the pipe,
    //         close them
    //   TODO: copy the pid of the child process to self->pid
    else {
        if(self->fdin != STDIN_FILENO) {
            close(self->fdin);
        }
        else if(self->fdout != STDOUT_FILENO) {
            close(self->fdout);
        }
        self->pid = pid;
    }
}

//execute an internal command
static void execute_internal_cmd(command_t *self) {
    ON_FALSE_EXIT(self->argc > 0, "empty command");

    if(strcmp(self->argv[0], "exit") == 0) {
        exit(0);
    }
    else if(strcmp(self->argv[0], "cd") == 0) {
        ON_FALSE_EXIT(self->argc <= 2, "too many params");
        char *path = self->argc == 2 ? self->argv[1]: getenv("HOME");
        ON_FALSE_EXIT(path != NULL, "path is NULL");
        ON_FALSE_EXIT(chdir(path) == 0, strmsg("cd to %s failed", path));
    }
}

//whether self is an internal command
static int is_internal_cmd(command_t *self) {
    return  strcmp(self->argv[0], "exit") == 0 ||
            strcmp(self->argv[0], "cd") == 0;
}

//destroy the command
static void destroy_cmd(command_t *self) {
    free(self);
}

//connect prev and curr using pipe
static void set_pipe(command_t *self, command_t *next) {
    //check if self's fdout and next's fdin are already set
    ON_FALSE_EXIT(self->fdout == STDOUT_FILENO, "fdout is already set");
    ON_FALSE_EXIT(next->fdin  == STDIN_FILENO,  "fdin is already set");

    //prepare fdin and fdout of self and next commands
    int fd_pipe[2];  //for pipe
    //TODO: set fd_pipe using pipe
    pipe(fd_pipe);
    //TODO: set fdin and fdout of self and next commands
    self->fdout = fd_pipe[1];
    next->fdin = fd_pipe[0];
}

//set fdout of cmd
static void set_redir_out(command_t *self, char *fname) {
    //check if the self's fdout is already set
    ON_FALSE_EXIT(self->fdout == STDOUT_FILENO, "fdout is already set");

    //TODO: open fname for redirection
    //hint: use mode for the mode
    mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH;
    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, mode);

    //prepare fdout of the command
    //TODO: set fdout of cmd
    self->fdout = fd;
}

//set fdin of cmd
static void set_redir_in(command_t *self, char *fname) {
    //check if the command's fdin is already set
    ON_FALSE_EXIT(self->fdin == STDIN_FILENO, "fdin is already set");

    //TODO: open fname for redirection
    int fd = open(fname, O_RDONLY, 0);

    //prepare fdin of the command
    //TODO: set fdin of cmd
    self->fdin = fd;
}
