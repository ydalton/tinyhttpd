#ifndef DEFS_H_
#define DEFS_H_

#include <sys/socket.h>

#define stdout 1
#define stderr 2

/* syscalls */
extern unsigned int write(int fd, const char *s, unsigned int count);
extern void exit(int code);
extern int close(int fd);

/* functions */
extern unsigned int strlen(const char *s);

#endif
