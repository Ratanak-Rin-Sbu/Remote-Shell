#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "command.h"

//forward definitions
static void execute_cmd_grp(cmd_grp_t *self, int bg_proc);
static void destroy_cmd_grp(cmd_grp_t *self);
static void destroy_cmds(cmd_grp_t *self);

//make command group
cmd_grp_t *make_cmd_grp() {
    cmd_grp_t *cgrp = malloc(sizeof(cmd_grp_t));
    cgrp->nr_cmd = 0;

    cgrp->execute       = execute_cmd_grp;
    cgrp->destroy       = destroy_cmd_grp;
    cgrp->destroy_cmds  = destroy_cmds;
    return cgrp;
}

//execute a command group
static void execute_cmd_grp(cmd_grp_t *self, int bg_proc) {
    command_t **cmds = self->cmds;
    int nr_cmd = self->nr_cmd;  

    //run commands
    for(int i = 0; i < nr_cmd; i++) {
        //empty command
        if(cmds[i]->argc == 0) 
            continue;

        //TODO: if cmds[i] is an internal command, call its execute_internal
        //      otherwise, call its execute
        if (cmds[i]->is_internal(cmds[i])) {
            cmds[i]->execute_internal(cmds[i]);
        } else {
            cmds[i]->execute(cmds[i]);
        }
    }

    //wait for child processes to finish
    for(int i = 0; i < nr_cmd; i++) {
        //internal or empty commands
        if(cmds[i]->pid == -1)
            continue;

        //TODO: if bg_proc, print the command and the pid
        //                  of the child proc running it
        //      else reap the child process
        if (bg_proc) {
            printf("pid: %d, background processing: %s\n", cmds[i]->pid, cmds[i]->argv[0]);
        } else {
            waitpid(cmds[i]->pid, NULL, 0);
        }
    }

    //TODO: for all terminated background child processes,
    //      reap them and print their pids.
    //hint: use waitpid with WNOHANG option
    if (!bg_proc) {
        return;
    }

    pid_t pid;

    while ((pid == waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("background processing terminated, pid: %d\n", pid);
    }
}

//destroy command group
static void destroy_cmd_grp(cmd_grp_t *self) {
    self->destroy_cmds(self);
    free(self);
}

//destroy parsed commands
static void destroy_cmds(cmd_grp_t *self) {
    for(int i = 0; i < self->nr_cmd; i++) {
        self->cmds[i]->destroy(self->cmds[i]);
    }
}
