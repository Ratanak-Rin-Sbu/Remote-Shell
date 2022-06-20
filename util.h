#ifndef __UTIL__
#define __UTIL__

#define MAX_LINE    512

extern int readn(int fd, char *ptr, int nbytes);
extern int read_line(int fd, char *ptr, int maxlen);
extern int writen(int fd, char *ptr, int nbytes);
extern int write_str(int fd, char *str);
extern int write_file(int fd, char *fname);

#endif
