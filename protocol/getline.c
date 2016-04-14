
#include "getline.h"

int writeline_format(int fd, int leading, const char * fmt, ...)
{
    char out[4096];
    int len, res;
    va_list az;
    if (!fmt) return GETLINE_ERR;
    va_start(az, fmt);
    len = vsnprintf(out, 4090, fmt, az);
    switch (leading)
    {
    case WRITELINE_LEADING_NONE:
        break;
    case WRITELINE_LEADING_N:
        out[len] = '\n';
        len++;
        break;
    case WRITELINE_LEADING_RN:
    default:
        out[len] = '\r';
        len++;
        out[len] = '\n';
        len++;
        break;
    }
    res = secure_write(fd, out, len);
    return res; /* bytes / GETLINE_ERR */
}

int secure_write(int fd, char * buf, int len)
{
    /* writes len bytes from buf to fd */
    /* repeats if fd can't send len at once */
    /* returns error or number of bytes written */
    int written = 0;
    int i;
    if (len == 0) return GETLINE_OK; /* that's ok */
    if (len < 1)  return GETLINE_ERR; /* grmpf, that's not */
    while ((i = r_write(fd, &buf[written], len - written)) < len)
    {
        /* Socket went away */
        if (i == -1 && errno == EPIPE) return GETLINE_PIPE;
        /* Other errors */
        if (i < 1) return GETLINE_ERR;
        written += i;
        len -= i;
    }
    return written;
}

int write_line(int fd, int leading, char *c)
{
    char *out;
    int len, res;

    if (!c) return GETLINE_ERR;
    if ((out = malloc(strlen(c) + 3)) == NULL) return GETLINE_ERR;
    switch (leading)
    {
    case WRITELINE_LEADING_NONE:
        len = sprintf(out, "%s", c);
        break;
    case WRITELINE_LEADING_N:
        len = sprintf(out, "%s\n", c);
        break;
    case WRITELINE_LEADING_RN:
    default:
        len = sprintf(out, "%s\r\n", c);
        break;
    }
    res = secure_write(fd, out, len);
    free(out);
    return res; /* bytes / GETLINE_ERR / GETLINE_PIPE */
}


void linebuf_zero(struct LineBuffer *l)
{
    l->lineend = NULL;
    l->bufend = NULL;
    l->len = 0;
    l->linelen = GETLINE_LINE_NULL;
    l->line = NULL;
}

LineBuffer * linebuf_init(int len)
{
    LineBuffer * l;
    l = malloc(sizeof(LineBuffer));
    if (!l) return NULL;
    if ((l->buf = malloc(len)) == NULL) return NULL;
    l->max = len;
    linebuf_zero(l);
    return l;
}

int select_fd_read(int fd)
{
    /* calls select (on read) for a given fd) */
    fd_set fds;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    return select(fd + 1, &fds, NULL, NULL, &timeout);
}

int get_line(int fd, LineBuffer * l)
{
    int len = 0, last_r, maxread;
    char *p;
    /* is there one more line in buf? */
    if (l->lineend == l->bufend)
    {
        /* no --> read some chars */
        maxread = l->max - 1;
        if (maxread < 1) return GETLINE_TOO_LONG;
        if (select_fd_read(fd) < 1) return GETLINE_NEED_READ;
        len = r_read(fd, l->buf, maxread );
        if (len < 1) return GETLINE_EOF;
        l->len = len;
        l->line = l->buf;
        l->bufend = &(l->buf[len - 1]);
    }
    else
    {
        /* move next line (from which we have already some data) to beginning of buf */
        if (l->lineend && l->bufend)
        {
            len = l->bufend - l->lineend;
            memmove(l->buf, l->lineend + 1, len );
            l->line = l->buf;
            l->bufend = l->buf + len - 1 ;
            l->len = len;
        }
        else
        {
            /* there is a not yet finished line in buf */
            maxread = l->max - l->len - 1;
            if (maxread < 1) /* no more space left in buf */
            {
                if (l->len == l->max)
                {
                    l->linelen = l->max - 1;
                    //l->buf[l->max-1]='\0';
                }
                else
                {
                    l->linelen = l->len - 1;
                    //l->buf[len]='\0';
                }
                l->line = l->buf;
                /* set to zero, then next call will work */
                l->len = 0;
                l->lineend = NULL;
                l->bufend = NULL;
                return GETLINE_TOO_LONG;
            }
            if (select_fd_read(fd) < 1) return GETLINE_NEED_READ;
            len = r_read(fd, &(l->buf[l->len]), maxread );
            if (len < 1)  /* EOF */
            {
                /* TODO: what's up if last line has no [\r]\n ? */
                l->moredata = 0;
                l->linelen = GETLINE_LINE_NULL;
                l->line = l->buf;
                l->buf[0] = '\0';
                return GETLINE_EOF;
            }
            l->len += len;
            l->line = l->buf;
            l->bufend = &(l->buf[l->len - 1]);
        }
    }
    last_r = 0;
    p = l->buf;
    while (p <= l->bufend )
    {
        switch (*p)
        {
        case '\r':
            last_r = 1;
            break;
        case '\n':
            l->lineend = p;
            *p = '\0';
            if (last_r) *(p - 1) = '\0';
            l->linelen = strlen(l->line);
            l->moredata = (l->lineend != l->bufend);
            return GETLINE_OK; /* voila, there is a line */
            break;
        default:
            last_r = 0;
        }
        p++;
    }
    l->lineend = NULL;
    l->linelen = GETLINE_LINE_INCOMPLETE;
    l->moredata = 0;
    return GETLINE_NOLINE; /* could not complete a whole line */
}



void linebuf_uninit(LineBuffer *l)
{
    if (l == NULL) return;
    if (l->buf != NULL) free(l->buf);
    free(l);
    l = NULL;
}
