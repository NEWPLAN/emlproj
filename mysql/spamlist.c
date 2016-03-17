/*************************************************************************
	> File Name: spamlist.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时01分05秒
 ************************************************************************/

#include<stdio.h>
#include"spamlist.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="spam_list";
static int rowNum=0;

spamlistPtr load_spamlist(void* A, void* B)
{

    FetchRtePtr val=NULL;
    spamlistPtr spam_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table spamlist \n");
        return NULL;
    }
    rowNum=val->row;
    spam_list=(spamlistPtr)malloc(sizeof(spamlist)*(val->row+1));
    if(!spam_list)
    {
        printf("error in malloc for spamlist \n");
        return NULL;
    }
    memset(spam_list,0,sizeof(spamlist)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(spam_list[index].strategy_id,val->dataPtr[index][0],sizeof(spam_list[index].strategy_id)-1);
        memcpy(spam_list[index].strategy_type,val->dataPtr[index][1],sizeof(spam_list[index].strategy_type)-1);
        memcpy(spam_list[index].strategy_info,val->dataPtr[index][2],sizeof(spam_list[index].strategy_info)-1);
        memcpy(spam_list[index].strategy_terminal,val->dataPtr[index][3],sizeof(spam_list[index].strategy_terminal)-1);
        memcpy(spam_list[index].strategy_target,val->dataPtr[index][4],sizeof(spam_list[index].strategy_target)-1);
    }
    free_memory(val);
    val=NULL;
    return spam_list;
}

void print_spamlist(void)
{
    spamlistPtr var=load_spamlist(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_spamlist\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_info,var[rowindex].strategy_terminal,var[rowindex].strategy_target);
        rowindex++;
    }
    spamlistRelase(&var);
}
void spamlistRelase(spamlistPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

