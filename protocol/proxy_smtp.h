#ifndef _PROXY_SMTP_H
#define _PROXY_SMTP_H

#define SMTP_INIT_WAIT_SERVER 0
#define SMTP_INIT_WAIT_CLIENT 1
#define SMTP_TRAPPING_EMAIL 2//receive data
#define SMTP_READ_MAIL_INTO_FILE 3//receive server 354
#define SMTP_START_PROCESSING 4
#define SMTP_PROCESS_COMPLETE_SEND_DATA 5
#define SMTP_PROCESS_COMPLETE_SEND_TO_CLIENT 6

int get_line_from_server(ProxyContext * proxy_context);
int get_line_from_client(ProxyContext * proxy_context);
int proxy_smtp(ProxyContext * proxy_context);
unsigned long send_smtp_mailfile(char * mailfile,  ProxyContext *p);
void check_client_alive(ProxyContext * proxy_context);
void check_server_alive(ProxyContext * proxy_context);
void clean_and_exit(ProxyContext *proxy_context, int exit_code);
void *keep_server_alive(void *arg);
#endif