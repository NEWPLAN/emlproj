/*************************************************************************
	> File Name: statistic_keyclass_basic.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时17分54秒
 ************************************************************************/

#include<stdio.h>
#include "statistic_keyclass_basic.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_keywordclass_Basic";
static int rowNum=0;

dlpkeyclassPtr load_dlpkeyclass(void* A, void* B)
{

    FetchRtePtr val=NULL;
    dlpkeyclassPtr dlp_keyclass=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table dlpkeyclass \n");
        return NULL;
    }
    rowNum=val->row;
    dlp_keyclass=(dlpkeyclassPtr)malloc(sizeof(dlpkeyclass)*(val->row+1));
    if(!dlp_keyclass)
    {
        printf("error in malloc for dlpkeyclass \n");
        return NULL;
    }
    memset(dlp_keyclass,0,sizeof(dlpkeyclass)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(dlp_keyclass[index].dlp_keywordsclass_id,val->dataPtr[index][0],sizeof(dlp_keyclass[index].dlp_keywordsclass_id)-1);
        memcpy(dlp_keyclass[index].dlp_keywordsclass_eml_id,val->dataPtr[index][1],sizeof(dlp_keyclass[index].dlp_keywordsclass_eml_id)-1);
        memcpy(dlp_keyclass[index].dlp_keyclass_hitlist,val->dataPtr[index][2],sizeof(dlp_keyclass[index].dlp_keyclass_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    return dlp_keyclass;
}

void print_dlpkeyclass(void)
{
    dlpkeyclassPtr var=load_dlpkeyclass(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_dlpkeyclass\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].dlp_keywordsclass_id,var[rowindex].dlp_keywordsclass_eml_id,var[rowindex].dlp_keyclass_hitlist);
        rowindex++;
    }
    dlpkeyclassRelase(&var);
}
void dlpkeyclassRelase(dlpkeyclassPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

