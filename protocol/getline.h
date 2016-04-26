#ifndef _GETLINE_H
#define _GETLINE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
/* return values */
#define GETLINE_OK         0  /* ok, there is a line */
#define GETLINE_NOLINE     1  /* there's no complete line*/
#define GETLINE_NEED_READ  2  /* we need to read from int (but select returned < 0 ) */
#define GETLINE_EOF        -1 /* eof */
#define GETLINE_ERR        -2 /* an error occured */
#define GETLINE_TOO_LONG   -3 /* line is too long for buf */
#define GETLINE_PIPE       -4 /* Socket went away */

/* writeline[_*] \r / \r\n control */
#define WRITELINE_LEADING_NONE   0  /* no lading characters */
#define WRITELINE_LEADING_N      1  /* leading \n */
#define WRITELINE_LEADING_RN     2  /* leading \r\n */

/* values of linelen */
#define GETLINE_LINE_INCOMPLETE  -1 /* line is not complete */
#define GETLINE_LINE_NULL        -2 /* line is NULL (not zero-line)  */

typedef struct LineBuffer
{
    /* these values can be touched if you need */
    char *line;      /* where the line starts */
    int linelen;     /* strlen of line, -1 if not complete */
    /* do not touch these values, we need it */
    char *buf;       /* buffer for data */
    int max;         /* malloced size for buf */
    int len;         /* size which is in buffer */
    char *lineend;   /* pointer to end of line (including [\r]\n */
    char *bufend;    /* pointer to last data char in buf */
    int moredata;    /* is 1 when there is more data then line */
} LineBuffer;



int secure_write(int fd, char * buf, int len);
int write_line(int fd, int leading, char *c);
LineBuffer * linebuf_init(int len);
void linebuf_zero(struct LineBuffer *l);
int get_line(int fd, LineBuffer * l);
int select_fd_read(int fd);
void linebuf_uninit(LineBuffer *l);
int writeline_format(int fd, int leading, const char * fmt, ...);
#endif