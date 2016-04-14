#include "util.h"

static int stralloc_ptr;
static char *strings[8];
static int str_tag[8];

ssize_t r_SSL_write(SSL *ssl, void *buf, size_t size)
{
    char *bufp;
    size_t bytestowrite;
    ssize_t byteswritten;
    size_t totalbytes;

    for (bufp = buf, bytestowrite = size, totalbytes = 0;
            bytestowrite > 0;
            bufp += byteswritten, bytestowrite -= byteswritten)
    {
        byteswritten = SSL_write(ssl, bufp, bytestowrite);
        if ((byteswritten) == -1 && (errno != EINTR))
            return -1;
        if (byteswritten == -1)
            byteswritten = 0;
        totalbytes += byteswritten;
    }
    return totalbytes;
}

pid_t r_waitpid(pid_t pid, int *stat_loc, int options)
{
    pid_t retval;
    while (((retval = waitpid(pid, stat_loc, options)) == -1) && (errno == EINTR)) ;
    return retval;
}

int r_open2(const char *path, int oflag)
{
    int retval;
    while (retval = open(path, oflag), retval == -1 && errno == EINTR) ;
    return retval;
}

char * make_message(const char *fmt, ...)
{
    /* Guess we need no more than 100 bytes. */
    int n, size = 100;
    char *msg;
    va_list ap;
    if ((msg = malloc (size)) == NULL) return NULL;
    while (1)
    {
        /* Try to print in the allocated space. */
        va_start(ap, fmt);
        n = vsnprintf (msg, size, fmt, ap);
        va_end(ap);
        /* If that worked, return the string. */
        if (n > -1 && n < size) return msg;
        /* Else try again with more space. */
        if (n > -1) size = n + 1; /* glibc 2.1 - precisely what is needed*/
        else size *= 2; /* glibc 2.0 - twice the old size*/
        if ((msg = realloc (msg, size)) == NULL) return NULL;
    }
}

char *stralloc(size_t length)
{
    register int i;

    if (UINT_MAX == length) return NULL;      // Assume size_t == unsigned int

    i = stralloc_ptr++;
    ++length;                     // Allow for terminating NUL

    if ((!strings[i]) || (length > strlen(strings[i])))
    {
        strings[i] = (char *)realloc(strings[i], length);
        assert(NULL != strings[i]);
        str_tag[i] = -1;
    }
    else  str_tag[i] = 0;
    stralloc_ptr &= 7;
    return (strings[i]);
    // Maintains 8 strings in a circular buffer
}

char *right(char *string, size_t i)
{
    char *buf;
    size_t strlength = strlen(string);

    if (i > strlength) i = strlength;
    buf = stralloc(i);
    strcpy(buf, &string[strlength - i]);
    return buf;
}

/* copy max count chars from string, start at start.
 * Append \0 at end of returned substring */
char *substr(const char *string, int start, size_t count)
{
    size_t len = strlen(string);

    if (start < 0) start = len + start;
    if (start >= 0 && start < len)
    {
        if (count == 0 || count > len - start) count = len - start;
        char *smtpstr = malloc(count + 1);
        memcpy(smtpstr, string + start, count);
        smtpstr[count] = 0;
        return smtpstr;
    }

    char *smtpstr = malloc(sizeof(char));
    smtpstr[0] = 0; /* The NUL string error return */
    return smtpstr;
}

int stristr(const char *string, const char *pattern)
{
    const char *pptr, *sptr, *start;

    for (start = string; *start != NUL; start++)
    {
        /* find start of pattern in string */
        for ( ; ((*start != NUL) && (toupper(*start) != toupper(*pattern))); start++);
        if (NUL == *start) return -1;
        pptr = pattern;
        sptr = start;
        while (toupper(*sptr) == toupper(*pptr))
        {
            sptr++;
            pptr++;
            /* if end of pattern then pattern was found */
            if (NUL == *pptr) return (start - string);
        }
    }
    return -1;
}



ssize_t r_read(int fd, void *buf, size_t size)
{
    ssize_t retval;
    while (retval = read(fd, buf, size), retval == -1 && errno == EINTR) ;
    return retval;
}

ssize_t r_write(int fd, void *buf, size_t size)
{
    char *bufp;
    size_t bytestowrite;
    ssize_t byteswritten;
    size_t totalbytes;

    for (bufp = buf, bytestowrite = size, totalbytes = 0;
            bytestowrite > 0;
            bufp += byteswritten, bytestowrite -= byteswritten)
    {
        byteswritten = write(fd, bufp, bytestowrite);
        if ((byteswritten) == -1 && (errno != EINTR))
            return -1;
        if (byteswritten == -1)
            byteswritten = 0;
        totalbytes += byteswritten;
    }
    return totalbytes;
}

int r_close(int fildes)
{
    int retval;
    while (retval = close(fildes), retval == -1 && errno == EINTR) ;
    return retval;
}
