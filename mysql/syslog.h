/*************************************************************************
	> File Name: syslog.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 17时04分26秒
 ************************************************************************/

#ifndef DB__SYSLOG_H
#define DB__SYSLOG_H

typedef struct SYSLOG
{
    char modular[255+1];
    char time[255+1];
    char level[255+1];
    char content[255+1];
} syslog, *syslogPtr;

int load_syslog(void* A, void* B);

#endif
