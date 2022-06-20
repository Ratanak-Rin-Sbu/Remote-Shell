#ifndef __PARSER__
#define __PARSER__

#include "scanner.h"
#include "command_grp.h"

extern void parse_command_group(token_buf_t *tb, cmd_grp_t *cgrp);

#endif
