/*************************************************************************
	> File Name: statistic.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月18日 星期一 15时25分27秒
 ************************************************************************/

#include <stdio.h>
#include "statistic.h"
#include "all.h"
#include "dboperate.h"

static const char* const tables="syssuperuser_operationreport";
int insertLogs(char* commands)
{
	//char dbops[1024]={0};
	char items[1024]={0};
	//sprintf(dbops,"now(),'pop3', 'block', 'spam', 'spam', '192.168.1.1', '192.168.1.2', 'bupllrl@163.com', 'lindu@163.com'");
	sprintf(items,"`time`, `protocol`, `strategy`, `reason`, `detail`, `sourceip`, `destip`, `sender`, `receiver`");
	database_insert_withcond(tables, items, commands);
	return 0;
}
