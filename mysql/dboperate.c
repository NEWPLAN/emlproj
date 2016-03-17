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
 *
 * 有选择的操作数据库
 *sql const char* :数据库语句指令
 *
 *
 * ***************************************************************/
static int mysql_opt(const char *sql)
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
static int mysql_insert (const char *sql)
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
static int mysql_delete(const char *sql)
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
static int mysql_update(const char *sql)
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
static void* mysql_get(const char *sql)
{
    MYSQL *conn_ptr;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    //MYSQL_FIELD *fd;
    int res=0, col=0, row=0,index=0;
    int rowindex=0,colindex=0;
    char ***tables=NULL;
    FetchRtePtr RteVal=(FetchRtePtr)malloc(sizeof(FetchRte));

    if(!RteVal)
    {
        printf("error in malloc for rteval\n");
        return 0;
    }
    else
    {
        RteVal->col=RteVal->row=0;
        RteVal->dataPtr=NULL;
    }
    /************初始化数据库*************/
    conn_ptr = mysql_init(NULL);
    if (!conn_ptr)
    {
        printf("error in init msyql\n");
        return NULL;
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
            res_ptr = mysql_store_result(conn_ptr); //取出结果集
            if(res_ptr)
            {

                /********************malloc for fetch result************************/
                row=(int)mysql_num_rows(res_ptr);/*get行数*/
                printf("%lu Rows\n",(unsigned long)row);
                col = mysql_num_fields(res_ptr);/*数据库列数*/

                /**********分配行数***********/
                tables=(char***)malloc(sizeof(char**)*(row+1));
                if(!tables)
                {
                    printf("error in malloc memory\n");
                    return NULL;
                }
                memset(tables,0,sizeof(char**)*(row+1));

                /*********分配列数*************/
                for(index=0; index<row; index++)
                {
                    tables[index]=(char**)malloc(sizeof(char*)*col);
                    if(NULL==tables[index])
                    {
                        printf("error in malloc memory\n");
                        return NULL;
                    }
                    memset(tables[index],0,sizeof(char*)*col);
                }
                while((sqlrow = mysql_fetch_row(res_ptr)))      //依次取出记录
                {
                    for(index = 0; index < col; index++)
                    {
                        //printf("%s\t", sqlrow[i]);              //输出
                        /***************处理字符串******************/
                        tables[rowindex][colindex]=(char*)malloc(sizeof(char)*strlen(sqlrow[index])+1);
                        memset(tables[rowindex][colindex],0,sizeof(char)*strlen(sqlrow[index])+1);
                        if(sqlrow[index])//拷贝到内存空间中去
                            memcpy(tables[rowindex][colindex],sqlrow[index],strlen(sqlrow[index]));
                        colindex++;
                    }
                    colindex=0;
                    rowindex++;
                    // printf("\n");
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
    {

        RteVal->col=col;
        RteVal->row=row;
        RteVal->dataPtr=tables;
    }
    return (void*)RteVal;
    //return EXIT_SUCCESS;
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
/****************************************************************
 * 数据库删除
 * tableName const char* 数据库表名
 * parameter const char* 参数
 * 返回结果：
*****************************************************************/
int database_delete(const char *tableName,char *parameter)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql, "delete from %s where %s",tableName,parameter);
    return mysql_delete(sql);
}
/****************************************************************
 * 数据库更新
 *tableName const char* 数据库表名
 *parameter const char* 参数
 * 返回结果：
*****************************************************************/
int database_update(const char *tableName,char *parameter)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql, "update %s %s",tableName,parameter);
    return mysql_update(sql);
}
/****************************************************************
 * 有条件查询数据库
 * tableName const char* 表明称
 * condition const char* 查询条件
 * 返回结果：
*****************************************************************/
void* database_query_withConditions (const char *tableName,char *condition)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql,"select * from %s where %s",tableName,condition);
    return  mysql_get(sql);
}
/****************************************************************
 * 数据库查询，获取整个表的内容
 * tableName const char* 数据库表名
 * 返回结果：
*****************************************************************/
void* database_query (const char *tableName)
{
    char sql[MAX_BUF] = {0};
    sprintf(sql,"select * from %s",tableName);
    return mysql_get(sql);
}
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
 ************************************************************** */
int database_connect_local(const char *password)
{
    return database_connect("localhost",password);
}

void free_memory(FetchRtePtr memptr)
{
    int rowindex,colindex;
    for(rowindex=0; rowindex<memptr->row; rowindex++)
    {
        for(colindex=0; colindex<memptr->col; colindex++)
        {
            free(memptr->dataPtr[rowindex][colindex]);
            memptr->dataPtr[rowindex][colindex]=NULL;
        }
        free(memptr->dataPtr[rowindex]);
        memptr->dataPtr[rowindex]=NULL;
    }
    free(memptr->dataPtr);
    memptr->dataPtr=NULL;
    free(memptr);
}

