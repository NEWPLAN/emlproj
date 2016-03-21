/*************************************************************************
	> File Name: syslog.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时58分24秒
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

syslogPtr load_syslog(void* A, void* B);
void print_syslog(void);
void syslogRelase(syslogPtr* tables);
int SysLogTest(int flags);
#endif
