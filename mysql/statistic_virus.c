/*************************************************************************
	> File Name: statistic_virus.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时12分04秒
 ************************************************************************/

#include<stdio.h>
#include "statistic_virus.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Virus_Basic";
static int rowNum=0;

statvirusPtr load_statistic_virus(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statvirusPtr viruslist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statvirus switch\n");
        return NULL;
    }
    rowNum=val->row;
    viruslist=(statvirusPtr)malloc(sizeof(statvirus)*(val->row+1));
    if(!viruslist)
    {
        printf("error in malloc for statvirus switch\n");
        return NULL;
    }
    memset(viruslist,0,sizeof(statvirus)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(viruslist[index].virus_id,val->dataPtr[index][0],sizeof(viruslist[index].virus_id)-1);
        memcpy(viruslist[index].virus_eml_id,val->dataPtr[index][1],sizeof(viruslist[index].virus_eml_id)-1);
        memcpy(viruslist[index].virus_filename,val->dataPtr[index][2],sizeof(viruslist[index].virus_filename)-1);
        memcpy(viruslist[index].virus_info,val->dataPtr[index][3],sizeof(viruslist[index].virus_info)-1);
    }
    free_memory(val);
    val=NULL;
    return viruslist;
}

void print_statvirus(void)
{
    statvirusPtr var=load_statistic_virus(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printstatvirusSwitch\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].virus_id,var[rowindex].virus_eml_id,var[rowindex].virus_filename,var[rowindex].virus_info);
        rowindex++;
    }
    statvirusRelase(&var);
}
void statvirusRelase(statvirusPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

