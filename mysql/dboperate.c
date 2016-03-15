//
//  mysql.c
//  cTEST
//
//  Created by Tsinghua476 on 16/3/7.
//  Copyright © 2016年 Tsinghua476. All rights reserved.
//
#include "dboperate.h"

#define  defaultDB "mailgateway"

char defaultAddr[20] = "localhost";
char defaultPassWord[20] = "root";




/*****************************************************************
 *从数据库读取的二维表格
 *dataPtr char**:二维表格指针
 *row int 行数
 *col int 列数
*****************************************************************/
typedef struct mysqlRte
{
    char** dataPtr;
    int row,col;/*row means line number, col means vertical*/
} FetchRteType;


/****************************************************************
 * 连接数据库
 *IPAddr const char* ip 地址
 *password const char* 密码
 * 返回结果：
*****************************************************************/
int database_connect(const char *IPAddr,const char *password)
{

    MYSQL *conn_ptr;

    conn_ptr = mysql_init(NULL);
    if (!conn_ptr)
    {
        printf("mysql_init failed\n");
        return EXIT_FAILURE;
    }

    if (strcmp(IPAddr, "localhost") == 0 || strcmp(IPAddr, "127.0.0.1") == 0)
    {
        conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root",password, defaultDB, 0, NULL, 0);
    }
    else
    {
        conn_ptr = mysql_real_connect(conn_ptr, IPAddr, "root",password, defaultDB, 0, NULL, 0);
        strcpy(defaultAddr,IPAddr);
    }

    if (strcmp(defaultPassWord,password) != 0)
    {
        strcpy(defaultPassWord,password);
    }
    if (conn_ptr)
    {
        printf("Connection success\n");
    }
    else
    {
        printf("Connection failed\n");
    }
    mysql_close(conn_ptr);
    return EXIT_SUCCESS;
}
/****************************************************************
 *连接本地数据库
 *password const char* :连接数据库的密码
 *
 *
 ************************************************************** */
int database_connect_local(const char *password)
{
    return database_connect("localhost",password);
}
/*****************************************************************
 *
 * 有选择的操作数据库
 *sql const char* :数据库语句指令
 *
 *
 * ***************************************************************/
int mysql_opt(const char *sql)
{
    MYSQL *conn_ptr;
    int res;

    conn_ptr = mysql_init(NULL);/*init mysql db*/
    if (!conn_ptr)
    {
        printf("mysql_init failed\n");
        return EXIT_FAILURE;
    }

    conn_ptr = mysql_real_connect(conn_ptr, defaultAddr, "root", defaultPassWord, defaultDB, 0, NULL, 0);

    if (conn_ptr)
    {
        res = mysql_query(conn_ptr, sql);/*查询语句，返回0表示成功*/
        if (!res)       //输出受影响的行数
        {
            printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
        }
        else            //打印出错误代码及详细信息
        {
            fprintf(stderr, "Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
        }
    }
    else
    {
        printf("Connection failed\n");
    }
    mysql_close(conn_ptr);
    return EXIT_SUCCESS;
}


/*****************************************************************
 *数据库插入操作
 *sql char* const 插入指令
 *
*****************************************************************/
int mysql_insert (const char *sql)
{
    if(mysql_opt(sql) == 0)
    {
        printf("insert success\n");
        return 0;
    }
    else
    {
        return 1;
    }

}
/*****************************************************************
 *
 *数据库删除操作
 *
 *sql const char * 删除数据库语句
 *
 *
 *****************************************************************/
int mysql_delete(const char *sql)
{
    if(mysql_opt(sql) == 0)
    {
        printf("delete success\n");
        return 0;
    }
    else
    {
        return 1;
    }
}


/*****************************************************************
*
*数据库更新操作
*sql const char* 更新数据库语句
*
*****************************************************************/
int mysql_update(const char *sql)
{
    if(mysql_opt(sql) == 0)
    {
        printf("update success\n");
        return 0;
    }
    else
    {
        return 1;
    }
}
/**************************************
*获取数据库的数据信息
* sql const char* : 执行的数据库命令
**************************************/
int mysql_get(const char *sql)
{
    MYSQL *conn_ptr;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
    int res, i, j;

    conn_ptr = mysql_init(NULL);
    if (!conn_ptr)
    {
        return EXIT_FAILURE;
    }
    conn_ptr = mysql_real_connect(conn_ptr, defaultAddr, "root", "root", defaultDB, 0, NULL, 0);
    if (conn_ptr)
    {
        res = mysql_query(conn_ptr, sql); //查询语句
        if (res)
        {
            printf("SELECT error:%s\n",mysql_error(conn_ptr));
        }
        else
        {
            res_ptr = mysql_store_result(conn_ptr);             //取出结果集
            if(res_ptr)
            {
                printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));/*行数*/
                j = mysql_num_fields(res_ptr);/*数据库列数*/
                while((sqlrow = mysql_fetch_row(res_ptr)))      //依次取出记录
                {
                    for(i = 0; i < j; i++)
                        printf("%s\t", sqlrow[i]);              //输出
                    printf("\n");
                }
                if (mysql_errno(conn_ptr))
                {
                    fprintf(stderr,"Retrive error:%s\n",mysql_error(conn_ptr));
                }
            }
            mysql_free_result(res_ptr);
        }
    }
    else
    {
        printf("Connection failed\n");
    }
    mysql_close(conn_ptr);
    return EXIT_SUCCESS;
}

/***************************************************************
* 插入数据信息
* 插入格式：insert into TABLENAME values(VAL1,VAL2,...,VALN)
* tableName const char* : 数据表名称
****************************************************************/
int database_insert(const char *tableName,char *parameter)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql, "insert into %s values(%s)",tableName,parameter);
    return mysql_insert(sql);
}

int database_delete(const char *tableName,char *parameter)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql, "delete from %s where %s",tableName,parameter);
    return mysql_delete(sql);
}

int database_update(const char *tableName,char *parameter)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql, "update %s %s",tableName,parameter);
    return mysql_update(sql);
}

int database_query_withConditions (const char *tableName,char *condition)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql,"select * from %s where %s",tableName,condition);
    return  mysql_get(sql);
}

int database_query (const char *tableName)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql,"select * from %s",tableName);
    return mysql_get(sql);
}

