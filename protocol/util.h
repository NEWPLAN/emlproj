#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define NUL                   '\0'

#define NONULL(x) ( x==NULL ? "" : x) /* this is nice, found in the mutt code */

#define TRIM(a) \
{ \
   int len=0; \
   while ((a)[len] && isspace((a)[len])) len++; \
   memmove((a),&(a)[len], strlen(&(a)[len])+1); \
   while ((len=strlen(a))>0 && isspace((a)[len-1])) (a)[len-1]='\0'; \
}
char *substr(const char *string, int start, size_t count);
int stristr(const char *string, const char *pattern);
ssize_t r_read(int fd, void *buf, size_t size);
ssize_t r_write(int fd, void *buf, size_t size);
ssize_t r_SSL_write(SSL *ssl, void *buf, size_t size);
int r_close(int fildes);

char *right(char *string, size_t i);
char *stralloc(size_t length);
char * make_message(const char *fmt, ...);
int r_open2(const char *path, int oflag);

pid_t r_waitpid(pid_t pid, int *stat_loc, int options);
#endif