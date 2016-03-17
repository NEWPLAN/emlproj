/*************************************************************************
	> File Name: statistic_spam.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时13分42秒
 ************************************************************************/

#include<stdio.h>
#include "statistic_spam.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Spam_Basic";
static int rowNum=0;

statspamPtr load_statistic_spam(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statspamPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statspam \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(statspamPtr)malloc(sizeof(statspam)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for statspam \n");
        return NULL;
    }
    memset(prolist,0,sizeof(statspam)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].spam_id,val->dataPtr[index][0],sizeof(prolist[index].spam_id)-1);
        memcpy(prolist[index].spam_eml_path,val->dataPtr[index][1],sizeof(prolist[index].spam_eml_path)-1);
        memcpy(prolist[index].spam_eml_score,val->dataPtr[index][2],sizeof(prolist[index].spam_eml_score)-1);
        memcpy(prolist[index].spam_eml_info,val->dataPtr[index][3],sizeof(prolist[index].spam_eml_info)-1);
        memcpy(prolist[index].eml_eml_id,val->dataPtr[index][4],sizeof(prolist[index].eml_eml_id)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void print_statspam(void)
{
    statspamPtr var=load_statistic_spam(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_statspam\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].spam_id,var[rowindex].spam_eml_path,var[rowindex].spam_eml_score,var[rowindex].spam_eml_info,var[rowindex].eml_eml_id);
        rowindex++;
    }
    statspamRelase(&var);
}
void statspamRelase(statspamPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

