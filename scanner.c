#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "scanner.h"

//forward declarations
static char *skip_whitespace(char *str);
static char *read_token(token_buf_t *self);
static char *peek_token(token_buf_t *self);
static void destroy(token_buf_t *self);

//break str into token strings
token_buf_t *tokenize(char *str) {
    token_buf_t *tb = malloc(sizeof(token_buf_t));
    ON_FALSE_EXIT(tb != NULL, "out of memory");

    tb->read_token = read_token;
    tb->peek_token = peek_token;
    tb->destroy    = destroy;

    int count = 0;
    char *buf = tb->buf;
    tb->pos = 0;
    for(str = skip_whitespace(str); *str; ) {
        tb->tokens[count++] = buf;
        if(IS_OP(*str)) {
            *buf++ = *str++;
        }
        else {
            while(*str && !IS_WS(*str) && !IS_OP(*str))
                *buf++ = *str++;
        }
        *buf++ = 0;
        str = skip_whitespace(str);
    }
    tb->tokens[count++] = EOLSTR;
    return tb;
}

static char *skip_whitespace(char *str) {
    while(IS_WS(*str))
        str++;
    return str;
}

//read the token and move to the next token
static char *read_token(token_buf_t *self) {
    return self->tokens[self->pos++];
}

//read the token without moving to the next token
static char *peek_token(token_buf_t *self) {
    return self->tokens[self->pos];
}

static void destroy(token_buf_t *self) {
    free(self);
}
