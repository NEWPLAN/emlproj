#ifndef _GETLINE_SSL_H
#define _GETLINE_SSL_H

#include "getline.h"

#define BUFSIZZ 1024
#define CERTFILE "cert.pem"

extern BIO *bio_err;
int berr_exit (char *string);
int err_exit(char *string);

SSL_CTX *initialize_ctx();
void destroy_ctx(SSL_CTX *ctx);

#ifndef ALLOW_OLD_VERSIONS
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
#error "Must use OpenSSL 0.9.6 or later"
#endif
#endif


/* reads a line from fd or l->buf is there any
 * this function is non blocking.
 * return values:
 * GETLINE_OK        : there is a (complete line)
 * GETLINE_NOLINE    : we've received data, but no complete line
 * GETLINE_NEED_READ : we need to read, but select returned no data
 * GETLINE_EOF       : fd is EOF
 * GETLINE_TOO_LONG  : The line can't be retrieved because buf is too
 *                     small. The (incomplete) line is accessable!
 *                     You can call getline again, but it will return
 *                     the rest of that line and not the next one!
 */

int get_line_ssl(SSL * ssl, struct LineBuffer * l);

/* writes line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int write_line_ssl(SSL * ssl, int leading, char * c);

/* writes a formatted line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int writeline_format_ssl(SSL * ssl, int leading, char * fmt, ...);

/* write len bytes of buf to fd
 * if fd can't receive whole len at once, write repeats
 * until all is sent */
int secure_write_ssl(SSL * ssl, char * buf, int len);

/* runs select (read) on fd with a timeval of 0 (not NULL!) */
int select_fd_read_ssl(SSL * ssl);

/* creates and destroy ssl connection */
int SSL_create_conn(int fd, struct sockaddr * addr, int addr_len, SSL ** p_ssl, SSL_CTX ** p_ctx, BIO ** p_sbio);
void SSL_destroy_conn(int sock, SSL * ssl, SSL_CTX * ctx, BIO * sbio);

#endif /* ifndef _GETLINE_H */
