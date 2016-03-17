/*************************************************************************
	> File Name: behaviorLog.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 12时49分29秒
 ************************************************************************/

#include<stdio.h>
#include "behaviorLog.h"
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

