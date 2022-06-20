#include <string.h>

#include "common.h"
#include "parser.h"

#define MATCH(tokens, str, msg) (ON_FALSE_EXIT(match(tokens, str), msg))

//forward declarations
static void parse_pipe(     token_buf_t *tb, cmd_grp_t *cgrp);
static void parse_redir_out(token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd);
static void parse_redir_in( token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd);
static void parse_command(  token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd);
static int  match(token_buf_t *tb, char *str);

void parse_command_group(token_buf_t *tb, cmd_grp_t *cgrp) {
    parse_pipe(tb, cgrp);
}

//parse pipe
static void parse_pipe(token_buf_t *tb, cmd_grp_t *cgrp) {
    //parse the first command
    cgrp->cmds[0] = make_command();
    parse_redir_out(tb, cgrp, cgrp->cmds[0]);

    //parse the piped commands if there are
    int i = 1;
    while(strcmp(tb->peek_token(tb), "|") == 0) {
        MATCH(tb, "|", "unexpected token");
        ON_FALSE_EXIT(i < MAXCMDS, "too many commands");
        cgrp->cmds[i] = make_command();
        parse_redir_out(tb, cgrp, cgrp->cmds[i]);

        command_t *prev = cgrp->cmds[i-1];
        prev->set_pipe(prev, cgrp->cmds[i]);
        i++;
    }
    cgrp->nr_cmd = i;
}

//parse output redirection
static void parse_redir_out(token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd) {
    //parse the first command
    parse_redir_in(tb, cgrp, cmd);

    if(strcmp(tb->peek_token(tb), ">") == 0) {
        MATCH(tb, ">", "unexpected token");
        char *fname = tb->read_token(tb);
        ON_FALSE_EXIT(!IS_OP(fname[0]), "file name is expected");

        cmd->set_redir_out(cmd, fname);
    }
}

//parse input redirection
static void parse_redir_in(token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd) {
    parse_command(tb, cgrp, cmd);

    if(strcmp(tb->peek_token(tb), "<") == 0) {
        MATCH(tb, "<", "unexpected token");
        char *fname = tb->read_token(tb);
        ON_FALSE_EXIT(!IS_OP(fname[0]), "file name is expected");

        cmd->set_redir_in(cmd, fname);
    }
}

//build the argv array
static void parse_command(token_buf_t *tb, cmd_grp_t *cgrp, command_t *cmd) {
    //build the argv array
    int argc = 0;
    while(!IS_OP(tb->peek_token(tb)[0])) {
        ON_FALSE_EXIT(argc < MAXARGS, "too many args");
        cmd->argv[argc++] = tb->read_token(tb);
    }

    //terminate the argv array with NULL
    ON_FALSE_EXIT(argc < MAXARGS, "too many args");
    cmd->argv[argc] = NULL;

    //set the argc
    cmd->argc = argc;
    return;
}

//whether the token matches str
static int match(token_buf_t *tb, char *str) {
    return strcmp(tb->read_token(tb), str) == 0;
}
