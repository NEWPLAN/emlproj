/*************************************************************************
	> File Name: behaviorLog.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时49分29秒
 ************************************************************************/

#include<stdio.h>
#include "behavior_log.h"
#include "dboperate.h"


static const char * const tableName="behavior_log";
static int rowNum=0;

behaviorlogPtr load_behavoirLog(void* A, void* B)
{

    FetchRtePtr val=NULL;
    behaviorlogPtr behaviorlog_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table behaviorlog \n");
        return NULL;
    }
    rowNum=val->row;
    behaviorlog_list=(behaviorlogPtr)malloc(sizeof(behaviorlog)*(val->row+1));
    if(!behaviorlog_list)
    {
        printf("error in malloc for behaviorlog \n");
        return NULL;
    }
    memset(behaviorlog_list,0,sizeof(behaviorlog)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(behaviorlog_list[index].time,val->dataPtr[index][0],sizeof(behaviorlog_list[index].time)-1);
        memcpy(behaviorlog_list[index].level,val->dataPtr[index][1],sizeof(behaviorlog_list[index].level)-1);
        memcpy(behaviorlog_list[index].user_id,val->dataPtr[index][2],sizeof(behaviorlog_list[index].user_id)-1);
        memcpy(behaviorlog_list[index].object,val->dataPtr[index][3],sizeof(behaviorlog_list[index].object)-1);
        memcpy(behaviorlog_list[index].behavior,val->dataPtr[index][4],sizeof(behaviorlog_list[index].behavior)-1);
    }
    free_memory(val);
    val=NULL;
    return behaviorlog_list;
}

void print_behaviorlog(void)
{
    behaviorlogPtr var=load_behavoirLog(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_behaviorlog\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].time,var[rowindex].level,var[rowindex].user_id,var[rowindex].object,var[rowindex].behavior);
        rowindex++;
    }
    behaviorlogRelase(&var);
}
void behaviorlogRelase(behaviorlogPtr* tables)
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
int BehaviorLogTest(int flags)
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
        database_insert(tableName,"'2016:03:21','level',11,'object','behavior'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_behaviorlog();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set behavior='做啥子？' where user_id=11");
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
        print_behaviorlog();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"user_id=11");

    if(flags&DISPLAY_DATA_FLAGS)
        print_behaviorlog();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
