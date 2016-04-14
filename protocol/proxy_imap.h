#ifndef _PROXY_IMAP_H
#define _PROXY_IMAP_H

/*
#define IMAP_INIT_WAIT_CLIENT 0
#define IMAP_INIT_WAIT_SERVER 1
#define IMAP_TRAPPING_EMAIL 2
#define IMAP_READ_MAIL_INTO_FILE 3
#define IMAP_START_PROCESSING 4
#define IMAP_PROCESS_COMPLETE_SEND_DATA 5
#define IMAP_PROCESS_COMPLETE_SEND_TO_CLIENT 6
*/

#define IMAP_INIT 1
#define IMAP_TRAPPING_EMAIL 2
#define IMAP_READ_MAIL_INTO_FILE 3
#define IMAP_START_PROCESSING 4
#define IMAP_PROCESS_COMPLETE_AND_SEND_TO_CLIENT 5

int proxy_imap(ProxyContext * proxy_context);
unsigned long send_imap_mailfile(char * mailfile,  ProxyContext *p);

#endif

