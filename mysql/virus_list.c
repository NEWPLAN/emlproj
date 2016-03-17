/*************************************************************************
	> File Name: virus_list.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时06分58秒
 ************************************************************************/

#include<stdio.h>
#include"virus_list.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="virus_list";
static int rowNum=0;

viurlistPtr load_virus_list(void* A, void* B)
{
    FetchRtePtr val=NULL;
    viurlistPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table virus list\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(viurlistPtr)malloc(sizeof(viruslist)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for virus list\n");
        return NULL;
    }
    memset(prolist,0,sizeof(viruslist)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].strategy_id,val->dataPtr[index][0],sizeof(prolist[index].strategy_id)-1);
        memcpy(prolist[index].strategy_type,val->dataPtr[index][1],sizeof(prolist[index].strategy_type)-1);
        memcpy(prolist[index].strategy_level,val->dataPtr[index][2],sizeof(prolist[index].strategy_level)-1);
        memcpy(prolist[index].strategy_info,val->dataPtr[index][3],sizeof(prolist[index].strategy_info)-1);
        memcpy(prolist[index].strategy_terminal,val->dataPtr[index][4],sizeof(prolist[index].strategy_terminal)-1);
        memcpy(prolist[index].strategy_target,val->dataPtr[index][5],sizeof(prolist[index].strategy_target)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void printvirus_list(void)
{
    viurlistPtr var=load_virus_list(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_virus_list\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_level,
               var[rowindex].strategy_info,var[rowindex].strategy_terminal,var[rowindex].strategy_target);
        rowindex++;
    }
    virus_listRelase(&var);
}
void virus_listRelase(viurlistPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

