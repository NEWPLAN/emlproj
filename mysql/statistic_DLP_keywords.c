/*************************************************************************
	> File Name: statistic_DLP_keywords.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时22分08秒
 ************************************************************************/

#include<stdio.h>
#include"statistic_DLP_keywords.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_DLP_keywords";
static int rowNum=0;


statisticwordsPtr load_statistic_DLP_keywords(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statisticwordsPtr sta_keyword_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table Statistic_DLP_keywords\n");
        return NULL;
    }
    rowNum=val->row;
    sta_keyword_list=(statisticwordsPtr)malloc(sizeof(statisticwords)*(val->row+1));
    if(!sta_keyword_list)
    {
        printf("error in malloc for Statistic_DLP_keywords\n");
        return NULL;
    }
    memset(sta_keyword_list,0,sizeof(statisticwords)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(sta_keyword_list[index].dlp_keywords_id,val->dataPtr[index][0],sizeof(sta_keyword_list[index].dlp_keywords_id)-1);
        memcpy(sta_keyword_list[index].dlp_keywords_eml_id,val->dataPtr[index][1],sizeof(sta_keyword_list[index].dlp_keywords_eml_id)-1);
        memcpy(sta_keyword_list[index].dlp_keywords_hitlist,val->dataPtr[index][2],sizeof(sta_keyword_list[index].dlp_keywords_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    return sta_keyword_list;
}

void printStatistic_DLP_keywords(void)
{
    statisticwordsPtr var=load_statistic_DLP_keywords(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printStatistic_DLP_keywords\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].dlp_keywords_id,var[rowindex].dlp_keywords_eml_id,var[rowindex].dlp_keywords_hitlist);
        rowindex++;
    }
    Statistic_DLP_keywordsRelase(&var);
}
void Statistic_DLP_keywordsRelase(statisticwordsPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

