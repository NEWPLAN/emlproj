#include "proxy.h"
#include "proxy_smtp.h"
#include "proxy_imap.h"
#include "emailhead.h"

extern struct GatewayConfiguration *g_gateway_config;
static volatile int loop = 0;

/* p->ismail legend:
   ismail=0 not processing a message - parsing client commands
   ismail=1 enable message parsing (got RETR/TOP/DATA) - start trapping email
   ismail=2 got "+ok", read the mail into file
   ismail=3 closed header buffer
   ismail=4 have entire message, start processing - email complete, start server noop's
   ismail=5 scanning done, send mail to client
*/

#include <assert.h>
ProxyContext * proxy_context_init(void)
{
    struct ProxyContext * proxy_context;
    if ((proxy_context = malloc(sizeof(struct ProxyContext))) == NULL)
        return NULL;
	memset(proxy_context,0, sizeof(proxy_context));
	
    proxy_context->keep_alive = 0;
    proxy_context->client_fd = -1;
    proxy_context->server_fd = -1;
    proxy_context->socksize = sizeof(struct sockaddr_in);
    proxy_context->server_buf = linebuf_init(MAXFILENAME);
    proxy_context->client_buf = linebuf_init(MAXFILENAME);

    proxy_context->params   = paramlist_init();

    proxy_context->now = time(NULL);
    proxy_context->noop = 0;
    proxy_context->processed_mail_count = 0;

	proxy_context->protocol = (char*)malloc(32);
	assert(proxy_context->protocol!=NULL);
	
    proxy_context->filename = NULL;



    proxy_context->process_state = INIT;

    proxy_context->account_username = NULL;

    proxy_context->scannerinit = 1;
    proxy_context->bytecount = 0;

    proxy_context->ssl_enable = 0;


    proxy_context->ssl = NULL;
    proxy_context->ssl_server = NULL;

    proxy_context->ctx = NULL;
    proxy_context->ctx_server = NULL;
    proxy_context->sbio = NULL;
    proxy_context->sbio_server = NULL;
    proxy_context->hdrfrom = 0;
    proxy_context->hdrto = 0;
    // added by wfx
    proxy_context->hdrdate = 0;
    proxy_context->action = 0;

    proxy_context->recv_mail_state = 0;

    //proxy_context->mailhdr_fd   = -1;
    return proxy_context;
}

void proxy_context_uninit(ProxyContext * proxy_context)
{
    if (proxy_context->client_fd != -1 )
        r_close(proxy_context->client_fd);
    //if (proxy_context->mailhdr_fd != -1 )
    //  r_close(proxy_context->mailhdr_fd);
    if (proxy_context->server_fd != -1 )
    {
        if (proxy_context->ssl_enable)
            SSL_destroy_conn(proxy_context->server_fd, proxy_context->ssl, proxy_context->ctx, proxy_context->sbio);
        else
            r_close(proxy_context->server_fd);
    }
    if (!loop)
        paramlist_uninit(&proxy_context->params);
    linebuf_uninit(proxy_context->client_buf);
    linebuf_uninit(proxy_context->server_buf);
//  if (config->broken)
//      linebuf_uninit(proxy_context->hdrbuf);
    free(proxy_context);
}

int proxy(ProxyContext *proxy_context)
{

    if ( htonl( INADDR_ANY ) == g_gateway_config->target_server_addr.sin_addr.s_addr )
    {
        if ( getsockopt( proxy_context->client_fd, SOL_IP, SO_ORIGINAL_DST, &proxy_context->server_addr, &proxy_context->socksize ) )
        {
            do_log( LOG_CRIT, "ERR: No IP-Conntrack-data (getsockopt failed)" );
            return (1);
        }
        /* try to avoid loop */
        if ( ( (ntohl( proxy_context->server_addr.sin_addr.s_addr ) == INADDR_LOOPBACK) && proxy_context->server_addr.sin_port == g_gateway_config->gateway_self_addr.sin_port)
                /* src.ip == dst.ip */
                || (proxy_context->server_addr.sin_addr.s_addr == proxy_context->client_addr.sin_addr.s_addr) )
        {
            //loop = 1;
            do_log( LOG_CRIT, "ERR: Oops, that would loop!" );
            return (1);
        }
    }
    else
    {
        /* non-proxy mode */
        proxy_context->server_addr.sin_addr.s_addr  = g_gateway_config->target_server_addr.sin_addr.s_addr;
        proxy_context->server_addr.sin_port     = g_gateway_config->target_server_addr.sin_port;
    }
    do_log( LOG_DEBUG, "start connect to server %i", ntohs( proxy_context->server_addr.sin_port ));
    /* open socket to 'real-server' */
    if ( (proxy_context->server_fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        do_log( LOG_CRIT, "ERR: Cannot open socket to real-serve:" );
        return (1);
    }


    /* Check connection type */

    do_log( LOG_NOTICE, "Real-server address is %d:%i", inet_ntoa( proxy_context->server_addr.sin_addr ), ntohs( proxy_context->server_addr.sin_port ) );

    //smtp
    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->smtp_port )
    {

        //    p->cksmtp   = 1;    /* Processing an SMTP message */
        //    p->checksmtp    = 1;    /* We should scan it */
        do_log( LOG_NOTICE, "SMTP Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );
        
        strcpy(proxy_context->protocol, "smtp");
        return proxy_smtp(proxy_context);
    }

    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->smtps_port )
    {

        //    p->cksmtp   = 1;    /* Processing an SMTP message */
        //    p->checksmtp    = 1;    /* We should scan it */
        do_log( LOG_NOTICE, "SMTP Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );
        proxy_context->ssl_enable = 1;
        strcpy(proxy_context->protocol, "smtps");
        return proxy_smtp(proxy_context);
    }


    //pop3, add by wfx
    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->pop3_port )
    {


        do_log( LOG_NOTICE, "POP3 Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );

		strcpy(proxy_context->protocol, "pop3");
        return proxy_pop3(proxy_context);
    }

    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->pop3s_port )
    {

        do_log( LOG_NOTICE, "POP3S Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );
        proxy_context->ssl_enable = 1;
        strcpy(proxy_context->protocol, "pop3s");
        return proxy_pop3(proxy_context);
    }


    //imap
    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->imap_port )
    {
        do_log( LOG_NOTICE, "IMAP Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );

		strcpy(proxy_context->protocol, "imap");
        return proxy_imap(proxy_context);
    }

    if ( ntohs( proxy_context->server_addr.sin_port ) == g_gateway_config->imaps_port )
    {
        do_log( LOG_NOTICE, "IMAP Connection from %d:%i", inet_ntoa( proxy_context->client_addr.sin_addr ), ntohs( proxy_context->client_addr.sin_port ) );
        proxy_context->ssl_enable = 1;
        strcpy(proxy_context->protocol, "imaps");
        return proxy_imap(proxy_context);
    }

    return -1;
}

int scan_mailfile( ProxyContext *proxy_context )
{

    EmailType et;
    et.filePath = proxy_context->mailfile;
    et.srcIP = proxy_context->client_addr;
    et.destip = proxy_context->server_addr;
    
    et.ipfrom = (char*)malloc(32);
    et.ipto = (char*)malloc(32);
    
    strcpy(et.ipfrom, inet_ntoa(proxy_context->client_addr.sin_addr));
    strcpy(et.ipto, inet_ntoa(proxy_context->server_addr.sin_addr));
    
    et.protocol = proxy_context->protocol;
    printf("filepath= %s\n",et.filePath);
    
    printf("ipform %s\n", et.ipfrom);
    printf("ipto %s\n", et.ipto);
    printf("protocol %s\n", et.protocol);
    
    return Email_ParseFile(&et);

    //sleep(10);
    //return SCANNER_RET_OK;
    //return SCANNER_RET_VIRUS;
}


int do_virusaction(ProxyContext * p)
{
    return -1;
}


unsigned long send_mailfile(char * mailfile,  ProxyContext *p)
{

    struct LineBuffer *filebuf, *footbuf;
    int mailfd, footfd;
    int res = 0, sendret = 0, gotprd = 0, gottxt = 0, nogo = 0;
    unsigned long len = 0;
    char svrout[1];

    if ((mailfd = r_open2(mailfile, O_RDONLY )) < 0)
    {
        proxy_context_uninit(p);
        // Emergency: send_mailfile: Can't open client mail file.
        g_gateway_config->emergency = make_message("Can't open mailfile (%s)!", mailfile);
        do_log(LOG_EMERG, "ERR: Can't open mailfile (%s)!", mailfile);
        return 0;
    }
    filebuf = linebuf_init(16384);
    footbuf = linebuf_init(512);
    if (!filebuf)
    {
        r_close(mailfd);

        if (p->ssl_enable)
        {
            SSL_shutdown(p->ssl);
            /* 释放SSL */
            SSL_free(p->ssl);
        }

        r_close(p->server_fd);
        proxy_context_uninit(p);
        // Emergency: send_mailfile: Unable to get memory.
        g_gateway_config->emergency = "Could not allocate memory for sending mail!";
        do_log(LOG_EMERG, "ERR: Could not allocate memory for sending mail!");
    }

    gotprd = 0;
    /*  // advance to mailfd pointer to past data already sent:
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

            if ((strncmp(filebuf->line, ".", 1 ) == 0 && strlen(filebuf->line) == 1))
                gotprd = 1;
            if ((strncmp(filebuf->line, ".\r", 2) == 0 && strlen(filebuf->line) == 2))
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
                proxy_context_uninit(p);
                linebuf_uninit(filebuf);
                // Emergency: send_mailfile: Sending mail to client
                g_gateway_config->emergency = "Sending mail to client";
                do_log(LOG_EMERG, "ERR: Sending mail to client");
                // we are dead now. Should not reach here. But allow it
                //to fall through in case LOG_EMERG is changed in the future.
                return 1;
            }
        }


    }

    if (res != GETLINE_EOF)
    {
        do_log(LOG_CRIT, "ERR: reading from mailfile %s, error code: %d", mailfile, res);
        linebuf_uninit(filebuf);
        return 1;
    }

    if (!gotprd)
    {
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
