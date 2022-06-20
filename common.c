//
// common.c
//
#include "common.h"
#include <stdarg.h>
#include <stdio.h>

char *strmsg(char *fmt, ...) {
    static char msg[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    return msg;
}
