#include "proxy.h"
#include "proxy_smtp.h"

extern struct GatewayConfiguration *g_gateway_config;

static pthread_mutex_t keep_alive_lock;
void *keep_server_alive(void *arg)
{
    ProxyContext *proxy_context;
    proxy_context = ( ProxyContext *) arg;

    //time_t last_time = time(NULL);
    struct timeval timeout;
    fd_set fds_read;
    int ret;
    int is_timeout;

    while (1)
    {
        FD_ZERO(&fds_read);
        FD_SET(proxy_context->server_fd, &fds_read);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;


        if ((ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, &timeout)) < 1)
            is_timeout = 1;
        else
            is_timeout = 0;

        pthread_mutex_lock(&keep_alive_lock);
        if (proxy_context->keep_alive)
        {
            if (is_timeout)
            {
                if (proxy_context->ssl_enable)
                    writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "NOOP" );
                else
                    writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "NOOP" );

                proxy_context->noop = 1;
            }
            else
                check_server_alive(proxy_context);

            pthread_mutex_unlock(&keep_alive_lock);
        }
        else
        {
            pthread_mutex_unlock(&keep_alive_lock);
            return;
        }

    }


}

int proxy_smtp(ProxyContext * proxy_context)
{
    int ret;
    int scanner_ret;
    int error = 0;
    //int smtpcmd = 0;
    //int trapdata = 0;
    //int postdata = 0;
    //int smtprstlb = 0;

    int scanfd = -1;
    //int trapcapa1 = 0, trapcapa2 = 0, trapcapa3 = 0;

    //struct timeval timeout;

    fd_set fds_read;

    proxy_context->process_state = SMTP_INIT_WAIT_SERVER;

    //连接至server
    if (proxy_context->ssl_enable)
    {
        ret = SSL_create_conn(proxy_context->server_fd, (struct sockaddr *) &proxy_context->server_addr, proxy_context->socksize, &proxy_context->ssl, &proxy_context->ctx, &proxy_context->sbio);
        ret = SSL_create_conn_server(proxy_context->client_fd, (struct sockaddr *) &proxy_context->client_addr, proxy_context->socksize, &proxy_context->ssl_server, &proxy_context->ctx_server, &proxy_context->sbio_server);
        if (proxy_context->ssl_server != NULL)
        {
            do_log(LOG_DEBUG, "Accepting SSL connection... -->");
            if (SSL_accept(proxy_context->ssl_server) <= 0)
            {
                do_log(LOG_EMERG, "Error accepting SSL connection");
                // will never reach here
                clean_and_exit(proxy_context, EXIT_FAILURE);
            }
            //do_log(LOG_DEBUG,"SSL connection accepted on %d fd <--", SSL_get_fd(p->ssl_server));
        }
    }
    else
        ret = connect( proxy_context->server_fd, (struct sockaddr *) &proxy_context->server_addr, proxy_context->socksize );

    if (ret)
    {
        do_log( LOG_CRIT, "ERR: Cannot connect to real-server");
        //return (1);
        clean_and_exit(proxy_context, 1);
    }


    //状态机开始
    do_log( LOG_DEBUG, "starting mainloop" );
    while ( 1 )
    {
        int client_ret;
        int server_ret;

        switch (proxy_context->process_state)
        {
        case SMTP_INIT_WAIT_SERVER:
        {
            FD_ZERO(&fds_read);
            FD_SET(proxy_context->client_fd, &fds_read);
            FD_SET(proxy_context->server_fd, &fds_read);
            ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, NULL);


            int receive_data = 0;
            int stage_change = 0;

            check_client_alive(proxy_context);

            do
            {
                server_ret = get_line_from_server(proxy_context);
                if ( proxy_context->server_buf->linelen >= 0)
                {
                    if ( !strncasecmp( proxy_context->server_buf->line, "250-STARTTLS", 12 ) )
                    {
                        do_log( LOG_DEBUG, "Ignoring SMTP servers STARTTLS capability..." );
                    }
                    else if ( !strncasecmp( proxy_context->server_buf->line, "250-PIPELINING", 14 ) )
                    {
                        do_log( LOG_DEBUG, "Ignoring SMTP servers PIPELINING capability..." );
                    }
                    else
                    {
                        int res;
                        if (proxy_context->ssl_enable)
                            res = write_line_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
                        else
                            res = write_line( proxy_context->client_fd, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
                        if ( res )
                        {
                            do_log( LOG_CRIT, "ERR: Can't send to client: %s", proxy_context->server_buf->line );
                            //return (1);
                            clean_and_exit(proxy_context, 1);
                        }
                    }

                    proxy_context->server_buf->linelen = GETLINE_LINE_NULL;
                    /* write server_buf to fd */
                    //  if ( smtpcmd && !strncasecmp( proxy_context->server_buf->line, "250", 3 ) )
                    //     smtpcmd--;

                    //  if ( strncasecmp( proxy_context->server_buf->line, "354", 3 ) )
                    //  {

                    //  }
                    //   else
                    //       do_log( LOG_DEBUG, "Caught servers 354" );

                    receive_data = 1;
                    stage_change = 1;
                    //proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;

                    //proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
                }
                else
                    receive_data = 0;
            }
            while (receive_data);
            if (stage_change)
                proxy_context->process_state = SMTP_INIT_WAIT_CLIENT;

        }
        break;
        case SMTP_INIT_WAIT_CLIENT:
        {
            FD_ZERO(&fds_read);
            FD_SET(proxy_context->client_fd, &fds_read);
            FD_SET(proxy_context->server_fd, &fds_read);
            ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, NULL);

            check_server_alive(proxy_context);

            client_ret = get_line_from_client(proxy_context);
            if ( proxy_context->client_buf->linelen >= 0)
            {
                //处理mail from
                if ( !strncasecmp( proxy_context->client_buf->line, "mail from:", 10 ) )
                {
                    char *message_ptr = NULL;
                    unsigned long len;
                    char buf[4096];/* used to set username */

                    message_ptr = proxy_context->client_buf->line + stristr( proxy_context->client_buf->line, "size=" );
                    if ( g_gateway_config->max_mail_size && message_ptr >= proxy_context->client_buf->line )
                    {
                        unsigned long mail_size;
                        /* we have size of message */
                        char *str = NULL;
                        str = substr( message_ptr, 5, strlen( message_ptr ) - 5 );
                        int str_len = strlen(str);
                        mail_size = atoi( str ) / 1024;
                        free( str );
                        if ( str_len > 12 || mail_size > g_gateway_config->max_mail_size )
                        {
                            do_log( LOG_CRIT, "SMTP Message too large for scanning: %i", mail_size );

                            if (proxy_context->ssl_enable)
                            {
                                writeline_format_ssl( proxy_context->ssl_server, WRITELINE_LEADING_RN, "552 5.3.4 Message size exceeds file system imposed limit" );
                                writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "RSET" );
                                writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "QUIT" );
                            }
                            else
                            {
                                writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "552 5.3.4 Message size exceeds file system imposed limit" );
                                writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "RSET" );
                                writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "QUIT" );
                            }
                            //return -1;
                            clean_and_exit(proxy_context, -1);
                            //proxy_context->process_state   = INIT;
                            //
                            //r_close( proxy_context->mailhdr_fd );
                            //unlink( proxy_context->mailhdr );
                        }
                        else
                            do_log( LOG_DEBUG, "smtpsize=%i", mail_size );
                    }
                    len = proxy_context->client_buf->linelen - 10;
                    if ( message_ptr )
                        len = (len - 5) - strlen( message_ptr + 5 );
                    if ( len >= sizeof(buf) )
                        len = sizeof(buf) - 1;
                    if ( len > 0 )
                    {

                        memcpy( buf, (char *) (proxy_context->client_buf->line) + 10, len );
                        buf[len] = '\0';

                        //没初始化啊，这段待查
                        /*  if ( maybe_a_space )
                            {
                                char *pbuf = strchr( buf, ' ' );
                                if ( NULL != pbuf )
                                    *pbuf = '\0';
                            }*/
                        TRIM( buf );
                        if ( strlen( NONULL( buf ) ) > 1 )
                        {
                            paramlist_set( proxy_context->params, "%USERNAME%", buf );
                            proxy_context->account_username = paramlist_get( proxy_context->params, "%USERNAME%" );
                            do_log( LOG_DEBUG, "SMTP USER set to: %s", proxy_context->account_username );
                        }
                        else if ( strlen( NONULL( proxy_context->account_username ) ) )
                            do_log( LOG_DEBUG, "SMTP USER was NULL. Currently set to: %s", proxy_context->account_username );
                        else do_log( LOG_DEBUG, "SMTP USER remains not set" );
                    }

                    proxy_context->process_state = SMTP_INIT_WAIT_SERVER;
                    //smtpcmd++;
                }
                //处理rcpt to
                else if ( !strncasecmp( proxy_context->client_buf->line, "rcpt to:", 8 )  )
                {
                    //smtpcmd++;
                    proxy_context->process_state = SMTP_INIT_WAIT_SERVER;
                }
                else if (!strncasecmp( proxy_context->client_buf->line, "STARTTLS", 8 ))
                {
                    if (proxy_context->ssl_enable)
                        writeline_format_ssl( proxy_context->ssl_server, WRITELINE_LEADING_RN, "502 5.5.2 Error: command not recognized" );
                    else
                        writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "502 5.5.2 Error: command not recognized" );

                    proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
                }


                //处理data
                else if ( !strncasecmp( proxy_context->client_buf->line, "data", 4 )  )
                {
                    /* SMTP message being submitted */
                    if ( g_gateway_config->scanner_enable )
                    {
                        proxy_context->process_state   = SMTP_TRAPPING_EMAIL;
                        //trapdata    = 1; /* do not send "DATA" command to server */
                        do_log( LOG_DEBUG, "intercepted DATA command. " );
                        proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
                        /* been through once? If so, we need to clean up! */
                        //postdata = 0;

                    }
                    proxy_context->processed_mail_count++;
                }
                else
                    proxy_context->process_state = SMTP_INIT_WAIT_SERVER;


                /* write clientbuf to server_fd */
                if ( proxy_context->client_buf->linelen >= 0 )
                {
                    if (proxy_context->ssl_enable)
                        ret = write_line_ssl(proxy_context->ssl, WRITELINE_LEADING_RN, proxy_context->client_buf->line);
                    else

                        ret = write_line( proxy_context->server_fd, WRITELINE_LEADING_RN, proxy_context->client_buf->line);

                    if ( ret )
                    {
                        do_log( LOG_CRIT, "ERR: Can't send to server!" );
                        //return (1);
                        clean_and_exit(proxy_context, 1);
                    }
                }
                proxy_context->client_buf->linelen = GETLINE_LINE_NULL;

            }

        }
        break;
        case SMTP_TRAPPING_EMAIL:
        {

            /* tell the client we will accept their smtp traffic. */
            /**
            It seems RFC2821 says we can reset (abort) a submission
            by sending RSET after a DATA, EOM event like such:
            ...
            DATA
            ...
            .
            RSET
            but in the real world, this does not seem to work.
            So, we are going to let the server hang while we are
            processing this submission. Otherwise, the smtp server will
            send a partial message to the recipient in the event we want
            to abort the sending of the message. If we find that we do not
            wish to send the messge, we can then cleanly abort it.

            This assumes the actual server would have accepted the data
            and in my eyes, is not a clean solution.
             */
            int res;
            if (proxy_context->ssl_enable)
                res = write_line_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, "354 "PROGRAM" "VERSION" accepting data.");
            else
                res = write_line(proxy_context->client_fd, WRITELINE_LEADING_RN, "354 "PROGRAM" "VERSION" accepting data.");
            if (res)
            {
                do_log(LOG_CRIT, "ERR: Can't send 354 to client!");
                //return 1;
                clean_and_exit(proxy_context, 1);
            }
            else
            {
                do_log(LOG_DEBUG, "Sent 354 "PROGRAM" "VERSION" accepting data.");
                //postdata = 1;
            }


            unsigned long len;

            /* do_log(LOG_DEBUG, "Write to client third case 3"); */

            /* Set timer now because we might have parsed alot of message numbers */
            proxy_context->now = time( NULL );  /* Can't see how we can use this for IMAP right now as IMAP hangs onto the socket. */

            /* generate unique filename */
            do_log( LOG_DEBUG, "Generating mailfile..." );

            len = strlen( g_gateway_config->saved_mail_files_dir ) + 14;
            snprintf( proxy_context->mailfile, len, "%smail.XXXXXX", g_gateway_config->saved_mail_files_dir );
            //snprintf( proxy_context->mailhdr, len, "%sp3shdr.XXXXXX", g_gateway_config->saved_mail_files_dir );
            /* if ((scanfd=p3_mkstemp(p->mailfile)) < 0 ){ */
            if ( (scanfd = mkstemp( proxy_context->mailfile ) ) < 0 )
            {
                //proxy_context->process_state = INIT;
                //proxy_context_uninit( proxy_context );
                /* Emergency: proxy: Can't open mailfile */
                g_gateway_config->emergency = make_message( "Critical error opening file '%s', Program aborted.", proxy_context->mailfile );
                do_log( LOG_EMERG, "ERR: Critical error opening file '%s', Program aborted.", proxy_context->mailfile );
                /* Should not reach here as we are dead */
                //return -1;
                clean_and_exit(proxy_context, -1);
            }
            else
            {
                do_log( LOG_DEBUG, "Created %s", proxy_context->mailfile );
                proxy_context->filename = right( proxy_context->mailfile, 13 );


                //if ( (proxy_context->mailhdr_fd = mkstemp( proxy_context->mailhdr ) ) < 0 )
                //{
                //  proxy_context->process_state = INIT;
                //  proxy_context_uninit( proxy_context );
                //  /* Emergency: proxy: Can't open header file */
                //  g_gateway_config->emergency = make_message( "Critical error opening file '%s', Program aborted.", proxy_context->mailhdr );
                //  do_log( LOG_EMERG, "ERR: Critical error opening file '%s', Program aborted.", proxy_context->mailhdr );
                //  /* Should not reach here as we are dead */
                //}

                proxy_context->process_state       = SMTP_READ_MAIL_INTO_FILE;

                pthread_t thread;
                proxy_context->keep_alive = 1;
                pthread_create(&thread, NULL, keep_server_alive, proxy_context);
                //p->header_exists    = 0;
                //p->fakehdrdone      = 0;
                //p->notified     = 0;
                //p->gobogus      = 0;
                //p->boguspos     = 0;
                //p->hdrdate      = 0;
                //p->hdrfrom      = 0;
                //p->hdrto        = 0;
                //p->errmsg       = 0;
                //p->noscan       = 0;
                //p->nosend       = 0;

            }
            //proxy_context->process_state = SMTP_READ_MAIL_INTO_FILE;


        }
        break;
        case SMTP_READ_MAIL_INTO_FILE:
        {
            struct timeval timeout;
            FD_ZERO(&fds_read);
            FD_SET(proxy_context->client_fd, &fds_read);
            timeout.tv_sec = 0;
            timeout.tv_usec = 10;
            ret = select(proxy_context->client_fd + 1, &fds_read, NULL, NULL,  &timeout);

            client_ret = get_line_from_client(proxy_context);

            if ( client_ret == GETLINE_TOO_LONG )
            {
                if ( proxy_context->client_buf->linelen >= 0 )
                {
                    write_line( scanfd, WRITELINE_LEADING_NONE, proxy_context->client_buf->line );
                    //if ( p->ismail < 3 )
                    //  writeline( p->mailhdr_fd, WRITELINE_LEADING_NONE, p->clientbuf->line, NULL );

                }
            }
            else
            {
                if ( proxy_context->client_buf->linelen >= 0 )
                {
                    write_line( scanfd, WRITELINE_LEADING_N, proxy_context->client_buf->line );
                    //if ( p->ismail < 3 )
                    //  writeline( p->mailhdr_fd, WRITELINE_LEADING_N, p->clientbuf->line, NULL );

                }
            }



            if ( (proxy_context->client_buf->linelen == 1 && proxy_context->client_buf->line[0] == '.') || (proxy_context->server_buf->linelen == 1 && proxy_context->server_buf->line[0] == '.') )
            {
                /* mail is complete */
                error = 0;
                r_close( scanfd );
                do_log( LOG_DEBUG, "got '.\\r\\n', mail is complete." );
                //if ( proxy_context->process_state == READ_MAIL_INTO_FILE )
                //  closehdrfile( p );

                proxy_context->process_state = SMTP_START_PROCESSING;

            } /* mail complete */

            proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
            //proxy_context->server_buf->linelen = GETLINE_LINE_NULL;      /* don't send to client */
            /* p->ismail>1 */

        }
        break;
        case SMTP_START_PROCESSING:
        {

            /* initialize the scanner before scanning the first mail but only if scanning is enabled */
            if ( g_gateway_config->scanner_enable)
            {
                //set_maildateparam( p->params );
                //set_paramsfrommailheader( p->mailfile, p->params, p );

                /* Scan the file now */
                scanner_ret = SCANNER_RET_OK;
                //snprintf( proxy_context->maildir, 4090, "%s.dir", proxy_context->mailfile );
                if ( proxy_context->scannerinit > 0 )
                {
                    scanner_ret = scan_mailfile( proxy_context );

                    /* see if there was a critical error */
                    if ( scanner_ret == SCANNER_RET_CRIT )
                    {
                        //if ( !p->errmsg )
                        do_log( LOG_CRIT, "ERR: Writing to client!" );
                        /* exact error already reported so kill the child. This should get the sysadmins attention. */
                        //proxy_context->process_state = INIT;
                        //return (1);
                        clean_and_exit(proxy_context, 1);
                    }

                }
                else
                    scanner_ret = SCANNER_RET_ERR;   /* ! error */
            }
            else
                scanner_ret = SCANNER_RET_OK;


            if ( scanner_ret == SCANNER_RET_VIRUS ) /* send mail if no virus */
            {

                /* Try cleaning it up again */
                do_log( LOG_CRIT, "ERR: Virusaction failed. Sending 554 and reseting smtp data sent." );

                if (proxy_context->ssl_enable)
                {
                    writeline_format_ssl( proxy_context->ssl_server, WRITELINE_LEADING_RN, "554 %s", g_gateway_config->smtp_rset_message );
                    writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "RSET" );
                    //writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "QUIT" );
                }
                else
                {
                    writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "554 %s", g_gateway_config->smtp_rset_message );
                    writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "RSET" );
                    // writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "QUIT" );
                }
                do_log( LOG_DEBUG, "Sending RSET to real smtp server." );
                //unlink( proxy_context->mailfile );

                //proxy_context->process_state = INIT;


                //unset_paramsfrommailheader( proxy_context->params );

                proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
                proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
                //if ( config->delit )
                unlink( proxy_context->mailfile );
                proxy_context->process_state = SMTP_INIT_WAIT_CLIENT;
                //return 1;
                //clean_and_exit(proxy_context, 1);

            }/* virus */
            else
            {
                unsigned long len;
                if ( scanner_ret == SCANNER_RET_ERR )
                {
                    do_log( LOG_ALERT, "ERR: We can't say if it is a virus! So we have to give the client the mail! You should check your configuration/system" );
                    //proxy_context_uninit( proxy_context );
                    /* Emergency: proxy: Scanner returned unexpected error code. */
                    g_gateway_config->emergency = "Scanner returned unexpected error code. You should check your configuration/system.";
                    do_log( LOG_EMERG, "ERR: Scanner returned unexpected error code. You should check your configuration/system." );
                    /* We are dead now. Don't let virus mails pass */

                    clean_and_exit(proxy_context, -1);
                }
                /* no virus  / error / scanning disabled */
                do_log( LOG_DEBUG, "Scanning done, sending mail now." );


                do_log( LOG_DEBUG, "Sending DATA to server." );
                pthread_mutex_lock(&keep_alive_lock);
                proxy_context->keep_alive = 0;
                pthread_mutex_unlock(&keep_alive_lock);

                int res;
                if (proxy_context->ssl_enable)
                    res = write_line_ssl(proxy_context->ssl, WRITELINE_LEADING_RN, "DATA");
                else
                    res = write_line( proxy_context->server_fd, WRITELINE_LEADING_RN, "DATA" );

                if ( res )
                {
                    do_log( LOG_CRIT, "ERR: Can't send DATA to server!" );
                    //return (1);
                    clean_and_exit(proxy_context, 1);
                }

                //proxy_context->server_buf->linelen = GETLINE_LINE_NULL; /* assume 354 from server */
                proxy_context->process_state = SMTP_PROCESS_COMPLETE_SEND_DATA;



            } /* send mail */
            //r_close( p->mailhdr_fd );
            //unlink( p->mailhdr );

        }
        break;
        case SMTP_PROCESS_COMPLETE_SEND_DATA:
        {
            FD_ZERO(&fds_read);
            FD_SET(proxy_context->client_fd, &fds_read);
            FD_SET(proxy_context->server_fd, &fds_read);
            ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, NULL);


            check_client_alive(proxy_context);

            server_ret = get_line_from_server(proxy_context);

            if ( proxy_context->server_buf->linelen >= 0 )
            {

                if ( !strncasecmp( proxy_context->server_buf->line, "354", 3 ) )
                {
                    do_log( LOG_DEBUG, "Caught servers 354" );
                    proxy_context->process_state = SMTP_PROCESS_COMPLETE_SEND_TO_CLIENT;
                }
                else
                {
                    if ( !strncasecmp( proxy_context->server_buf->line, "250", 3 ) )
                    {
                        if (proxy_context->ssl_enable)
                        {
                            writeline_format_ssl( proxy_context->ssl_server, WRITELINE_LEADING_RN, "554 Server Rejected" );
                            writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "RSET" );
                            //writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "QUIT" );
                        }
                        else
                        {
                            writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "554 Server Rejected" );
                            writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "RSET" );
                            ///writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "QUIT" );
                        }
                        do_log( LOG_DEBUG, "Sending RSET to real smtp server." );
                    }
                    else
                    {
                        int res;
                        if (proxy_context->ssl_enable)
                            res = write_line_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
                        else
                            res = write_line( proxy_context->client_fd, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
                        if ( res )
                        {
                            do_log( LOG_CRIT, "ERR: Can't send to client: %s", proxy_context->server_buf->line );
                            //return (1);
                            clean_and_exit(proxy_context, 1);
                        }
                    }

                    proxy_context->process_state = SMTP_INIT_WAIT_CLIENT;
                    //return (1);
                    //clean_and_exit(proxy_context, 1);
                }

                proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
            }
        }
        break;


        case SMTP_PROCESS_COMPLETE_SEND_TO_CLIENT:
        {
            check_client_alive(proxy_context);

            do_log( LOG_DEBUG, "Sending smtp message to server." );
            int len;

            if ( (len = send_smtp_mailfile( proxy_context->mailfile,  proxy_context ) ) < 0 )
            {
                do_log( LOG_CRIT, "ERR: Can't submit mail! We have to quit now!" );
                //proxy_context->process_state = INIT;
                // return (1);
                clean_and_exit(proxy_context, 1);
            }

            do_log( LOG_DEBUG, "Sending done." );
            //proxy_context->process_state       = INIT;
            proxy_context->bytecount        += len;
            proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
            linebuf_zero( proxy_context->server_buf );
            //trapdata = 0;
            unlink( proxy_context->mailfile );          /* we do not unlink virusmails, so only here */
            do_log( LOG_DEBUG, "Mail action complete" );

            proxy_context->process_state = SMTP_INIT_WAIT_SERVER;
        }
        break;

        }


    }

    do_log( LOG_WARNING, "Connection timeout" );
    do_log( LOG_DEBUG, "Child finished" );
    //return (0);
    clean_and_exit(proxy_context, 0);

}


unsigned long send_smtp_mailfile(char * mailfile,  ProxyContext *p)
{

    struct LineBuffer *filebuf, *footbuf;
    int mailfd, footfd;
    int res = 0, sendret = 0, gotprd = 0, gottxt = 0, nogo = 0;
    unsigned long len = 0;
    char svrout[1];

    if ((mailfd = r_open2(mailfile, O_RDONLY )) < 0)
    {
        //proxy_context_uninit(p);
        // Emergency: send_mailfile: Can't open client mail file.
        g_gateway_config->emergency = make_message("Can't open mailfile (%s)!", mailfile);
        do_log(LOG_EMERG, "ERR: Can't open mailfile (%s)!", mailfile);
        clean_and_exit(p, -1);
        //return 0;

    }
    filebuf = linebuf_init(16384);
    footbuf = linebuf_init(512);
    if (!filebuf)
    {
        r_close(mailfd);

        //if (p->ssl_enable)
        //{
        //    SSL_shutdown(p->ssl);
        //    /* 释放SSL */
        //    SSL_free(p->ssl);
        //}

        //r_close(p->server_fd);
        //proxy_context_uninit(p);
        // Emergency: send_mailfile: Unable to get memory.
        g_gateway_config->emergency = "Could not allocate memory for sending mail!";
        do_log(LOG_EMERG, "ERR: Could not allocate memory for sending mail!");

        clean_and_exit(p, -1);
    }

    gotprd = 0;

    while (1)
    {

        //to do check timeout

        if ((res = get_line(mailfd, filebuf)) < 0)
        {
            if (res == GETLINE_TOO_LONG)
            {
                // Buffer contains part of line,
                //take care of later
            }
            else
            {
                // Other error, take care of later
                break;
            }
        }
        if (filebuf->linelen >= 0 )
        {
            len += filebuf->linelen;
            //if (config->debug_message)
            do_log(LOG_DEBUG, ">%s", filebuf->line);

            if ((strncmp(filebuf->line, ".", 1 ) == 0 && strlen(filebuf->line) == 1))
                gotprd = 1;
            if ((strncmp(filebuf->line, ".\r", 2) == 0 && strlen(filebuf->line) == 2))
                gotprd = 1;

            // Take care of buffer here
            if (res == GETLINE_TOO_LONG)
            {

                if (p->ssl_enable)
                    sendret = write_line_ssl(p->ssl, WRITELINE_LEADING_NONE, filebuf->line);
                else
                    sendret = write_line(p->server_fd, WRITELINE_LEADING_NONE, filebuf->line);
            }
            else
            {
                if (!gotprd)
                {

                    if (p->ssl_enable)
                        sendret = write_line_ssl(p->ssl, WRITELINE_LEADING_RN, filebuf->line);
                    else
                        sendret = write_line(p->server_fd, WRITELINE_LEADING_RN, filebuf->line);
                }

            }


            if (sendret == GETLINE_PIPE)
            {
                do_log(LOG_CRIT, "ERR: Client disappeared during mail send!");
                linebuf_uninit(filebuf);
                return EPIPE;
            }
            else if (sendret)
            {
                //proxy_context_uninit(p);
                linebuf_uninit(filebuf);
                // Emergency: send_mailfile: Sending mail to client
                g_gateway_config->emergency = "Sending mail to client";
                do_log(LOG_EMERG, "ERR: Sending mail to client");
                // we are dead now. Should not reach here. But allow it
                //to fall through in case LOG_EMERG is changed in the future.
                //return 1;
                clean_and_exit(p, -1);
            }
        }


    }

    if (res != GETLINE_EOF)
    {
        do_log(LOG_CRIT, "ERR: reading from mailfile %s, error code: %d", mailfile, res);
        linebuf_uninit(filebuf);
        return 1;
    }

    if (gotprd)
    {
        check_client_alive(p);
        do_log(LOG_DEBUG, "Wrote new EOM.");

        if (p->ssl_enable)
            write_line_ssl(p->ssl, WRITELINE_LEADING_RN, ".");
        else
            write_line(p->server_fd, WRITELINE_LEADING_RN, ".");
    }
    linebuf_uninit(filebuf);
    r_close(mailfd);
    return len;

}

int get_line_from_server(ProxyContext * proxy_context)
{
    int server_ret = 0;
    if (proxy_context->ssl_enable)
        server_ret = get_line_ssl(proxy_context->ssl, proxy_context->server_buf);
    else
        //从server段读取一行
        server_ret = get_line( proxy_context->server_fd, proxy_context->server_buf);
    if ( server_ret < 0 )
    {
        if ( server_ret == GETLINE_TOO_LONG )
        {
            do_log( LOG_DEBUG, "Line too long: Getting rest of line." );
        }
        else
        {
            do_log( LOG_DEBUG, "Closing connection (no more input from server)" );

            clean_and_exit(proxy_context, -1);
        }
    }
    else
    {
        //处理server空操作
        if ( proxy_context->noop )
        {
            if ( !strncasecmp( proxy_context->server_buf->line, "250", 3 ) )
            {
                do_log( LOG_DEBUG, "%s: NOOP response. Flushed %i NOOP's", proxy_context->server_buf->line, proxy_context->noop );
                linebuf_zero( proxy_context->server_buf );
                proxy_context->noop = 0;
            }
            else
            {
                do_log( LOG_DEBUG, "Oops, %s doesn't looks like a server's NOOP response. Waiting next...", proxy_context->server_buf->line );
            }
        }
    }
    //debug：打印从server读取的行
    if ( server_ret == GETLINE_OK && proxy_context->server_buf->line != NULL )
    {
        //if ( ( (p->ismail < 2 || p->ismail > 3) || (config->debug_message) )  )
        do_log( LOG_DEBUG, "<-- %s", proxy_context->server_buf->line );
    }
    return server_ret;
}

int get_line_from_client(ProxyContext * proxy_context)
{
    int client_ret = 0;
    //从client端读取一行
    if (proxy_context->ssl_enable)
        client_ret = get_line_ssl(proxy_context->ssl_server, proxy_context->client_buf);
    else
        client_ret = get_line( proxy_context->client_fd, proxy_context->client_buf );
    if ( client_ret < 0 )
    {
        if ( client_ret == GETLINE_TOO_LONG )
        {
            do_log( LOG_DEBUG, "Line too long: Getting rest of line." );
        }
        else
        {
            do_log( LOG_DEBUG, "Closing connection (no more input from client)" );
            clean_and_exit(proxy_context, -1);
        }
    }

    //debug：打印从client读取的行
    if ( client_ret == GETLINE_OK )
    {
        /*
        if ( proxy_context->ismail < 2 || proxy_context->ismail > 4 )
            do_log( LOG_DEBUG, "--> %s", proxy_context->client_buf->line );
        else if ( config->debug_smtp )
        */
        do_log( LOG_DEBUG, "--> %s", proxy_context->client_buf->line );

    }
    return client_ret;
}

void check_client_alive(ProxyContext * proxy_context)
{
    //check client is alive.
    get_line_from_client(proxy_context);
    if ( proxy_context->client_buf->linelen >= 0)
    {
        proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
    }
}

void check_server_alive(ProxyContext * proxy_context)
{
    //check server is alive.
    get_line_from_server(proxy_context);
    if ( proxy_context->server_buf->linelen >= 0)
    {
        proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
    }
}

void clean_and_exit(ProxyContext *proxy_context, int exit_code)
{
    r_close( proxy_context->server_fd );
    r_close( proxy_context->client_fd );

    unlink( proxy_context->mailfile );
    proxy_context_uninit( proxy_context );

    exit(exit_code);
}