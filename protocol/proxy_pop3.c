#include "proxy.h"
#include "proxy_smtp.h"
#include "proxy_pop3.h"

extern struct GatewayConfiguration *g_gateway_config;

static pthread_mutex_t keep_alive_lock;

extern unsigned long send_pop3_mailfile(char * mailfile,  ProxyContext *p);






int proxy_pop3(ProxyContext *proxy_context)
{
    int ret;
    int scanner_ret;
    int error = 0;


    int scanfd = -1;


    int get_msg_size = 0; //就是说get_msg_size这个变量在收到list命令之后会置1
    int first = 1, didit = 0, again = 0;
    int uidl_processed = 0; // 当收到uidl命令后这个变量会置1

    int *msgnr = NULL, *tmpmsgnr;

    // i, ii, rest, rest2 都是临时变量
    size_t  i, ii, user_len, len_t;
    char *rest, *rest2;
    int trapcapa1 = 0, trapcapa2 = 0, trapcapa3 = 0;



	struct timeval timeout;
	fd_set fds_read;



    if (proxy_context->ssl_enable)
    {
        //do_log(LOG_NOTICE, "MY_POP3S client addr is %s:%i client fd %d", inet_ntoa(p->client_addr.sin_addr), ntohs(p->client_addr.sin_port),p->client_fd);
        ret = SSL_create_conn(proxy_context->server_fd, (struct sockaddr *) &proxy_context->server_addr, proxy_context->socksize, &proxy_context->ssl, &proxy_context->ctx, &proxy_context->sbio);
        ret = SSL_create_conn_server(proxy_context->client_fd, (struct sockaddr *) &proxy_context->client_addr, proxy_context->socksize, &proxy_context->ssl_server, &proxy_context->ctx_server, &proxy_context->sbio_server);
        if (proxy_context->ssl_server != NULL)
        {
            do_log(LOG_DEBUG, "Accepting SSL connection... -->");
            if (SSL_accept(proxy_context->ssl_server) <= 0)
            {
                do_log(LOG_EMERG, "Error accepting SSL connection");
                exit(EXIT_FAILURE); // will never reach here
            }
            //do_log(LOG_DEBUG,"SSL connection accepted on %d fd <--", SSL_get_fd(p->ssl_server));
        }
        //do_log(LOG_DEBUG, "SSL server listens on %d fd",SSL_get_fd(p->ssl_server));
        //do_log(LOG_DEBUG, "Real Client's fd is %d fd",p->client_fd);
        //do_log(LOG_DEBUG, "SSL client listens on %d fd",SSL_get_fd(p->ssl));
        //do_log(LOG_DEBUG, "Real Server's fd is %d fd",p->server_fd);
    }

    else
    {
        ret = connect( proxy_context->server_fd, (struct sockaddr *) &proxy_context->server_addr, proxy_context->socksize );
    }

	

	if ( ret )
	{
		do_log( LOG_CRIT, "ERR: Cannot connect to real-server" );
		return (1);
	}

	do_log( LOG_DEBUG, "starting POP3 mainloop" );

	//状态机开始
	while ( 1 )
	{
		int client_ret;
		int server_ret;

		timeout.tv_sec = 0;
        timeout.tv_usec = 10;
		
		FD_ZERO(&fds_read);
		FD_SET(proxy_context->client_fd, &fds_read);
		FD_SET(proxy_context->server_fd, &fds_read);
		ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, &timeout);

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
				return (0);
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

		//从server端读取一行
        if (proxy_context->ssl_enable)
        {
            server_ret = get_line_ssl(proxy_context->ssl, proxy_context->server_buf);
        }
        else
        {
            server_ret = get_line( proxy_context->server_fd, proxy_context->server_buf);
        }
            
		// 解读从server端读到的内容
		if ( server_ret < 0 )
		{
			if ( server_ret == GETLINE_TOO_LONG )
			{
				do_log( LOG_DEBUG, "Line too long: Getting rest of line." );
			}
			else
			{
				do_log( LOG_DEBUG, "Closing connection (no more input from server)" );
                if ( g_gateway_config->skipsize && (sizeof msgnr) ) 
                {
                    if ( msgnr ) 
                    {
                        free( msgnr );
                    }
                    
                }
				return (0);
			}
		}
		else
		{
			//处理server空操作
			if ( proxy_context->noop )
			{
				if ( !strncasecmp( proxy_context->server_buf->line, POK, 3 ) )
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




        /*
            从这里到前面的while(1)的循环开始的部分，都是打印从client和server收到的信息，
            从此之后才开始真正的分析收到的数据是什么，并根据不同的数据执行不同的操作
        */



		/* 检查UIDL server端命令, 因为3.0版本的新加了把邮件写入文件的操作了 */
		
		if ( !proxy_context->process_state && proxy_context->server_buf->linelen > 0 
			&& strncasecmp( proxy_context->server_buf->line, "uidl", 4 ) )
        {
            uidl_processed = 1; 
        }
			

        // 如果get_msg_size为1，也就是意味着之前有个list的命令
        if ( get_msg_size && !proxy_context->process_state )
        {
            // in LIST, save message sizes to an array. 就是说get_msg_size这个变量在收到list命令之后会置1

            if ( ( !strncasecmp( proxy_context->server_buf->line, "+ok", 3) || ( uidl_processed && !strncasecmp(proxy_context->server_buf->line, ".", 1) ) ) && !didit) 
            {
                do_log( LOG_DEBUG, "Write to client second case 2" );
                didit=1;
                ii=0;

                if ( uidl_processed ) 
                {
                    uidl_processed = 0;
                }
                    
                if ( (msgnr = malloc(STARTWITH)) == NULL) 
                {
                    do_log( LOG_EMERG, "LIST malloc failed!" );
                }
                
            } 
            else 
            {
                if ( (proxy_context->server_buf->line[0]=='.') ) 
                {
                    get_msg_size=0;
                }
                else 
                {
                    i = strtol( proxy_context->server_buf->line, &rest, 10 );
                    if ( i != ii )
                    {
                        if ( ( tmpmsgnr=realloc(msgnr, sizeof(int) * i) ) == NULL ) 
                        {
                            if (msgnr) 
                            {
                                free(msgnr);
                            }
                            do_log(LOG_EMERG,"LIST realloc failed!");
                        }
                        else 
                        {
                            msgnr = tmpmsgnr;
                        }
                            
                        msgnr[i-1] = strtol(rest, &rest2, 10) / 1024;
                        do_log( LOG_DEBUG, "Set msgsize to %d for msgnr %i", msgnr[i-1], i);
                        ii=i;
                    }
                } 
            }
        }
		


		//判断timeout
		if ( client_ret == GETLINE_NEED_READ && server_ret == GETLINE_NEED_READ )
		{
			FD_ZERO( &fds_read );
			FD_SET( proxy_context->client_fd, &fds_read );
			FD_SET( proxy_context->server_fd, &fds_read );
			timeout.tv_sec  = 300;
			timeout.tv_usec = 0;
			if ( (ret = select( proxy_context->server_fd + 1, &fds_read, NULL, NULL, &timeout ) ) < 1 )
			{
				// timeout
				do_log( LOG_DEBUG, "select returned %i", ret );
				break;
			}
			else
				continue;
		}
	
		

		// 清空server端buffer, 对应2455
		if ( proxy_context->process_state > CLOSED_HEADER_BUFFER )
		{
			proxy_context->server_buf->line = NULL;
		}
			


		//扫描client端发送的命令
		if ( proxy_context->client_buf->linelen >= 0 && proxy_context->process_state < READ_MAIL_INTO_FILE )
		{
			// list 这个命令没太看懂，有待检查
            if ( !strncasecmp( proxy_context->client_buf->line, "list", 4) )
            {
                if ( g_gateway_config->skipsize ) 
                {
                    get_msg_size = 1;
                }
                didit = 0;
            }

			// retr 命令, 只有retr和top命令才会进入trapping mail
			else if ( !strncasecmp( proxy_context->client_buf->line, "retr", 4 ) )
			{
				proxy_context->recv_mail_state = POP3_MAIL_WAITING;

				proxy_context->message_num = atoi( &proxy_context->client_buf->line[5] );
				// debug message number
				if ( proxy_context->message_num < 1 )
				{
					do_log(LOG_WARNING," RETR message %i (<1) !!!! ", proxy_context->message_num);
					proxy_context->process_state = INIT;
				}
				else
				{
					if ( g_gateway_config->scanner_enable )
					{
						do_log(LOG_DEBUG, " RETR %i (Scanner Enabled) ", proxy_context->message_num);
						proxy_context->process_state = TRAPPING_EMAIL;
                        proxy_context->action = ACT_RETR;
                        do_log(LOG_DEBUG, " proxy_context->process_state = TRAPPING_EMAIL ; proxy_context->action = ACT_RETR ; ");

                        if ( g_gateway_config->skipsize )
                        {
                            if ( (msgnr[proxy_context->message_num - 1] >= g_gateway_config->skipsize) )
                            {
                                proxy_context->process_state = INIT;
                                proxy_context->action        = 0;
                                do_log( LOG_NOTICE, "Skipped scanning: %s message: %d size: %dk", proxy_context->dspamuser, proxy_context->message_num, msgnr[proxy_context->message_num - 1] );
                            }
                        }
					}
                    else 
                    {
                        do_log( LOG_DEBUG, "RETR %i (Scanner Disabled)", proxy_context->message_num );
                    }
					proxy_context->processed_mail_count++;
				}

			}

            // 处理capa 命令, 但是并没有看懂
            else if ( !strncasecmp( proxy_context->client_buf->line, "capa", 4) )
            {
                trapcapa1 = 1;
                trapcapa2 = 1;
                trapcapa3 = 1;
                do_log( LOG_DEBUG, "Client checking server CAPABILITIES..." );
            }

            /*
			// 处理 user 命令
			else if ( !strncasecmp( proxy_context->client_buf->line, "user", 4 ) ) 
			{
				paramlist_set( proxy_context->params, "%USERNAME%", 
					right( proxy_context->client_buf->line, strlen(proxy_context->client_buf->line) - 5 ) );
				proxy_context->dspamuser = paramlist_get( proxy_context->params, "%USERNAME%" );
			}
            */


			// 处理 top 命令
			else if ( !strncasecmp(proxy_context->client_buf->line, "top", 3) )
			{
				proxy_context->recv_mail_state = POP3_MAIL_WAITING;

				proxy_context->message_num = atoi( &proxy_context->client_buf->line[4] );
				// debug message number
				if ( proxy_context->message_num < 1 )
				{

					do_log( LOG_WARNING, "TOP message %i (<1) !!!! " , proxy_context->message_num );
					proxy_context->process_state = INIT;
					
				}
				else
				{
					do_log( LOG_DEBUG, "TOP %i", proxy_context->message_num );

					if ( g_gateway_config->scanner_enable )
					{
						
                            do_log( LOG_DEBUG, "TOP %i (Scanner Enabled)", proxy_context->message_num );
                            /* enable message parsing (only if scanner enabled) */
                            proxy_context->process_state = TRAPPING_EMAIL;
                            proxy_context->action = ACT_TOP;
                            if ( g_gateway_config->skipsize )
                            {
                                if ( ( msgnr[proxy_context->message_num - 1] >= g_gateway_config->skipsize ) )
                                {
                                    proxy_context->process_state = INIT;
                                    proxy_context->action = ACT_INIT;
                                    do_log( LOG_NOTICE, "Skipped scanning: %s message: %d size: %dk", proxy_context->dspamuser, proxy_context->message_num, msgnr[proxy_context->message_num - 1] );
                                }
                            }
					}
                    else
                    {
                        do_log( LOG_DEBUG,"TOP %i (Scanner Disabled)", proxy_context->message_num);
                    }
					proxy_context->processed_mail_count++;
				}
			}

            else if ( !strncasecmp( proxy_context->client_buf->line, "uidl", 4) )
            {
                uidl_processed = 1;
            }

			else
			{
				proxy_context->process_state = INIT;
			}




            // Compared with 3.0, here we ignore a part, check if there is a space within username
            //int maybe_a_space;





			/* write client_buf to server_fd */
			if ( proxy_context->client_buf->linelen >= 0 )
			{
                if (proxy_context->ssl_enable)
                {
                    ret = write_line_ssl(proxy_context->ssl, WRITELINE_LEADING_RN, proxy_context->client_buf->line);
                }
                else
                {
                    ret = write_line( proxy_context->server_fd, WRITELINE_LEADING_RN, proxy_context->client_buf->line);
                }


				if ( ret )
				{
					do_log( LOG_CRIT, "ERR: Can't send to server!" );
					return (1);
				}
			}
			proxy_context->client_buf->linelen = GETLINE_LINE_NULL;

		} // end of scan client buffer



		// scan server buffer 
		if ( proxy_context->server_buf->linelen >= 0 )
		{
			if ( proxy_context->process_state == TRAPPING_EMAIL )
			{
				if ( !strncasecmp(proxy_context->server_buf->line, "+ok", 3) )
				{

					if ( proxy_context->recv_mail_state == POP3_MAIL_WAITING )
					{
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
							proxy_context->process_state = INIT;
							proxy_context_uninit( proxy_context );
							/* Emergency: proxy: Can't open mailfile */
							g_gateway_config->emergency = make_message( "Critical error opening file '%s', Program aborted.", proxy_context->mailfile );
							do_log( LOG_EMERG, "ERR: Critical error opening file '%s', Program aborted.", proxy_context->mailfile );
							/* Should not reach here as we are dead */
						}
						else
						{
							do_log( LOG_DEBUG, "Created %s", proxy_context->mailfile );
							proxy_context->filename = right( proxy_context->mailfile, 13 );


							proxy_context->process_state       = READ_MAIL_INTO_FILE;

							proxy_context->hdrdate      = 0;
							proxy_context->hdrfrom      = 0;
							proxy_context->hdrto        = 0;

						}

						proxy_context->recv_mail_state = POP3_MAIL_BUILT_FILE;
					}


				}
				else
				{
					do_log( LOG_DEBUG, "process_state=TRAPPING_EMAIL, but we haven't got '+ok' so... setting proxy_context->process_state = INIT" );
					proxy_context->process_state = INIT;
				}
				
			}
			else if ( proxy_context->process_state > TRAPPING_EMAIL )
			{

				// 把buffer里的内容写入文件
             	if ( server_ret == GETLINE_TOO_LONG )
             	{

                	write_line( scanfd, WRITELINE_LEADING_NONE, proxy_context->server_buf->line );//_NONE means do not change line
                	
            	} 
            	else 
            	{
                	write_line( scanfd, WRITELINE_LEADING_N, proxy_context->server_buf->line );//_N means add "\n"
                	
            	}

            	/*
            	error = checktimeout(proxy_context);
            	if ( error < 0 ) 
            	{
            		do_log(LOG_CRIT,"ERR: Writing to client!");
            	}
            	*/


                if ( (proxy_context->client_buf->linelen == 1 && proxy_context->client_buf->line[0] == '.') || (proxy_context->server_buf->linelen == 1 && proxy_context->server_buf->line[0] == '.') )
                {
                	
                    /* mail is complete */
                    error = 0;
                    r_close( scanfd );
                    do_log( LOG_DEBUG, "got '.\\r\\n', mail is complete." );


                    proxy_context->process_state = START_PROCESSING;

    				/* initialize the scanner before scanning the first mail but only if scanning is enabled */
    				if ( g_gateway_config->scanner_enable)
    				{

    					/* Scan the file now */
    					scanner_ret = SCANNER_RET_OK;
    					//snprintf( proxy_context->maildir, 4090, "%s.dir", proxy_context->mailfile );
    					if ( proxy_context->scannerinit > 0 )
    					{
    						// 得到返回值scanner_ret
    						scanner_ret = scan_mailfile( proxy_context );

    						/* see if there was a critical error */
    						if ( scanner_ret == SCANNER_RET_CRIT )
    						{
    							//if ( !p->errmsg )
    							do_log( LOG_CRIT, "ERR: Writing to client!" );
    							/* exact error already reported so kill the child. This should get the sysadmins attention. */
    							proxy_context->process_state = INIT;
    							return (1);
    						}

    					}
    					else
                        {
                            scanner_ret = SCANNER_RET_ERR;   /* ! error */
                        }
    						
    				}
    				else
                    {
                        scanner_ret = SCANNER_RET_OK;
                    }
    					


    				if ( scanner_ret == SCANNER_RET_VIRUS ) /* send mail if no virus */
    				{


    					proxy_context->recv_mail_state = POP3_MAIL_VIRUS;

                        /* Try cleaning it up again */

                        do_log( LOG_CRIT, "ERR: Virusaction failed. Sending -ERR to remind pop3 connection." );
                        

                        if (proxy_context->ssl_enable)
                        {
                            writeline_format_ssl( proxy_context->ssl_server, WRITELINE_LEADING_RN, "-ERR Message contains a virus." );
                            //writeline_format_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, "554 %s", g_gateway_config->smtp_rset_message );


                        }
                        else
                        {
                            writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "-ERR Message contains a virus." );
                            //writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "554 %s", g_gateway_config->smtp_rset_message );

                        }
                        do_log( LOG_DEBUG, "Sending remind to real smtp server." );


                        proxy_context->process_state = INIT;
                        //return (1);
    					

    					//unset_paramsfrommailheader( proxy_context->params );

    					proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
    					proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
    					//if ( config->delit )
    					//unlink( proxy_context->mailfile );

    				}/* virus */
    				else
    				{
                        // 没有virus
                        proxy_context->process_state       = INIT;
                        proxy_context->recv_mail_state 	   = POP3_MAIL_OK;
    				
                    } /* send mail */
    				//r_close( p->mailhdr_fd );
    				//unlink( p->mailhdr );
    				do_log( LOG_DEBUG, "Mail scan complete" );
    				//} /* mail complete */
                }
            	//proxy_context->server_buf->linelen = GETLINE_LINE_NULL;      /* don't send to client */
				/* p->ismail>1 */


			}

			else if ( trapcapa1 && !strncasecmp( proxy_context->server_buf->line, "PIPELINING", 10 ) )
			{
				proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;    /* don't send to client */
				trapcapa1       = 0;
				do_log( LOG_DEBUG, "Ignoring servers PIPELINING capability..." );
			}
			else if ( trapcapa3 && !strncasecmp( proxy_context->server_buf->line, "STLS", 4 ) )
			{
				proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;    /* don't send to client */
				trapcapa3       = 0;
				do_log( LOG_DEBUG, "Ignoring servers STLS capability..." );
			}
			else if ( !strncasecmp( proxy_context->server_buf->line, "250-PIPELINING", 14 ) )
			{
				proxy_context->server_buf->linelen = GETLINE_LINE_NULL;      /* don't send to client */
				do_log( LOG_DEBUG, "Ignoring SMTP servers PIPELINING capability..." );
			}


		}

		if ( proxy_context->recv_mail_state == POP3_MAIL_WAITING || proxy_context->recv_mail_state == POP3_MAIL_BUILT_FILE )
		{
			// 在没有涉及到病毒的情况下
			if ( proxy_context->server_buf->linelen >= 0 )
			{
				/* write server_buf to fd */
				
					if ( strncasecmp( proxy_context->server_buf->line, "354", 3 ) )
					{
						int res;
		                if (proxy_context->ssl_enable)
		                    res = write_line_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
		                else
		                    res = write_line( proxy_context->client_fd, WRITELINE_LEADING_RN, proxy_context->server_buf->line);
		                if ( res )
		                {
		                    do_log( LOG_CRIT, "ERR: Can't send to client: %s", proxy_context->server_buf->line );
		                    return (1);
		                }
					}
					else
					{
						do_log( LOG_DEBUG, "Caught servers 354" );
					}

					proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
					proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
				
			}
			if ( proxy_context->recv_mail_state == POP3_MAIL_BUILT_FILE )
			{
				proxy_context->recv_mail_state = POP3_MAIL_RECVING;
			}

		}
		else if ( proxy_context->recv_mail_state == POP3_MAIL_OK )
		{

			do_log( LOG_DEBUG, "Sending pop3 email to client." );
            unsigned long len;

            if ( (len = send_pop3_mailfile( proxy_context->mailfile,  proxy_context ) ) < 0 )
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
            linebuf_zero( proxy_context->client_buf );

            //unlink( proxy_context->mailfile );          /* we do not unlink virusmails, so only here */
            do_log( LOG_DEBUG, "Mail action complete" );

            

			proxy_context->recv_mail_state = POP3_MAIL_WAITING;
		}	

		else if ( proxy_context->recv_mail_state == POP3_MAIL_VIRUS )
		{
			proxy_context->recv_mail_state = POP3_MAIL_WAITING;
		}


	}

	do_log( LOG_WARNING, "Connection timeout" );
	do_log( LOG_DEBUG, "Child finished" );
	return (0);

}


unsigned long send_pop3_mailfile(char * mailfile,  ProxyContext *p)
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
                    sendret = write_line_ssl(p->ssl_server, WRITELINE_LEADING_NONE, filebuf->line);
                else
                    sendret = write_line(p->client_fd, WRITELINE_LEADING_NONE, filebuf->line);
            }
            else
            {
                if (!gotprd)
                {

                    if (p->ssl_enable)
                        sendret = write_line_ssl(p->ssl_server, WRITELINE_LEADING_RN, filebuf->line);
                    else
                        sendret = write_line(p->client_fd, WRITELINE_LEADING_RN, filebuf->line);
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
            write_line_ssl(p->ssl_server, WRITELINE_LEADING_RN, ".");
        else
            write_line(p->client_fd, WRITELINE_LEADING_RN, ".");
    }
    linebuf_uninit(filebuf);
    r_close(mailfd);
    return len;

}