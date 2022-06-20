#ifndef __SCANNER__
#define __SCANNER__

#define MAXLINE 256
#define EOL     1
#define EOLSTR  "\x1"
#define IS_WS(chr)              ((chr)==' ' || (chr)=='\t' || (chr)=='\n' || (chr)=='\r')
#define IS_OP(chr)              ((chr)==';' || (chr)=='&'|| (chr)=='|' || (chr)=='<' || (chr)=='>' || (chr)==EOL)

typedef struct token_buf {
    char buf[MAXLINE*2];
    char *tokens[MAXLINE];
    int pos;

    char *(*read_token)(struct token_buf *self);
    char *(*peek_token)(struct token_buf *self);
    void  (*destroy)(struct token_buf *self);
} token_buf_t;

extern token_buf_t *tokenize(char *str);

#endif