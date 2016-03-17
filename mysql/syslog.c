/*************************************************************************
	> File Name: syslog.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 12时57分34秒
 ************************************************************************/

#include<stdio.h>
#include "syslog.h"
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
        memcpy(prolist[index].content,val->dataPtr[index][2],sizeof(prolist[index].content)-1);
    }
    free_memory(val);
    val=NULL;
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

