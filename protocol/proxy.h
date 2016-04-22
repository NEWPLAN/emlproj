#ifndef _PROXY_H
#define _PROXY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <linux/netfilter_ipv4.h>
#include <unistd.h>
#include <errno.h>

#include "util.h"
#include "log.h"
#include "gateway.h"
#include "getline.h"
#include "getline_ssl.h"
#include "parsefile.h"

/* p->ismail legend:
=0 not processing a message - parsing client commands
   ismail=1 enable message parsing (got RETR/TOP/DATA) - start trapping email
   ismail=2 got "+ok", read the mail into file
   ismail=3 closed header buffer
   ismail=4 have entire message, start processing - email complete, start server noop's
   ismail=5 scanning done, send mail to client
*/

#define SCANNER_RET_OK        0
#define SCANNER_RET_ERR       -1
#define SCANNER_RET_VIRUS     2
#define SCANNER_RET_CRIT      3




#define INIT 0
#define TRAPPING_EMAIL 1
#define READ_MAIL_INTO_FILE 2
#define CLOSED_HEADER_BUFFER 3
#define START_PROCESSING 4
#define PROCESS_COMPLETE_AND_SEND_TO_CLIENT 5

#define POK                      "+OK"

#define SMTPRSET                 "Virus detected! Rejected message!"


#define STARTWITH 1

#define ACT_INIT                 0  /* Action = Initial */
#define ACT_RETR                 1  /* Action = RETR msg from server */
#define ACT_TOP                  2  /* Action = TOP or part of msg from server */



typedef struct ProxyContext
{
    int                 client_fd;
    struct sockaddr_in  client_addr;

    int                 server_fd;
    struct sockaddr_in  server_addr;
    socklen_t           socksize;

    struct LineBuffer      * server_buf;
    struct LineBuffer      * client_buf;

    int                  process_state;
    int                  noop;
    int                  processed_mail_count;

    char                * account_username;
    char                *protocol;


    struct paramlist    * params;


    //char                mailhdr[MAXFILENAME];
    //int                 mailhdr_fd;


    char                mailfile[MAXFILENAME];
    const char          * filename;


    time_t              now;
    unsigned long       bytecount;

    int                 scannerinit;
    int                 nowrite;

    int                 ssl_enable;
    SSL                 * ssl, *ssl_server;
    SSL_CTX             * ctx, *ctx_server;
    BIO                 * sbio, *sbio_server;

    // added by wfx
    unsigned int        message_num;
    char                * dspamuser;
    int                 hdrdate;
    int                 hdrfrom;
    int                 hdrto;

    int                 mailhdr_fd;
    int keep_alive;

    int                 action;
    int                 recv_mail_state;


} ProxyContext;

ProxyContext * proxy_context_init(void);
int proxy_smtp(ProxyContext *proxy_context);
int proxy(ProxyContext *proxy_context);

int scan_mailfile(ProxyContext *proxy_context );
unsigned long send_mailfile(char * mailfile,  ProxyContext *p);




#endif
