/*************************************************************************
	> File Name: syslog.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时57分34秒
 ************************************************************************/

#include<stdio.h>
#include "Syslog.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Syslog";
static int rowNum=0;

syslogPtr load_syslog(void* A, void* B)
{

    FetchRtePtr val=NULL;
    syslogPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table sys log\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(syslogPtr)malloc(sizeof(syslog)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for sys log\n");
        return NULL;
    }
    memset(prolist,0,sizeof(syslog)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].modular,val->dataPtr[index][0],sizeof(prolist[index].modular)-1);
        memcpy(prolist[index].time,val->dataPtr[index][1],sizeof(prolist[index].time)-1);
        memcpy(prolist[index].level,val->dataPtr[index][2],sizeof(prolist[index].level)-1);
        memcpy(prolist[index].content,val->dataPtr[index][3],sizeof(prolist[index].content)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return prolist;
}

void print_syslog(void)
{
    syslogPtr var=load_syslog(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printsyslog\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].modular,var[rowindex].time,var[rowindex].level,var[rowindex].content);
        rowindex++;
    }
    syslogRelase(&var);
}
void syslogRelase(syslogPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

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
int SysLogTest(int flags)
{
    char*** results=NULL/*,***tptr=NULL*/;
    FetchRtePtr retval=NULL;

	if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);

    if(flags&LOCAL_CONNECT_FLAGS)
        database_connect_local("root");

    if(flags&REMOT_CONNECT_FLAGS)
        database_connect("127.0.0.1","root");

    //int ret = database_insert("Protocol_switch","'789','789',123");

    if(flags&INSERT_DB_FLAGS)
        database_insert(tableName,"'modular','2016:03:21','level','content'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_syslog();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set content='对对对,content' where modular='modular'");
    //database_update("Protocol_switch","set protocol='456',direction='456' where switch=456");
    if(flags&UNCON_QUERRY_FLAGS)
    {
        retval=(FetchRtePtr)database_query(tableName);
        // results=(char***)database_query("Protocol_switch");
        results=retval->dataPtr;
        if(results==NULL)
            return 0;
    }
    if(flags&DISPLAY_DATA_FLAGS)
        print_syslog();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"modular='modular'");

    if(flags&DISPLAY_DATA_FLAGS)
        print_syslog();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
