#ifndef __COMMAND_GRP__
#define __COMMAND_GRP__

#include "command.h"

#define MAXCMDS 100

//command group
typedef struct cmd_grp {
    command_t *cmds[MAXCMDS];
    int nr_cmd;

    void (*execute)(struct cmd_grp *self, int bg_proc);
    void (*destroy)(struct cmd_grp *self);

    void (*destroy_cmds)(struct cmd_grp *self);
} cmd_grp_t;

extern cmd_grp_t *make_cmd_grp();
#endif
