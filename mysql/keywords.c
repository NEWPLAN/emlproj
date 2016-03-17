/*************************************************************************
	> File Name: keywords.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 12时54分44秒
 ************************************************************************/

#include<stdio.h>
#include "keywords.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="keyword_list";
static int rowNum=0;

keywordsPtr load_keywords(void* A, void* B)
{

    FetchRtePtr val=NULL;
    keywordsPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table keywords \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(keywordsPtr)malloc(sizeof(keywords)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for keywords \n");
        return NULL;
    }
    memset(prolist,0,sizeof(keywords)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].keyword,val->dataPtr[index][0],sizeof(prolist[index].keyword)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void print_keywords(void)
{
    keywordsPtr var=load_keywords(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_keywords\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\n",var[rowindex].keyword);
        rowindex++;
    }
    keywordsRelase(&var);
}
void keywordsRelase(keywordsPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

