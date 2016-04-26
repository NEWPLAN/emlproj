#include "proxy.h"
#include "proxy_smtp.h"
#include "proxy_imap.h"

extern struct GatewayConfiguration *g_gateway_config;

static pthread_mutex_t keep_alive_lock;


int proxy_imap(ProxyContext *proxy_context)
{
	char  *tmp=NULL;
    char  *tmp1=NULL;
    char  *tmp2=NULL;
    char  *tmp3=NULL;
	char  *tmp4=NULL;
	char  *itag;
	const char spacer[] = " ";

	unsigned long len;
	long int inr;
	int scanner_ret;
	int error;
	int imapinit;
	int scanfd=-1;
	int ret;
	int client_ret, server_ret;

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
    else {
		ret = connect(proxy_context->server_fd, (struct sockaddr *) &proxy_context->server_addr, proxy_context->socksize);
    }
	
	if(ret)
	{
	    do_log(LOG_CRIT, "ERR: Cannot connect to real-server");
	    return 1;
	}

	do_log(LOG_DEBUG, "starting mainloop");
	proxy_context->process_state = IMAP_INIT;
	
	while (1)
	{
		
		timeout.tv_sec = 0;
        timeout.tv_usec = 10;
		
		FD_ZERO(&fds_read);
		FD_SET(proxy_context->client_fd, &fds_read);
		FD_SET(proxy_context->server_fd, &fds_read);
		ret = select(proxy_context->server_fd + 1, &fds_read, NULL, NULL, &timeout);
	
	    /* 从client端读取 */
		if (proxy_context->ssl_enable) 
            client_ret = get_line_ssl(proxy_context->ssl_server, proxy_context->client_buf);
		else 
            client_ret = get_line( proxy_context->client_fd, proxy_context->client_buf);

	    if (client_ret < 0)
	    {
	        if (client_ret == GETLINE_TOO_LONG)
	        {
	            do_log(LOG_DEBUG, "Line too long: Getting rest of line.");
	        }
	        else
	        {
	            do_log(LOG_DEBUG, "Closing connection (no more input from client)");
	            return 0;
	        }
	    }
		
		/*打印从client端读取的内容*/
	    if (client_ret == GETLINE_OK)
	    {
	    	/*
	        if (!strncasecmp(proxy_context->client_buf->line,"login", 6))
	        {
	            if (config->password) 
				{
					do_log(LOG_DEBUG, "--> %s", proxy_context->client_buf->line);
				}
	            else 
            	{
            		do_log(LOG_DEBUG, "--> <login/password sent>");
            	}
	        }
	        
	        else
	        */
			do_log(LOG_DEBUG, "--> %s", proxy_context->client_buf->line);
	    }



		//从服server端读取
        if (proxy_context->ssl_enable) 
            server_ret = get_line_ssl(proxy_context->ssl, proxy_context->server_buf);
        else 
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
		
		//打印从server端读取的内容
		if ( server_ret == GETLINE_OK && proxy_context->server_buf->line != NULL )
		{
			//if ( ( (p->ismail < 2 || p->ismail > 3) || (config->debug_message) )  )
			do_log( LOG_DEBUG, "<-- %s", proxy_context->server_buf->line );
		}

		
		/*
		if ( proxy_context->process_state > CLOSED_HEADER_BUFFER )
		{
			proxy_context->server_buf->line = NULL;
		}
		*/

		//在处理DATA之前的协议交互
		if ( proxy_context->client_buf->linelen >= 0 && proxy_context->process_state < IMAP_READ_MAIL_INTO_FILE )
		{
        	/* scan command the client sent */
			//do_log( LOG_DEBUG, "proxy interaction");

            // Lets do all imap stuff here
            if (stristr(proxy_context->client_buf->line,"fetch")>=0 && (stristr(proxy_context->client_buf->line,"RFC822")>=0 || stristr(proxy_context->client_buf->line,"body")>=0 || stristr(proxy_context->client_buf->line,"text")>=0))
			//if ((stristr(proxy_context->client_buf->line,"fetch")>=0 && (stristr(proxy_context->client_buf->line,"RFC822")>=0 || stristr(proxy_context->client_buf->line,"body")>=0 || stristr(proxy_context->client_buf->line,"text")>=0)) && ((stristr(proxy_context->client_buf->line,"BODYSTRUCTURE")>=0) || (stristr(proxy_context->client_buf->line,"MIME"))>=0))
            {
                // we have something that may be infected or we have header info...
                tmp=strndup(proxy_context->client_buf->line, proxy_context->client_buf->linelen);
				//do_log( LOG_DEBUG, "client: %s", tmp );
				tmp1 = tmp;
                // get IMAP tag number... being we can only work on one command at a time, save it locally.
                itag = strsep(&tmp1,spacer);
                //do_log( LOG_DEBUG, "client: %s", itag );
                // now get message UID number
                tmp2=strsep(&tmp1,spacer);   //command
                //do_log( LOG_DEBUG, "client: %s", tmp2 );
                tmp3=strsep(&tmp1,spacer);   //params
                //do_log( LOG_DEBUG, "client: %s", tmp3 );
				
                if ((stristr(tmp3,"fetch")>=0))
                {
                    while (stristr(tmp3,"fetch")>=0) tmp3=strsep(&tmp1,spacer);
                }
                if (scanfd > 0)
                {
                    do_log(LOG_DEBUG,"%d was not processed.",inr);
                    r_close(scanfd);
                }
                inr=atoi(tmp3);
                do_log(LOG_DEBUG,"getting imap tag: %s msgnr: %d for scanning...",itag,inr);
				free(tmp);
				
				do_log(LOG_DEBUG, "itag: %s", itag);
					
                proxy_context->process_state = IMAP_TRAPPING_EMAIL;
                //p->action=ACT_IMAP;
            }
            else if ((stristr(proxy_context->client_buf->line,"login")>=0))
            {
                //itag LOGIN "user" "password"
                tmp=strndup(proxy_context->client_buf->line,proxy_context->client_buf->linelen);
				tmp1 = tmp;
                tmp2=strsep(&tmp1,spacer); // itag
                tmp3=strsep(&tmp1,spacer); // LOGIN
                tmp4=strsep(&tmp1,spacer); // "user"
                //tmp4=substr(tmp4,0,strlen(tmp4));
                paramlist_set(proxy_context->params, "%USERNAME%",tmp4);

				free(tmp);
           
                proxy_context->dspamuser=paramlist_get(proxy_context->params,"%USERNAME%");
                do_log(LOG_DEBUG,"IMAP USER %s",proxy_context->dspamuser);
                //checklist(proxy_context);
            }
			else
				proxy_context->process_state = IMAP_INIT;

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
					return (1);
				}
			}
			proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
		}

		if ( proxy_context->server_buf->linelen >= 0)
		{
			//do_log(LOG_DEBUG, "check is it ok");
			if (stristr( proxy_context->server_buf->line , "OK Fetch complete")>=0)
			{
				proxy_context->process_state = IMAP_INIT;
			}
			if (  proxy_context->process_state == IMAP_TRAPPING_EMAIL)
			{
				/* scan for answer */
				//if ( !strncasecmp( proxy_context->server_buf->line, "* ok", 4 ))
				//if ((stristr( proxy_context->server_buf->line , "fetch")>=0) && (stristr( proxy_context->server_buf->line , "RFC822")>=0) && (stristr( proxy_context->server_buf->line , "body[]")>=0))
				if ((stristr( proxy_context->server_buf->line , "fetch")>=0) && (stristr( proxy_context->server_buf->line , "body")>=0))
				{
					//do_log(LOG_DEBUG, "Write to client third case 3");
	                imapinit=0;

                    tmp=strndup(proxy_context->server_buf->line,proxy_context->server_buf->linelen);
					tmp1 = tmp;
                    tmp2=strsep(&tmp1,spacer); //itag
                    tmp3=strsep(&tmp1,spacer); //command
                    if ((!strncasecmp(proxy_context->server_buf->line,"*",1)) && (atoi(tmp3)==inr))
                    {
                        // here comes the requested message(part)
                        do_log(LOG_DEBUG, "Starting trap of IMAP message...");
                        imapinit=1;
                    }


	                unsigned long len;

                    /* do_log(LOG_DEBUG, "Write to client third case 3"); */

                    /* Set timer now because we might have parsed alot of message numbers */
                    proxy_context->now = time( NULL );  /* Can't see how we can use this for IMAP right now as IMAP hangs onto the socket. */


                    /* generate unique filename */
                    do_log( LOG_DEBUG, "Generating mailfile..." );

                    len = strlen( g_gateway_config->saved_mail_files_dir ) + 14;
                    snprintf( proxy_context->mailfile, len, "%smail.XXXXXX", g_gateway_config->saved_mail_files_dir );
					

					if ( (scanfd = mkstemp( proxy_context->mailfile ) ) < 0 )
					{
						proxy_context->process_state = IMAP_INIT;
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

						proxy_context->process_state       = IMAP_READ_MAIL_INTO_FILE;

					}
					
					free(tmp);

				}
				else
				{
					do_log( LOG_DEBUG, "process_state = IMAP_TRAPPING_EMAIL, but we haven't got enough message so... setting proxy_context->process_state = IMAP_INIT" );
					proxy_context->process_state = IMAP_INIT;
				}
			}
			else if ( proxy_context->process_state > IMAP_TRAPPING_EMAIL )
			{
				/* that means we are reading mail into file */
				proxy_context->nowrite=0;
                // <itag> OK UID FETCH completed
                
                
				tmp=strndup(proxy_context->server_buf->line,proxy_context->server_buf->linelen);
				tmp1 = tmp;
                tmp2=strsep(&tmp1,spacer);
                tmp3=strsep(&tmp1,spacer);
				

				if ((proxy_context->server_buf->linelen == 1 && proxy_context->server_buf->line[0] == ')'))
				{
					//do_log(LOG_DEBUG,"tmp2: %s, tag: %s", tmp2, itag);
					proxy_context->nowrite=1;
					do_log(LOG_DEBUG,"Set nowrite... IMAP message complete.");
				}

				/*
				if (tmp2 && tmp3)
				{
					//if (!strcasecmp(tmp2,itag) && !strcasecmp(tmp3,"OK"))
					if ((proxy_context->server_buf->linelen == 1 && proxy_context->server_buf->line[0] == ')'))
		            {
		            	//do_log(LOG_DEBUG,"tmp2: %s, tag: %s", tmp2, itag);
		                proxy_context->nowrite=1;
		                do_log(LOG_DEBUG,"Set nowrite... IMAP message complete.");
		            }
				}
				*/

				if ( server_ret == GETLINE_TOO_LONG )
                {
                    if ( proxy_context->server_buf->linelen >= 0 )
                    {
                        write_line( scanfd, WRITELINE_LEADING_NONE, proxy_context->server_buf->line );
                        //if ( p->ismail < 3 )
                        //	writeline( p->mailhdr_fd, WRITELINE_LEADING_NONE, p->clientbuf->line, NULL );
                    }
                }
                else
                {
                    if ( proxy_context->server_buf->linelen >= 0 )
                    {
                        write_line( scanfd, WRITELINE_LEADING_N, proxy_context->server_buf->line );
                        //if ( p->ismail < 3 )
                        //	writeline( p->mailhdr_fd, WRITELINE_LEADING_N, p->clientbuf->line, NULL );
                    }
                }
                
                //do_log(LOG_DEBUG,"Inside p->action==ACT_IMAP");
                if(proxy_context->nowrite)
                {
                    // imap message complete. close the file buffer and scan...
                    error=0;
                    r_close(scanfd);
                    do_log(LOG_DEBUG, "got '%s %s ...', mail is complete.",tmp2,tmp3);
					
                    proxy_context->process_state = IMAP_START_PROCESSING;
					
                    do_log(LOG_DEBUG,"IMAP message start scanning");
                    /* initialize the scanner before scanning the first mail but only if scanning is enabled */
					do_log(LOG_DEBUG,"scannerinit is %d", proxy_context->scannerinit);
					
					if ( g_gateway_config->scanner_enable)
					{

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
								//proxy_context->process_state = IMAP_INIT;
								//return (1);
								clean_and_exit(proxy_context, 1);
							}

						}
						else
							scanner_ret = SCANNER_RET_ERR;   /* ! error */
					}
					else
						scanner_ret = SCANNER_RET_OK;
					
					/*
                    if (g_gateway_config->scanner_enable && proxy_context->scannerinit == SCANNER_INIT_NO)
                    {
                        if (g_gateway_config->scanner->init2)
                        {
                            if (g_gateway_config->scanner->init2(proxy_context)!=0)
                            {
                                proxy_context_uninit(proxy_context);
								
								// Emergency: proxy: Can't initialize scanner
                                g_gateway_config->emergency="Can't initialize scanner";
                                do_log(LOG_EMERG, "ERR: Can't initialize scanner!");
								
                                // Dead now. Configuration error! 
                                proxy_context->scannerinit=SCANNER_INIT_ERR;
                            }
                            else proxy_context->scannerinit=SCANNER_INIT_OK;
                        }
                        else proxy_context->scannerinit=SCANNER_INIT_NULL;
                    }
					do_log(LOG_DEBUG,"scannerinit is %d", proxy_context->scannerinit);
                    //set_maildateparam(proxy_context->params);
                    //set_paramsfrommailheader(proxy_context->mailfile, proxy_context->params,proxy_context);
                    
                    scanner_ret=SCANNER_RET_OK;
                    //snprintf(proxy_context->maildir, 4090, "%s.dir", proxy_context->mailfile);
                    if (proxy_context->scannerinit > 0)
					{
						scanner_ret=scan_mailfile(proxy_context);
					}
                    do_log(LOG_DEBUG,"IMAP message ended scanning scannerret is %d p->mailfile is %s",scanner_ret,proxy_context->mailfile);
					*/
					
					
                    if (scanner_ret==SCANNER_RET_VIRUS)
                    {
                    	/*
                        // virus 
                        if (proxy_context->virinfo) TRIM(proxy_context->virinfo);
                        paramlist_set(proxy_context->params, "%VIRUSNAME%", NONULL(proxy_context->virinfo));
                        paramlist_set(proxy_context->params, "%MAILFILE%", proxy_context->mailfile);
						
                        //if (g_gateway_config->delit) paramlist_set(proxy_context->params, "%P3SCANID%", g_gateway_config->notify);

						else paramlist_set(proxy_context->params, "%SCANID%", proxy_context->filename);
                        if (paramlist_get(proxy_context->params,"%MAILTO%")==NULL)
                        {
                            do_log(LOG_WARNING, "%s from %s:%s to %s:%s from %s user: %s virus: %s file: %s",
                                   paramlist_get(proxy_context->params,"%PROTOCOL%"),
                                   paramlist_get(proxy_context->params,"%CLIENTIP%"), paramlist_get(proxy_context->params,"%CLIENTPORT%"),
                                   paramlist_get(proxy_context->params,"%SERVERIP%"), paramlist_get(proxy_context->params,"%SERVERPORT%"),
                                   paramlist_get(proxy_context->params,"%MAILFROM%"),
                                   paramlist_get(proxy_context->params,"%USERNAME%"), paramlist_get(proxy_context->params,"%VIRUSNAME%"),
                                   paramlist_get(proxy_context->params,"%P3SCANID%")
                                  );
                        }
                        else
                        {
                            do_log(LOG_WARNING, "%s from %s:%s to %s:%s from %s to %s user: %s virus: %s file: %s",
                                   paramlist_get(proxy_context->params,"%PROTOCOL%"),
                                   paramlist_get(proxy_context->params,"%CLIENTIP%"), paramlist_get(proxy_context->params,"%CLIENTPORT%"),
                                   paramlist_get(proxy_context->params,"%SERVERIP%"), paramlist_get(proxy_context->params,"%SERVERPORT%"),
                                   paramlist_get(proxy_context->params,"%MAILFROM%"), paramlist_get(proxy_context->params,"%MAILTO%"),
                                   paramlist_get(proxy_context->params,"%USERNAME%"), paramlist_get(proxy_context->params,"%VIRUSNAME%"),
                                   paramlist_get(proxy_context->params,"%P3SCANID%")
                                  );
                        }
						*/
                        /*
                        Example:
                        C: A222 COPY 1:2 owatagusiam
                        S: * NO Disk is 98% full, please delete unnecessary data
                        S: A222 OK COPY completed
                        
                        do_log(LOG_DEBUG," * NO Message %i contains a virus (%s).", inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));
                        writeline_format(proxy_context->client_fd, WRITELINE_LEADING_RN," * NO Message %i contains a virus (%s).", inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));
                        do_log(LOG_DEBUG," * NO Message %i contains a virus (%s).", inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));
                        writeline_format(proxy_context->client_fd, WRITELINE_LEADING_RN," * NO Message %i contains a virus (%s).", inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));
                        do_log(LOG_DEBUG,"%s NO FETCH failed. Message %i contains a virus (%s).", itag, inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));
                        writeline_format(proxy_context->client_fd, WRITELINE_LEADING_RN,"%s NO FETCH failed. Message %i contains a virus (%s).", itag, inr, paramlist_get(proxy_context->params, "%VIRUSNAME%"));

						//unset_paramsfrommailheader(proxy_context->params);

						if (proxy_context->client_buf->linelen) do_log(LOG_DEBUG,"IMAP: client_buf: %s",proxy_context->client_buf->line);
                        proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
                        if (proxy_context->server_buf->linelen) do_log(LOG_DEBUG,"IMAP: server_buf: %s",proxy_context->server_buf->line);
                        proxy_context->server_buf->linelen = GETLINE_LINE_NULL;
                        */
                        if ( do_virusaction( proxy_context ) != 0 )
                        {

                            /* Try cleaning it up again */
                            do_log( LOG_CRIT, "ERR: Virusaction failed" );


                            if (proxy_context->ssl_enable)
                            {

                                //writeline_format_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, "0x800CCCDD, IMAP Unsolicited Bye" );
								//writeline_format_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, ")" );
								//write_line_ssl(proxy_context->ssl_server, WRITELINE_LEADING_RN, ")" );

								// writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "a01 logout" );
                                //writeline_format_ssl( proxy_context->ssl, WRITELINE_LEADING_RN, "QUIT" );

                            }
                            else
                            {
                                //writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, "0x800CCCDD, IMAP Unsolicited Bye" );
								//writeline_format( proxy_context->client_fd, WRITELINE_LEADING_RN, ")" );
								//write_line( proxy_context->client_fd, WRITELINE_LEADING_RN, ")" );

                                //writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "a01 logout" );
                                //writeline_format( proxy_context->server_fd, WRITELINE_LEADING_RN, "QUIT" );
                            }
							
                            do_log( LOG_DEBUG, "Virus detected, we're gonna give up this mail." );

							proxy_context->process_state = IMAP_TRAPPING_EMAIL;
							proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
                        	linebuf_zero(proxy_context->client_buf);

							//virus = 1;


                            //proxy_context->process_state = IMAP_INIT;
                            //return (1);
                        }

                        //unset_paramsfrommailheader( proxy_context->params );

                        //proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
                        //proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
                    } /* virus */
                    /* see if there was a critical error */
                    if (scanner_ret==SCANNER_RET_CRIT)
                    {
                        //if (!proxy_context->errmsg) do_log(LOG_CRIT,"ERR: Writing to client!");
                        /* exact error already reported so kill the child. This should get the sysadmins attention. */
                        proxy_context->process_state = IMAP_INIT;
                        return 1;
                    }
                    if (scanner_ret!=SCANNER_RET_VIRUS)  /* send mail if no virus */
                    {
                        if (scanner_ret==SCANNER_RET_ERR)
                        {
                            do_log(LOG_ALERT, "ERR: We can't say if it is a virus! So we have to give the client the mail! You should check your configuration/system");
                            proxy_context_uninit(proxy_context);
// Emergency: proxy: Scanner returned unexpected error code.
                            g_gateway_config->emergency="Scanner returned unexpected error code. You should check your configuration/system.";
                            do_log(LOG_EMERG, "ERR: Scanner returned unexpected error code. You should check your configuration/system.");
                            /* We are dead now. Don't let virus mails pass */
                        }
                        /* no virus  / error / scanning disabled */
                        do_log(LOG_DEBUG, "Scanning done, sending mail now.");
                        proxy_context->process_state = IMAP_PROCESS_COMPLETE_AND_SEND_TO_CLIENT;

                        if ((len=send_imap_mailfile(proxy_context->mailfile, proxy_context))<0)
                        {
                            do_log(LOG_CRIT, "ERR: Can't send mail! We have to quit now!");
                            proxy_context->process_state = IMAP_INIT;
                            return 1;
                        }
                        do_log(LOG_DEBUG, "Sending done.");
						
                        proxy_context->process_state = IMAP_TRAPPING_EMAIL;
						
                        proxy_context->bytecount += len;
                        proxy_context->client_buf->linelen = GETLINE_LINE_NULL;
                        linebuf_zero(proxy_context->client_buf);
                        //unlink(proxy_context->mailfile); /* we do not unlink virusmails, so only here */
                        do_log(LOG_DEBUG, "Mail action complete");
                    } /* send mail */
                }
                // End IMAP

				//do_log(LOG_DEBUG, "we are here 1");
				proxy_context->server_buf->linelen = GETLINE_LINE_NULL;      /* don't send to client */
				/* proxy_context->process_state > IMAP_TRAPPING_EMAIL */
				free(tmp);

			}

			/*
			else if ( trapcapa1 && !strncasecmp( proxy_context->server_buf->line, "PIPELINING", 10 ) )
			{
				proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;    // don't send to client 
				trapcapa1       = 0;
				do_log( LOG_DEBUG, "Ignoring servers PIPELINING capability..." );
			}
			else if ( trapcapa3 && !strncasecmp( proxy_context->server_buf->line, "STLS", 4 ) )
			{
				proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;    // don't send to client 
				trapcapa3       = 0;
				do_log( LOG_DEBUG, "Ignoring servers STLS capability..." );
			}
			else if ( !strncasecmp( proxy_context->server_buf->line, "250-PIPELINING", 14 ) )
			{
				proxy_context->server_buf->linelen = GETLINE_LINE_NULL;      // don't send to client 
				do_log( LOG_DEBUG, "Ignoring SMTP servers PIPELINING capability..." );
			}
			*/
		}
		
		/* we are not in mail-reading mode (proxy_context->process_state == IMAP_INIT) */

		if ( proxy_context->server_buf->linelen >= 0 )
		{
			// write server_buf to fd 

			//do_log(LOG_DEBUG, "we are here 2, and virus is %d", virus);
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
				do_log( LOG_DEBUG, "Caught servers 354" );
			proxy_context->server_buf->linelen   = GETLINE_LINE_NULL;
			proxy_context->client_buf->linelen   = GETLINE_LINE_NULL;
		}
		
		
	    

	}

	do_log( LOG_WARNING, "Connection timeout" );
	do_log( LOG_DEBUG, "Child finished" );
	return (0);

}

unsigned long send_imap_mailfile(char * mailfile,  ProxyContext *p) {

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
        //return 0;
        clean_and_exit(p, -1);
    }
    filebuf = linebuf_init(16384);
    footbuf = linebuf_init(512);
    if (!filebuf)
    {
        r_close(mailfd);

		/*
        if (p->ssl_enable)
        {
        	// 释放SSL 
            SSL_shutdown(p->ssl_server);
            
            SSL_free(p->ssl_server); 
        }

        r_close(p->client_fd);
        proxy_context_uninit(p);
		*/
        // Emergency: send_mailfile: Unable to get memory.
        g_gateway_config->emergency = "Could not allocate memory for sending mail!";
        do_log(LOG_EMERG, "ERR: Could not allocate memory for sending mail!");

		clean_and_exit(p, -1);
    }

    gotprd = 0;
    /*	// advance to mailfd pointer to past data already sent:
    	if (config->broken)
    	{
    		if (p->hdroffset && !p->gobogus)
    		{
    			while (p->hdroffset)
    			{
    				res = getlinep3(mailfd, filebuf, NULL);
    				p->hdroffset--;
    			}
    		}
    	}
    	else
    	{
    		if (p->hdroffset)
    		{
    			lseek(mailfd, p->hdroffset, SEEK_SET);
    		}
    		// See if bogus headerline sent
    		if (p->gobogus)
    		{
    			if (p->boguspos < 91)
    			{
    				svrout[0] = BOGUSX[p->boguspos];
    				secure_write(p->client_fd, svrout, 1, p->ssl_server);
    				p->boguspos++;
    			}
    			//now close it
    			writeline(p->client_fd, WRITELINE_LEADING_RN, PERIOD, p->ssl_server);
    			p->gobogus = 0;
    		}
    	}*/
    while (1)
    {
        /*
        sendret = checktimeout(p);
        if (sendret == GETLINE_PIPE)
        {
        	do_log(LOG_CRIT, "ERR: Client disappeared during mail send!");
        	linebuf_uninit(filebuf);
        	return EPIPE;
        }
        else if (sendret)
        {
        	context_uninit(p);
        	linebuf_uninit(filebuf);
        // Emergency: send_mailfile: Sending mail to client
        	g_gateway_config->emergency = "Sending mail to client";
        	do_log(LOG_EMERG, "ERR: Sending mail to client");
        	// we are dead now. Should not reach here. But allow it to fall through in case LOG_EMERG is changed in the future.
        	return 1;
        }*/


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

			if (strncmp(filebuf->line, ")", 1 ) == 0 && strlen(filebuf->line) == 1)
	            gotprd = 1;
			if ((strncmp(filebuf->line, ")\r", 2) == 0 && strlen(filebuf->line) == 2))
                gotprd = 1;
            

            /*
            //if ((strncmp(filebuf->line,"Content-Type: application/pgp-signature",39)==0 && strlen(filebuf->line)==39)) gotpgp=1;
            //if (gotpgp) nogo=1;
            if (strncmp(filebuf->line, "Content-Type: ", 14) == 0)
            {
            	if ((strncmp(filebuf->line, "Content-Type: text/plain;", 25) == 0 && strlen(filebuf->line) == 25)) gottxt = 1;
            	else nogo = 1;
            }
            */

            /*
            if ( gotprd && !nogo)
            {
            	if ((footfd = r_open2(FOOTER, O_RDONLY)) >= 0)
            	{
            		sendret = writeline(fd, WRITELINE_LEADING_RN, "**********", p->ssl_server);
            		while (1)
            		{
            			if ((sendret = getlinep3(footfd, footbuf, NULL)) < 0) break;
            			if (footbuf->linelen >= 0 ) sendret = writeline(fd, WRITELINE_LEADING_RN, footbuf->line, p->ssl_server);
            		}
            		r_close(footfd);
            		writeline_format(fd, NULL, WRITELINE_LEADING_RN, PROGRAM" "VERSION" running on %s.%s", paramlist_get(p->params, "%HOSTNAME%"), paramlist_get(p->params, "%DOMAINNAME%"));
            		sendret = writeline_format(fd, NULL, WRITELINE_LEADING_RN, "%s", paramlist_get(p->params, "%VDINFO%"));
            		sendret = writeline(fd, WRITELINE_LEADING_RN, "**********", p->ssl_server);
            	}
            }*/

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

    if (res != GETLINE_EOF) {
        do_log(LOG_CRIT, "ERR: reading from mailfile %s, error code: %d", mailfile, res);
        linebuf_uninit(filebuf);
        return 1;
    }

    if (!gotprd) {
        do_log(LOG_DEBUG, "Wrote new EOM.");

		if (p->ssl_enable)
            write_line_ssl(p->ssl_server, WRITELINE_LEADING_RN, ")");
        else
            write_line(p->client_fd, WRITELINE_LEADING_RN, ")");

    }
    linebuf_uninit(filebuf);
    r_close(mailfd);
    return len;
	
}