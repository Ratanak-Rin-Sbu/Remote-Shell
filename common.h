//
// common.h
//
#ifndef __COMMON__
#define __COMMON__
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ON_FALSE_GOTO(exp, label, msg) {\
    if(!(exp)) {\
        char *str = (char*)msg;\
        if(str && str[0] != '\0')\
            fprintf(stderr, "%s in file: %s, function: %s, line: %d\n%s\n",\
                str, __FILE__, __FUNCTION__, __LINE__, strerror(errno));\
        goto label;\
    }\
}

#define ON_FALSE_EXIT(exp, msg) {\
    if(!(exp)) {\
        char *str = (char*)msg;\
        if(str && str[0] != '\0')\
            fprintf(stderr, "%s in file: %s, function: %s, line: %d\n%s\n",\
                str, __FILE__, __FUNCTION__, __LINE__, strerror(errno));\
        exit(1);\
    }\
}

extern char *strmsg(char *fmt, ...);

#endif
