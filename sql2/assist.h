/*************************************************************************
	> File Name: assist.h
	> Author:
	> Mail:
	> Created Time: 2016年04月06日 星期三 10时20分25秒
 ************************************************************************/

#ifndef SQL_ASSIST_H
#define SQL_ASSIST_H



#include "dboperate.h"

int checkInGateway(char* owner);

FetchRtePtr sql_query(char* command);

char* getDomain(char* owner);
#endif
