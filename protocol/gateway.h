#ifndef _GATEWAY_H
#define _GATEWAY_H

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define VERSION "3.0_rc1"

#define PROGRAM               "MailGateway"
#define CONFIGBASE "/etc/MailGateway"
#define MAXFILENAME           4096
typedef void (Sigfunc) (int);
typedef struct GatewayConfiguration
{
    int listen_port;
    int socket_backlog;
    int daemonize;
    char* run_as_user;
    int max_child_process_num;
    int is_child_process;
    char* saved_mail_files_dir;
    char* saved_mail_files_dir_base;

    struct sockaddr_in gateway_self_addr;
    struct sockaddr_in target_server_addr;

    int smtp_port;
    int smtps_port;
    int pop3_port;
    int pop3s_port;
    int imap_port;
    int imaps_port;

    int skipsize;

    unsigned long max_mail_size;

    int scanner_enable;


    char *        emergency;
    char *        smtp_rset_message;

} GatewayConfiguration;




void set_default_configurations();

int child_process_state_check(pid_t pid, int stat);
void do_child_process_terminate_signal(int signr);
Sigfunc * register_signal_callback_function(int signo, Sigfunc *func);
#endif

