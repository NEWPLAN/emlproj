/*************************************************************************
	> File Name: strategywords.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时04分24秒
 ************************************************************************/

#include<stdio.h>
#include "strategywords.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="DLP_list_keywords";
static int rowNum=0;

strategywordsPtr load_strategywords(void* A, void* B)
{

    FetchRtePtr val=NULL;
    strategywordsPtr stgwd=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table strategy key words\n");
        return NULL;
    }
    rowNum=val->row;
    stgwd=(strategywordsPtr)malloc(sizeof(strategywords)*(val->row+1));
    if(!stgwd)
    {
        printf("error in malloc for strategy key words\n");
        return NULL;
    }
    memset(stgwd,0,sizeof(strategywords)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(stgwd[index].strategy_id,val->dataPtr[index][0],sizeof(stgwd[index].strategy_id)-1);
        memcpy(stgwd[index].strategy_type,val->dataPtr[index][1],sizeof(stgwd[index].strategy_type)-1);
        memcpy(stgwd[index].strategy_level,val->dataPtr[index][2],sizeof(stgwd[index].strategy_level)-1);
        memcpy(stgwd[index].strategy_info,val->dataPtr[index][3],sizeof(stgwd[index].strategy_info)-1);
        memcpy(stgwd[index].strategy_terminal,val->dataPtr[index][4],sizeof(stgwd[index].strategy_terminal)-1);
        memcpy(stgwd[index].strategy_target,val->dataPtr[index][5],sizeof(stgwd[index].strategy_target)-1);
        memcpy(stgwd[index].strategy_content,val->dataPtr[index][6],sizeof(stgwd[index].strategy_content)-1);
    }
    free_memory(val);
    val=NULL;
    return stgwd;
}

void print_strategywords(void)
{
    strategywordsPtr var=load_strategywords(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print strategy key words\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_level,
               var[rowindex].strategy_info,var[rowindex].strategy_terminal,var[rowindex].strategy_target,
               var[rowindex].strategy_content);
        rowindex++;
    }
    strategywordsRelase(&var);
}
void strategywordsRelase(strategywordsPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

