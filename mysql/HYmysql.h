//
//  mysql.h
//  cTEST
//
//  Created by tsinghua476 on 16/3/7.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef mysql_h
#define mysql_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#define MAX_BUF 4096
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int database_connect_local(const char *password);
int database_connect(const char *IPAddr,const char *password);

int database_insert(const char *tableName,char *parameter);
int database_delete(const char *tableName,char *parameter);
int database_update(const char *tableName,char *parameter);
int database_query_withConditions(const char *tableName,char *condition);
int database_query (const char *tableName);

#endif /* mysql_h */

