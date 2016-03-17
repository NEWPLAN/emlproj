/*************************************************************************
	> File Name: statistic_DLP_url.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时20分52秒
 ************************************************************************/

#include<stdio.h>
#include "statistic_DLP_url.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_DLP_URL";
static int rowNum=0;

statisticurlPtr load_statisticURL(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statisticurlPtr staurl_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statisticurl \n");
        return NULL;
    }
    rowNum=val->row;
    staurl_list=(statisticurlPtr)malloc(sizeof(statisticurl)*(val->row+1));
    if(!staurl_list)
    {
        printf("error in malloc for statisticurl \n");
        return NULL;
    }
    memset(staurl_list,0,sizeof(statisticurl)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(staurl_list[index].dlp_url_id,val->dataPtr[index][0],sizeof(staurl_list[index].dlp_url_id)-1);
        memcpy(staurl_list[index].dlp_url_eml_id,val->dataPtr[index][1],sizeof(staurl_list[index].dlp_url_eml_id)-1);
        memcpy(staurl_list[index].dlp_url_hitlist,val->dataPtr[index][2],sizeof(staurl_list[index].dlp_url_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    return staurl_list;
}

void print_statisticurl(void)
{
    statisticurlPtr var=load_statisticURL(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_statisticurl\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].dlp_url_id,var[rowindex].dlp_url_eml_id,var[rowindex].dlp_url_hitlist);
        rowindex++;
    }
    statisticurlRelase(&var);
}
void statisticurlRelase(statisticurlPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

