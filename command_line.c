#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "command_grp.h"

//execute command line
void execute_cmd_line(char *cmdline) {
    //tokenize cmdline
    token_buf_t *tb = tokenize(cmdline);

    for(char *token = ""; token[0] != EOL; ) {
        //make a command group
        cmd_grp_t *cgrp = make_cmd_grp();

        //parse command group
        parse_command_group(tb, cgrp);

        //check the delimiter
        token = tb->read_token(tb);
        ON_FALSE_EXIT(token[0] == EOL || token[0] == ';' || token[0] == '&',
                      "delimiter should be EOL, ;, or &\n");

        //execute the command list
        cgrp->execute(cgrp, token[0] == '&'/*bg_proc*/);

        //destroy command group
        cgrp->destroy(cgrp);
    }

    tb->destroy(tb);
}
