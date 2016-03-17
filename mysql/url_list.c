/*************************************************************************
	> File Name: url_list.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时09分01秒
 ************************************************************************/

#include<stdio.h>
#include"url_list.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="URL_list";
static int rowNum=0;

urllistPtr load_url_list(void* A, void* B)
{

    FetchRtePtr val=NULL;
    urllistPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table url list\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(urllistPtr)malloc(sizeof(urllist)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for url list\n");
        return NULL;
    }
    memset(prolist,0,sizeof(urllist)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].strategy_id,val->dataPtr[index][0],sizeof(prolist[index].strategy_id)-1);
        memcpy(prolist[index].strategy_type,val->dataPtr[index][1],sizeof(prolist[index].strategy_type)-1);
        memcpy(prolist[index].strategy_content,val->dataPtr[index][2],sizeof(prolist[index].strategy_content)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void print_urllist(void)
{
    urllistPtr var=load_url_list(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printurllistSwitch\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_content,var[rowindex].url_level,var[rowindex].level_info);
        rowindex++;
    }
    urllistRelase(&var);
}
void urllistRelase(urllistPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

