/*************************************************************************
	> File Name: strategyclass.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时10分27秒
 ************************************************************************/

#include<stdio.h>
#include "strategyclass.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="DLP_list_keywordsclass";
static int rowNum=0;

strategyclassPtr load_strategyclass(void* A, void* B)
{

    FetchRtePtr val=NULL;
    strategyclassPtr keyclass=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table strategyclass \n");
        return NULL;
    }
    rowNum=val->row;
    keyclass=(strategyclassPtr)malloc(sizeof(strategyclass)*(val->row+1));
    if(!keyclass)
    {
        printf("error in malloc for strategyclass \n");
        return NULL;
    }
    memset(keyclass,0,sizeof(strategyclass)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(keyclass[index].strategy_id,val->dataPtr[index][0],sizeof(keyclass[index].strategy_id)-1);
        memcpy(keyclass[index].strategy_type,val->dataPtr[index][1],sizeof(keyclass[index].strategy_type)-1);
        memcpy(keyclass[index].strategy_level,val->dataPtr[index][2],sizeof(keyclass[index].strategy_level)-1);
        memcpy(keyclass[index].strategy_info,val->dataPtr[index][3],sizeof(keyclass[index].strategy_info)-1);
        memcpy(keyclass[index].strategy_terminal,val->dataPtr[index][4],sizeof(keyclass[index].strategy_terminal)-1);
        memcpy(keyclass[index].strategy_target,val->dataPtr[index][5],sizeof(keyclass[index].strategy_target)-1);
        memcpy(keyclass[index].strategy_content,val->dataPtr[index][6],sizeof(keyclass[index].strategy_content)-1);
    }
    free_memory(val);
    val=NULL;
    return keyclass;
}

void print_strategyclass(void)
{
    strategyclassPtr var=load_strategyclass(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printstrategyclass\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_level,
               var[rowindex].strategy_info,var[rowindex].strategy_terminal,var[rowindex].strategy_target
               ,var[rowindex].strategy_content);
        rowindex++;
    }
    strategyclassRelase(&var);
}
void strategyclassRelase(strategyclassPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

