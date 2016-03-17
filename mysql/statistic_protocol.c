/*************************************************************************
	> File Name: statistic_protocol.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时15分59秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include"statistic_protocol.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_staprotocol";
static int rowNum=0;

staprotocolPtr load_statisticProtocol(void* A, void* B)
{

    FetchRtePtr val=NULL;
    staprotocolPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table staprotocol \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(staprotocolPtr)malloc(sizeof(staprotocol)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for staprotocol \n");
        return NULL;
    }
    memset(prolist,0,sizeof(staprotocol)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].protocol,val->dataPtr[index][0],sizeof(prolist[index].protocol)-1);
        memcpy(prolist[index].source_ip,val->dataPtr[index][1],sizeof(prolist[index].source_ip)-1);
        memcpy(prolist[index].dest_ip,val->dataPtr[index][2],sizeof(prolist[index].dest_ip)-1);
        memcpy(prolist[index].open_time,val->dataPtr[index][3],sizeof(prolist[index].open_time)-1);
        memcpy(prolist[index].close_time,val->dataPtr[index][4],sizeof(prolist[index].close_time)-1);
        memcpy(prolist[index].filepath,val->dataPtr[index][5],sizeof(prolist[index].filepath)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void print_staprotocol(void)
{
    staprotocolPtr var=load_statisticProtocol(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printstaprotocol\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].protocol,var[rowindex].source_ip,var[rowindex].dest_ip,var[rowindex].open_time,var[rowindex].close_time,var[rowindex].filepath);
        rowindex++;
    }
    staprotocolRelase(&var);
}
void staprotocolRelase(staprotocolPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

