//
//  mysql.h
//  cTEST
//
//  Created by tsinghua476 on 16/3/7.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef DB__mysql_h
#define DB__mysql_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#define MAX_BUF 4096
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


/*****************************************************************
 *从数据库读取的二维表格
 *dataPtr char**:二维表格指针
 *row int 行数
 *col int 列数
*****************************************************************/
typedef struct mysqlRte
{
    char*** dataPtr;
    int row,col;/*row 行数, col 列数*/
} FetchRte,*FetchRtePtr;


int database_connect_local(const char *password);
int database_connect(const char *IPAddr,const char *password);

int database_insert(const char *tableName,char *parameter);
int database_delete(const char *tableName,char *parameter);
int database_update(const char *tableName,char *parameter);
void* database_query_withConditions(const char *tableName,char *condition);
void* database_query (const char *tableName);
void free_memory(FetchRtePtr memptr);
int database_insert_withcond(const char *tableName,char* items,char *parameter);

/********************************************************
***flags
***-31----8-7-6-5-4-3-2-1-0
***31-8不用
***7：本地连接测试     &128 0x80
***6：远程链接测试     &64  0x40
***5：插入数据库测试   &32  0x20
***4：无条件查询数据库 &16  0x10
***3：打印查询数据结构 &8   0x08
***2：删除数据库      &4   0x04
***1：更新数据库      &2   0x02
***0：打印测试信息		&1	 0x01
********************************************************/
#define LOCAL_CONNECT_FLAGS		0x80
#define	REMOT_CONNECT_FLAGS		0x40
#define	INSERT_DB_FLAGS			0x20
#define	UNCON_QUERRY_FLAGS		0x10
#define	DISPLAY_DATA_FLAGS		0x08
#define	DELETE_DB_FLAGS			0x04
#define	UPDATE_DB_FLAGS			0x02
#define	DEBUG_DB_FLAGS			0x01

//#define	DELETE_DB_FLAGS			0x0400

#endif /* mysql_h */

