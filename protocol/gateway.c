#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <pwd.h>
#include <grp.h>

#include <linux/netfilter_ipv4.h>
#include <unistd.h>
#include <errno.h>

#include <signal.h>


#include "log.h"
#include "gateway.h"
#include "getline.h"
#include "proxy.h"
#include "emailhead.h"

struct GatewayConfiguration *g_gateway_config;
int child_process_num;//进程数

int main(int argc, char ** argv)
{
    int pid, sid, cuid, guid;
    struct passwd *pws;
    struct group *grp;

    int socket_fd, operate_socket_result, reuse_addr_val;
    int conn_fd;
    int saved_mail_files_dir_name_len;
    ProxyContext * proxy_context = NULL;
    struct sockaddr_in  accept_addr;
    socklen_t socksize = sizeof(struct sockaddr_in);


    //为全局设置分配空间
    if ((g_gateway_config = malloc(sizeof(struct GatewayConfiguration))) == NULL)
    {
        do_log(LOG_EMERG, "ERR: No enough memory!");
        return 1;
    }

    //设置初始化
    set_default_configurations();

    //to do 载入配置

    //建立socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        do_log(LOG_EMERG, "ERR: Can't open socket!");

    reuse_addr_val = 1;
    operate_socket_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_val, sizeof(reuse_addr_val));
    if (operate_socket_result < 0)
        do_log(LOG_EMERG, "ERR: Can't create socket!");

    operate_socket_result = bind(socket_fd, (struct sockaddr *) &g_gateway_config->gateway_self_addr, sizeof(g_gateway_config->gateway_self_addr));
    if (operate_socket_result < 0)
        do_log(LOG_EMERG, "ERR: Can't bind to socket!");

    if (listen(socket_fd, g_gateway_config->socket_backlog))
        do_log(LOG_EMERG, "ERR: Can't listen on socket");

    do_log(LOG_NOTICE, "Listening on %i", ntohs(g_gateway_config->gateway_self_addr.sin_port));

    //daemonize
    if (g_gateway_config->daemonize)
    {
        if ((pid = fork()) < 0)
            exit(EXIT_FAILURE);
        else if (pid != 0)
            exit(EXIT_SUCCESS);

        if ((sid = setsid()) < 0)
            exit(EXIT_FAILURE);

        if (chdir("/"))
            exit(EXIT_FAILURE);

        umask(0);
        r_close(STDIN_FILENO);
        r_close(STDOUT_FILENO);
        r_close(STDERR_FILENO);
    }

    //降权
    cuid = getuid();
    if (cuid == 0)
    {
        do_log(LOG_DEBUG, "Changing uid (we are root)");
        pws = getpwnam(g_gateway_config->run_as_user);
        // Emergency: main: Unknown user in configuration file
        if (pws == NULL)
            do_log(LOG_EMERG, "ERR: Unknown User '%s'", g_gateway_config->run_as_user);
        if (setgid(pws->pw_gid))
            do_log(LOG_DEBUG, "setgid returned: %s", strerror(errno));
        if (setuid(pws->pw_uid))
            do_log(LOG_DEBUG, "setuid returned: %s", strerror(errno));
    }
    cuid = getuid();
    guid = getgid();

    pws = getpwuid(cuid);
    grp = getgrgid(guid);
    do_log(LOG_DEBUG, "Running as user: %s group: %s", pws->pw_name, grp->gr_name);



    if (Email_InitAll() == 0)
        exit(0);

    //to do 写pid
    /*
        if ((pd = fopen(config->pidfile, "w+")) != NULL)
        {
            fprintf(pd, "%i\n", getpid());
            fclose(pd);
        }

        else do_log(LOG_CRIT, "Can't write PID to %s", PID_FILE);
    */

    child_process_num = 0;

    //to do 注册信号量
    //to do 上层模块初始化


    if (register_signal_callback_function(SIGCHLD, do_child_process_terminate_signal) < 0)
        do_log(LOG_EMERG, "Could not set signal handler SIGCHLD");

    do_log(LOG_DEBUG, "Waiting for connections.....");

    while ((conn_fd = accept(socket_fd, (struct sockaddr *)&accept_addr, &socksize)) >= 0)
    {
        do_log(LOG_DEBUG, "Connection received.....");
        /*
        if (clean)
        {
            if (!numprocs) exit(EXIT_FAILURE);
            else do_log(LOG_WARNING, "Shutting down and cannot fork. %i children to go... ", numprocs);
            break;
        }
         */

        //如果是父进程，负责控制子进程的数量
        if ((pid = fork()) > 0)
        {
            /* parent */
            /*
            if (config->strace)
            {
                slen = strlen("sudo strace -p ") + strlen(" > /var/spool/p3scan/strace 2>&1 ") + 11;
                snprintf(spmcmd, slen, "sudo strace -p %i > /var/spool/p3scan/strace%i 2>&1 ", pid, pid);
                do_log(LOG_DEBUG, "***** strace=%s", spmcmd);
                if ((strace = popen(spmcmd, "r")) == NULL) do_log(LOG_EMERG, "Strace failed!");
            }*/

            //child_process_num++;
            __sync_fetch_and_add(&child_process_num, 1);

            do_log(LOG_DEBUG, "Forked, pid=%i, child_process_num=%i", pid, child_process_num);

            close (conn_fd);

            if (child_process_num >= g_gateway_config->max_child_process_num)
            {
                int stat = 0;
                do_log(LOG_WARNING, "MAX_CHILDS (%i) reached!", g_gateway_config->max_child_process_num);

                while (1)
                {
                    do_log(LOG_DEBUG, "r_waitpid");
                    pid = r_waitpid(-1, &stat, 0); // blocking
                    do_log(LOG_DEBUG, "child_process_state_check");
                    if (child_process_state_check(pid, stat))
                        break;
                }
            }


        }
        //如果是子进程，处理连接
        else
        {
            g_gateway_config->is_child_process = 1;
            pid = getpid();
            //剩余空间检查
            /*
            if ( statvfs( g_gateway_config->saved_mail_files_dir, &fs ) == SCANNER_RET_ERR)
            {
                //config->emergency = "Unable to get available space!";
                do_log(LOG_EMERG, "ERR: Unable to get available space!");
                return SCANNER_RET_CRIT; // Should never reach here, but keep it clean. :)
            }
            if (fs.f_bsize == 1024)
                kbfree = fs.f_bavail;
            else
                kbfree = fs.f_bsize * (fs.f_bavail / 1024) + fs.f_bavail % 1024 * fs.f_bsize / 1024;

            if (config->freespace != 0 && kbfree < config->freespace)
            {
            // Emergency: main: Not enough file space
                config->emergency = make_message("Not enough space! Available space: %lu", kbfree);
                do_log(LOG_EMERG, "ERR: Not enough space! Available space: %lu", kbfree);
                do_sigterm_proxy(1); // Should never reach here, but keep it clean. :)
                exit(EXIT_SUCCESS);
            }*/


            //设置邮件存储目录
            saved_mail_files_dir_name_len = strlen(g_gateway_config->saved_mail_files_dir_base) + 50;

            if ((g_gateway_config->saved_mail_files_dir = malloc(saved_mail_files_dir_name_len)) == NULL)
                do_log(LOG_EMERG, "No memory for saved_mail_files_dir.");
            snprintf(g_gateway_config->saved_mail_files_dir, saved_mail_files_dir_name_len, "%s/%d_", g_gateway_config->saved_mail_files_dir_base, pid);

            do_log(LOG_DEBUG, "setting the saved_mail_files_dir to %s", g_gateway_config->saved_mail_files_dir);


            /*
            if (clean_child_directory(pid))
                do_log(LOG_EMERG, "Error calling clean child directory!");
            */

            // if ((mkdir (g_gateway_config->saved_mail_files_dir, S_IRWXU) < 0))
            //     do_log(LOG_EMERG, "Could not create saved_mail_files_dir %s", g_gateway_config->saved_mail_files_dir);

            //to do 设置信号
            /*
            if (p3sigaction(SIGCHLD, SIG_DFL) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGCHLD"); //unset sigaction handler for child
            if (p3sigaction(SIGPIPE, SIG_IGN) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGPIPE"); // don't die on SIGPIPE
            if (p3sigaction(SIGTSTP, SIG_IGN) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGTSTP"); // Various TTY signals
            if (p3sigaction(SIGTTOU, SIG_IGN) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGTTOU");
            if (p3sigaction(SIGTTIN, SIG_IGN) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGTTIN");
            if (p3sigaction(SIGTERM, do_sigterm_proxy) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGTERM child");
            if (p3sigaction(SIGINT,  do_sigterm_proxy) < 0) do_log(LOG_EMERG, "Could not set sigaction handler SIGINT child");
             */
            if (register_signal_callback_function(SIGCHLD, SIG_DFL) < 0)
                do_log(LOG_EMERG, "Could not set sigaction handler SIGCHLD"); // unset sigaction handler for child

            do_log(LOG_DEBUG, "Initialize Context");

            if ((proxy_context = proxy_context_init()) == NULL)
                do_log(LOG_EMERG, "Could not init stuct(p)");
            proxy_context->client_fd = conn_fd;
            proxy_context->client_addr = accept_addr;

            do_log(LOG_DEBUG, "starting proxy");
            if (proxy(proxy_context))
            {

            }
            //responsemsg = strdup("Critical abort");

            else
            {

            }
            //responsemsg = strdup("Clean Exit");


            //结束
            free(proxy_context);
            proxy_context = NULL;
            return 0;
        }
    }


    //while (1) {}
    free(g_gateway_config);
    g_gateway_config = NULL;
    return 0;
}





void set_default_configurations()
{
    g_gateway_config->listen_port = 8888;

    g_gateway_config->socket_backlog = 5;
    g_gateway_config->daemonize = 0;
    g_gateway_config->run_as_user = "mail";
    g_gateway_config->max_child_process_num = 3000;
    g_gateway_config->is_child_process = 0;
    g_gateway_config->saved_mail_files_dir = "/var/spool/gateway";
    g_gateway_config->saved_mail_files_dir_base = "/var/spool/gateway";

    g_gateway_config->gateway_self_addr.sin_addr.s_addr = INADDR_ANY;
    g_gateway_config->gateway_self_addr.sin_port = htons(g_gateway_config->listen_port);
    g_gateway_config->gateway_self_addr.sin_family = AF_INET;

    g_gateway_config->target_server_addr.sin_addr.s_addr = INADDR_ANY;
    g_gateway_config->target_server_addr.sin_port = htons(g_gateway_config->listen_port);
    g_gateway_config->target_server_addr.sin_family = AF_INET;

    g_gateway_config-> smtp_port = 25;
    g_gateway_config-> smtps_port = 465;
    g_gateway_config-> pop3_port = 110;
    g_gateway_config-> pop3s_port = 995;
    g_gateway_config-> imap_port = 143;
    g_gateway_config-> imaps_port = 993;

    g_gateway_config->max_mail_size = 500000;
    g_gateway_config->scanner_enable = 1;

    g_gateway_config->smtp_rset_message = SMTPRSET;

    g_gateway_config->skipsize = 0;
}

void do_child_process_terminate_signal(int signr)
{
    pid_t pid;
    int stat;
    if (child_process_num > 0)
    {
        while ((pid = r_waitpid(-1, &stat, WNOHANG)) > 0)
            child_process_state_check(pid, stat);
    }

    /*
    if (clean && !numprocs)
    {
        do_log(LOG_NOTICE, "terminating due to child error.");
        raise(SIGTERM);
    }*/
}

int child_process_state_check(pid_t pid, int stat)
{
    int termsig = WTERMSIG(stat);
    int signaled = WIFSIGNALED(stat);
    int stopped = WIFSTOPPED(stat);

    if (child_process_num > 0)
        //child_process_num--;
        __sync_fetch_and_sub(&child_process_num, 1);
    if (termsig)
        do_log(LOG_CRIT, "ERR: Attention: child with pid %i died with abnormal termsignal (%i)! This is probably a bug. child_process_num is now %i", pid, termsig, child_process_num);
    else if (signaled)
        do_log(LOG_DEBUG, "waitpid: child %i died with a signal! %i, child_process_num is now %i", pid, WTERMSIG(stat), child_process_num);
    else if (stopped)
        do_log(LOG_DEBUG, "waitpid: child %i stopped! %i, child_process_num is now %i", pid, WSTOPSIG(stat), child_process_num);
    else
        do_log(LOG_DEBUG, "waitpid: child %i died with status %i, child_process_num is now %i", pid, WEXITSTATUS(stat), child_process_num);
    return 1;
}

Sigfunc * register_signal_callback_function(int signo, Sigfunc *func)
{
    struct sigaction act, oact;
    sigset_t chldmask, savemask;

    act.sa_handler = func;
    act.sa_flags = 0;
    // avoid zombies
    if (signo == SIGCHLD && (func != SIG_DFL)) act.sa_flags |= SA_NOCLDSTOP;

    if (signo == SIGCHLD && (func == SIG_DFL)) act.sa_flags |= SA_NOCLDWAIT;

    // end avoid zombies
    if (signo != SIGALRM)
        act.sa_flags |= SA_RESTART;
    if ((sigemptyset(&act.sa_mask) == -1) || (sigaction(signo, &act, &oact) == -1))
        return (SIG_ERR);
    return (oact.sa_handler);
}