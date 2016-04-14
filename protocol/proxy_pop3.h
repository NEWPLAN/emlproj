#ifndef _PROXY_POP3_H
#define _PROXY_POP3_H


#define POP3_MAIL_WAITING			0
#define POP3_MAIL_BUILT_FILE		1
#define POP3_MAIL_RECVING			2
#define POP3_MAIL_OK				3
#define POP3_MAIL_VIRUS				4



int proxy_pop3(ProxyContext * proxy_context);
//unsigned long send_imap_mailfile(char * mailfile,  ProxyContext *p);
unsigned long send_pop3_mailfile(char * mailfile,  ProxyContext *p);

#endif
